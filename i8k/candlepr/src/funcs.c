#include "funcs.h"

int exception(int e){
	inwPrint("Error occurs %i\r\n",e);
	return 0;
}
int sendCommand(unsigned long command){
	DO_32(DOSLOT,command);
	inwPrint("DO data = %-08lX\n\r",command);
	return 0;
}
int readSignals(unsigned long *data){
	unsigned long di;
	di = DI_32(DISLOT);
	memcpy(data,&di,4);
	inwPrint("DI data = %08X\n\r",data);
	return 0;
}

int readEncoderL(long *up,long *down){
    int ret=0;
    ret = readEncoder(0,up);
    if(ret)return ret;
    ret = readEncoder(1,down);
    return ret;
}
int readEncoderR(long *up,long *down){
    int ret=0;
    ret = readEncoder(2,up);
    if(ret)return ret;
    ret = readEncoder(3,down);
    return ret;
}
int readEncoder(int channel,long *data){
    int Overflow;
    i8080_ReadCntUpDown(ECSLOT,channel,data,&Overflow);
    inwPrint("[%i]=%04x\r\n",channel,data);
    return 0;
}
int getRuntime(psRuntimeValues prtv){
    EE_MultiRead(EEPROM_RUNTIME,0,sizeof(sRuntimeValues),(psRuntimeValues)prtv);
    return 0;
}
int setRuntime(sRuntimeValues rtv){
    EE_WriteEnable();
    EE_MultiWrite(EEPROM_RUNTIME,0,sizeof(sRuntimeValues),(psRuntimeValues)&rtv);
    EE_WriteProtect();
    return 0;
}
int log2EEPROM(char*d,char* o,int len){
	char reset[256];
	memset(reset,0xff,256);
	EE_WriteEnable();
	EE_MultiWrite(EEPROM_TOTAL+1,0,256,reset);
	EE_MultiWrite(EEPROM_TOTAL+2,0,256,reset);
    EE_MultiWrite(EEPROM_TOTAL+1,0,17,(char*)d);
    EE_MultiWrite(EEPROM_TOTAL+1,19,len,(char*)o);
    EE_WriteProtect();
    return 0;
}
byte lrc(byte *buf, int len){
	byte LRC =0;
	int i=0;
	for(i = 1; i < len; i++) {
		LRC += buf[i];
	}
	return (byte)-LRC;
}
sModbusPack parse(char* in,int len){
	sModbusPack ret;
	ret.colon=':';
	ret.addr=16*ascii_to_hex(in[1]) + ascii_to_hex(in[2]);
	ret.func=16*ascii_to_hex(in[3]) + ascii_to_hex(in[4]);
	ret.reg	=4096*ascii_to_hex(in[5]) + 256*ascii_to_hex(in[6]) + 16*ascii_to_hex(in[7]) + ascii_to_hex(in[8]);
	ret.data=4096*ascii_to_hex(in[9]) + 256*ascii_to_hex(in[10]) + 16*ascii_to_hex(in[11]) + ascii_to_hex(in[12]);
	ret.lrc	=16*ascii_to_hex(in[13]) + ascii_to_hex(in[14]);
	return ret;
}
void serialize(sModbusPack mb,byte *ret){
	int i=5;
	memset(ret,0,256);
	ret[0] = (byte)mb.colon;
	ret[1] = (byte)mb.addr/16;
	ret[2] = (byte)mb.addr%16;
	ret[3] = (byte)mb.func/16;
	ret[4] = (byte)mb.func%16;
	memcpy(ret+5,mb.response,mb.response_size);
	mb.lrc=lrc(ret,5+mb.response_size);
	ret[5+mb.response_size] = (byte)mb.lrc/16;
	ret[6+mb.response_size] = (byte)mb.lrc%16;
	for(i=0;i<(6+mb.response_size);i++){
		ret[i]=hex_to_ascii[(int)ret[i]];
	}
	ret[7+mb.response_size] = 0x0a;
	ret[8+mb.response_size] = 0x0d;
}
int checkLRC(sModbusPack mb){
	int lrc;
	sModbusPack ch;
	byte c[18];
	ch=mb;
	serialize(ch,c);
	lrc = 16*ascii_to_hex(c[13]) + ascii_to_hex(c[14]);
	Print(c);
	Show5DigitLed(1,8);
	if(lrc==mb.lrc){
		 leds(0xffff);
	}else{
		leds(lrc);
		leds(mb.lrc);
	}
	return (lrc==mb.lrc)?0:1;
}
sModbusPack analyzeModBus(sModbusPack mb){
	int ret =0;
	//Show5DigitLed(1,10); leds(pmb->addr);//addr
	//Show5DigitLed(1,15); leds(pmb->func);
	//Show5DigitLed(1,13); leds(pmb->reg);
	//Show5DigitLed(1,17); leds(pmb->data);
	switch(mb.func){
		case 0x03:{ // read single register
			EE_MultiRead(EEPROM_RUNTIME,mb.reg,2*mb.data,(byte*)mb.response);
			Show5DigitLed(1,13); leds(2*mb.data);
			mb.response_size=2*mb.data;
		}break;
		case 0x06:{ // write single register

		}break;
	}
	return mb;
}
int readModbus(){
	int ret = 0,i=0;
	byte inbuf[256],outbuf[256];
	sModbusPack mb,omb;
	memset(mb.response,0,256);
	memset(inbuf,0,sizeof(inbuf));
	memset(outbuf,0,sizeof(outbuf));
	if(Kbhit()){
		i=LineInput(inbuf,sizeof(inbuf));
		Getch();//get LF
		if(inbuf[0]==':'){
			mb=parse(inbuf,i);
			omb=analyzeModBus(mb);
			serialize(omb,outbuf);
			Puts(outbuf);
			Show5DigitLed(1,14); leds(mb.response_size+8);
			log2EEPROM(inbuf,outbuf,mb.response_size+8);
			Show5DigitLed(1,15);
			ledstr(outbuf,18);
			leds(mb.lrc);
		}
	}
	return ret;
}
int getTotal(psTotalValues ptv){
    EE_MultiRead(EEPROM_TOTAL,0,sizeof(sTotalValues),(psTotalValues)ptv);
    return 0;
}
int setTotal(sTotalValues tv){
    EE_WriteEnable();
    EE_MultiWrite(EEPROM_TOTAL,0,sizeof(sTotalValues),(psTotalValues)&tv);
    EE_WriteProtect();
    return 0;
}
int initInw(psRuntimeValues prtv, psTotalValues ptv){
    int iRet=0,channel;
    // Always do that to init lib i8000
	InitLib();
    // i8000 has only one systimer
	TimerOpen();
	TimerResetValue();
	//InitEncoder
	iRet=i8080_InitDriver(ECSLOT);
    if (iRet==(-1)){
		inwPrint("Initiate 8080 on slot%d error!\n\r",ECSLOT);
		inwPrint("  Cannot find 8080.\r\n");
	}
	else{
		inwPrint("Initiate 8080 on slot%d ok.\n\r",ECSLOT);
        if(iRet>0){
            inwPrint("  Some Pulse/Dir channels have one count offset.\n\r");
            inwPrint("  Return code:%02X\n\r",iRet);
        }
        for (channel=0; channel<8; channel++){
            i8080_SetXorRegister(ECSLOT,channel,0); // XOR=0 (Low Actived)
            i8080_SetChannelMode(ECSLOT,channel,1); // Up/Down counter mode
                //mode 0: Pulse/Dir counter mode
                //     1: Up/Down counter mode
                //     2: frequency mode
                //     3: Up counter mode

            i8080_SetLowPassFilter_Status(ECSLOT,channel,0); //Disable LPF
            i8080_SetLowPassFilter_Us(ECSLOT,channel,1); //Set LPF width= 0.001 ms
        }

        //Clear all count at beginning.
        for (channel=0; channel<8; channel++){
            i8080_ClrCnt(ECSLOT,channel);
        }
    }
    iRet = getTotal(ptv);
    if(iRet) return iRet;
    iRet = getRuntime(prtv);
    if(iRet) return iRet;
	if(prtv->tV10==0){
		/*
		byte tV10;byte tV19;
	   int LC;int Lh;int Lr;
	   int RC;int Rh;int Rr;
	   int L;int R;int tstart;int ttaker;
	   int CL;int CR;int TL;int TR;
		 */
		sRuntimeValues rtv = {
			5,5,
			100,150,60,
			100,150,60,
			100,100,5,5,
			0,0,0,0
		};
		memcpy(prtv,&rtv,sizeof(sRuntimeValues));
		iRet = setRuntime(rtv);
	}
	// Init leds
	Init5DigitLed();
	Show5DigitLed(1,16);
	Show5DigitLed(2,16);
	Show5DigitLed(3,16);
	Show5DigitLed(4,16);
	Show5DigitLed(5,16);
	// Init ComPort
    return 0;
}
/*
 *
 */
