#include "funcs.h"

int exception(int e){
	Print("Error occurs %i",e);
	return 0;
}
int sendCommand(unsigned long command){
	//DO_32(DOSLOT,command);
	return 0;
}
int readSignals(unsigned long *data){
	unsigned long di;
	di = DI_32(DISLOT);
	memcpy(data,&di,4);
	Print("DI data = %-08lX\n\r",data);
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
    Print("[%i]=%010ld\r\n",channel,data);
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
		Print("Initiate 8080 on slot%d error!\n\r",ECSLOT);
		Print("  Cannot find 8080.\r\n");
	}
	else{
		Print("Initiate 8080 on slot%d ok.\n\r",ECSLOT);
        if(iRet>0){
            Print("  Some Pulse/Dir channels have one count offset.\n\r");
            Print("  Return code:%02X\n\r",iRet);
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
    return 0;
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