void ledstr(char *str,int len){
	int row[5],i=0;
	Show5DigitLed(1,15);
	Show5DigitLed(2,13);
	Show5DigitLed(3,10);
	Show5DigitLed(4,13);
	Show5DigitLed(5,10);
	DelayMs(800);

	Show5DigitLed(1,ascii_to_hex((int)str[5]));
	Show5DigitLed(2,ascii_to_hex((int)str[6]));
	Show5DigitLed(3,ascii_to_hex((int)str[7]));
	Show5DigitLed(4,ascii_to_hex((int)str[8]));
	Show5DigitLed(5,ascii_to_hex((int)str[9]));
}
void leds(int s){
	int p1,p2,p3,p4;
	if(s<0){
		Show5DigitLed(2,15);
		Show5DigitLed(3,15);
		Show5DigitLed(4,15);
		Show5DigitLed(5,15);
	}
	else if(s<16){
		Show5DigitLed(2,16);
		Show5DigitLed(3,16);
		Show5DigitLed(4,16);
		Show5DigitLed(5,s);
	}
	else if(s<256){
		Show5DigitLed(2,16);
		Show5DigitLed(3,16);
		Show5DigitLed(4,s/16);
		Show5DigitLed(5,s%16);
	}
	else if(s<4096){
		p2 = s/256;
		p3 = (s-p2*256)/16;
		p4 = s-p2*256-p3*16;
		Show5DigitLed(2,16);
		Show5DigitLed(3,p2);
		Show5DigitLed(4,p3);
		Show5DigitLed(5,p4);
	}
	else{
		p1 = s/4096;
		p2 = (s-4096*p1)/256;
		p3 = (s-4096*p1-p2*256)/16;
		p4 = (s-4096*p1-p2*256-p3*16);
		Show5DigitLed(2,p1);
		Show5DigitLed(3,p2);
		Show5DigitLed(4,p3);
		Show5DigitLed(5,p4);
	}
	DelayMs(800);
}
void ledsOff(){
	Show5DigitLed(2,16);
	Show5DigitLed(3,16);
	Show5DigitLed(4,16);
	Show5DigitLed(5,16);
}
/*
 * Start scenario
 */
/*
 * Work scenario automate
 */
int loadMainScenario(sRuntimeValues rtv,sStep * sc){
     /*sStep scenatioW[]={
         {0x00000002,{0,0x00000002},0x00000000,0},  //00    убираем засыпку V2 -> A2
         {0x00000008,{0,0x00000008},0x00000000,0},  //01    верхний поршень прижимает V4 -> A4
         {0x00000410,{2,0},0x00000000,0},      //02    нижний поршень прижимает и отпускает фитиль(V11) V5->LS
         {0x00000008,{0,0x00000010},0x00000000,0},  //03    верхний дожимает V4 -> A5
         {0x00000004,{0,0x00000004},0x00000000,0},  //04    убираем вверхний V3->A3
         {0x00000080,{0,0x00000080},0x00000000,0},  //05    выдвигаем ухват V8->A8
         {0x00000410,{0,0x00000020},0x00000000,0},  //06    нижний поршень выдвигает и отпускает фитиль (V11) V5->A5
         {0x00000100,{1,0},0x00000100,0},  //07    зажимаем ухват и чуть ждем  V9
         {0x00000120,{2,0},0x00000100,0},      //08    опускаем нижний поршень V6->LH
         {0x00000300,{1,0},0x00000100,0},    //09    отрезаем свечу V10 на время tV10
         {0x00000140,{0,0x00000040},0x00000000,0},  //10    убираем ухват и отпускаем зажим V7->A7 V9=off
         {0x00000140,{0,0x00000040},0x00000000,0},  //11    ставим засыпаться левый цилиндр засыпку V1 -> A1

         {0x00000000,{3,0x00000000},0x00000000,0},  //12    Произвести фиксацию замеров для левого

         {0x00001000,{0,0x00000200},0x00000000,0},  //13    верхний поршень прижимает V13 -> A10
         {0x00082000,{2,0},0x00000000,0},      //14    нижний поршень прижимает и отпускает фитиль(V20) V14->RS
         {0x00001000,{0,0x00000400},0x00000000,0},  //15    верхний дожимает V13 -> A11
         {0x00000800,{0,0x00000100},0x00000000,0},  //16    убираем вверхний V12->A9
         {0x00008000,{0,0x00001000},0x00000000,0},  //17    выдвигаем ухват V16->A13
         {0x00002000,{0,0x00000800},0x00000000,0},  //18    нижний поршень выдвигает и отпускает фитиль (V20) V4->A12
         {0x00020000,{1,0},0x00020000,0},  //19    зажимаем ухват и чуть ждем  V18
         {0x00024000,{2,0},0x00020000,0},      //20    опускаем нижний поршень V15->RH
         {0x00060000,{1,0},0x00020000,0},    //21    отрезаем свечу V19 на время tV19
         {0x00030000,{0,0x00002000},0x00000000,0},  //22    убираем ухват и отпускаем зажим V17->A14 V18=off

         {0x00000000,{4,0x00000000},0x00000000,0}   //23    Произвести фиксацию замеров для левого
     };*/
	 sStep scenatioW[] = {
		 {0x00000002,{0,0x00002146},0x00000000,0},//00
		 {0x00000008,{0,0x0000214a},0x00000000,0},//01
		 {0x00000410,{2,0x0000214a/*rtv.LC*/},0x00000000,0},//02
		 {0x00000008,{0,0x00002152},0x00000000,0},//03
		 {0x00000004,{0,0x00002146},0x00000000,0},//04
		 {0x00000080,{0,0x00002186},0x00000000,0},//05
		 {0x00000410,{0,0x000021a6},0x00000000,0},//06
		 {0x00000100,{1,0x000021a6/*rtv.ttaker*/},0x00000100,0},//07
		 {0x00000120,{2,0x00002186/*rtv.Lh*/},0x00000100,0},//08
		 {0x00000300,{1,0x00002186/*rtv.tV10*/},0x00000100,0},//09
		 {0x00000140,{0,0x00002146},0x00000000,0},//10
		 {0x00000001,{0,0x00002145},0x00000000,0},//11

		 {0x00000000,{3,0x00002145},0x00000000,0},  //12    Произвести фиксацию замеров для левого

		 {0x00001000,{0,0x00002245},0x00000000,0},//13
		 {0x00082000,{2,0x00002245/*rtv.RC*/},0x00000000,0},//14
		 {0x00001000,{0,0x00002445},0x00000000,0},//15
		 {0x00000800,{0,0x00002145},0x00000000,0},//16
		 {0x00008000,{0,0x00001145},0x00000000,0},//17
		 {0x00082000,{0,0x00001945},0x00000000,0},//18
		 {0x00020000,{1,0x00001945/*rtv.ttaker*/},0x00020000,0},//19
		 {0x00024000,{2,0x00001145/*rtv.Rh*/},0x00020000,0},//20
		 {0x00060000,{1,0x00001145/*rtv.tV19*/},0x00020000,0},//21
		 {0x00030000,{0,0x00002145},0x00000000,0},//22

		 {0x00000000,{4,0x00002145},0x00000000,0},  //12    Произвести фиксацию замеров для левого
	 };
     scenatioW[2].wait.value = rtv.LC;
     scenatioW[7].wait.value = rtv.ttaker;
     scenatioW[8].wait.value = rtv.Lh;
     scenatioW[9].wait.value = rtv.tV10;
     scenatioW[14].wait.value = rtv.RC;
     scenatioW[19].wait.value = rtv.ttaker;
     scenatioW[20].wait.value = rtv.Rh;
     scenatioW[21].wait.value = rtv.tV19;
     memcpy(sc,scenatioW,sizeof(scenatioW));
     return 0;
}
/*
 * Start scenario automate
 */
int loadStartScenario(sRuntimeValues rtv,sStep * sc){
	sStep scenatioW[]={
		{0x00000001,{0,0x00000001},0x00000000,0},//00
		{0x00000002,{0,0x00004000},0x00000000,0},//01
		{0x00000044,{0,0x00004044},0x00000000,0},//02
		{0x00000410,{0,0x00004064},0x00000000,0},//03
		{0x00000020,{2,/*rtv.Lh*/},0x00000000,0},//04
		{0x00000800,{0,0x00004144},0x00000000,0},//05
		{0x00012000,{0,0x00006944},0x00000000,0},//06
		{0x00014000,{2,/*rtv.Rh*/},0x00000000,0},//07
	 };
	 scenatioW[4].wait.value = rtv.Lh;
     scenatioW[7].wait.value = rtv.Rh;
	 memcpy(sc,scenatioW,sizeof(scenatioW));
     return 0;
 }
/*
 * Init scenario automate
 */
int loadInitScenario(sRuntimeValues rtv,sStep * sc){
	sStep scenatioW[]={
		{0x00000020,{2,0/*rtv.Lh*/},0x00000000,0},//00
		{0x00000001,{0,0x00002945},0x00000000,0},//01
		{0x00002000,{2,0x00002145/*rtv.Rh*/},0x00000000,0},//02
		{0x00000000,{1,0x00002145/*rtv.tstart*/},0x00000000,0},//03
	 };
	 scenatioW[0].wait.value = rtv.Lh;
     scenatioW[2].wait.value = rtv.Rh;
     scenatioW[3].wait.value = rtv.tstart;
	 memcpy(sc,scenatioW,sizeof(scenatioW));
     return 0;
 }


/*=====================================================================*/
/*==== Following functions are used to receive data from COM port.  ===*/
/*==== You can copy the functions to your own program.              ===*/
/*=====================================================================*/
/*
The following two functions are implemented by [non-block] method.

Non-Block method: After calling the function, if there is no data in the
			   COM port input buffer, the CPU skip the function and
			   go to execute next code.
*/
int Receive_Data(int iPort,unsigned char* cInBuf,char cTerminator,long lTimeout){
	/*
	 Uses COM port to receive data with a terminative char.

	 iPort:    COM port number to receive data.
			   0:COM0, 1:COM1, 2:COM2  .....
	 *cInBuf:  Input buffer to receive data.
	 cTerminator: what is the last byte ?
	 lTimeout: timeout to receive data. (Unit: ms)
			   The timeout is measured from last received byte
			   to the terminator.
	 return: >0 :length of received data
			  0 :doen't receive any data
			 -1 :timeout
	*/
 	unsigned char cChar;
 	int iIndex=0;
 	unsigned long lStartTime;
	if(IsCom(iPort)){
		lStartTime=GetTimeTicks();
	 	for(;;){
			while(IsCom(iPort)) {//check COM port
				cChar=ReadCom(iPort);
			 	if(cChar==cTerminator){/* the terminal char is 0x0D */
					cInBuf[iIndex]=0;  /* Add the zero end to the data. */
				 	return iIndex;     /* return data length                */
				}/* (doesn't include the cTerminator) */
			 	else cInBuf[iIndex++]=cChar;
			 	lStartTime=GetTimeTicks();  /* refresh data timeout */
		 	}
		 	if((GetTimeTicks()-lStartTime)>=lTimeout){
				cInBuf[iIndex]=0; /* Add the zero end to the data. */
			 	return -1;  /* receive data timeout */
		 	}
		 	RefreshWDT();
	 	}
 	}
 	else return 0;
}
int Receive_Data_Length(int iPort,unsigned char* cInBuf,int iLength,long lTimeout){
	/*
	 Uses COM port to receive string (fixed data length).

	 iPort:    COM port number to receive data.
			   0:COM0, 1:COM1, 2:COM2  .....
	 *cInBuf:  Input buffer to receive data.
	 iLength:  how many bytes to receive?
	 lTimeout: timeout to receive data. (Unit: ms)
			   The timeout is measured from last received byte
			   to receive whole data.
	 return: >0 :length of received data
			  0 :doen't receive any data
			 -1 :timeout
	*/
	unsigned char cChar;
	int iIndex=0;
 	unsigned long lStartTime;
 	if(IsCom(iPort)){
		lStartTime=GetTimeTicks();
	 	for(;;){
		 	while(IsCom(iPort)){//check COM port
			 	cInBuf[iIndex++]=ReadCom(iPort);
			 	if(iIndex>=iLength){
					cInBuf[iIndex]=0;
				 	return iIndex;     /* return data length */
			 	}
				lStartTime=GetTimeTicks();  /* refresh data timeout */
		 	}
		 	if((GetTimeTicks()-lStartTime)>=lTimeout) return -1;  /* receive data timeout */
			RefreshWDT();
	 	}
 	}
 	else return 0;
}
void inwPrint(char *s,...){

	return;
}
int readModbusRTU(){
	int ret = 0,i=0;
	char inbuf[256];
	for(i=0;i<256;++i)inbuf[i]=0x00; //init buf
	i=0;
	while(Kbhit()){
		inbuf[i]=Getch();
		ret = ascii_to_hex(inbuf[i]);
		Show5DigitLed(1,12);
		leds(ret);
		i++;
	}
	ret = i;
	return ret;
}
