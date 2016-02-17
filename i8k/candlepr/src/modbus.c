#include "funcs.h"
extern int gRegisters[128];

sModbusPack parse(byte* in){
	sModbusPack ret;
	ret.colon=':';
	ret.addr=16*ascii_to_hex(in[1]) + ascii_to_hex(in[2]);
	ret.func=16*ascii_to_hex(in[3]) + ascii_to_hex(in[4]);
	ret.reg	=4096*ascii_to_hex(in[5]) + 256*ascii_to_hex(in[6]) + 16*ascii_to_hex(in[7]) + ascii_to_hex(in[8]);
	ret.data=4096*ascii_to_hex(in[9]) + 256*ascii_to_hex(in[10]) + 16*ascii_to_hex(in[11]) + ascii_to_hex(in[12]);
	ret.lrc	=16*ascii_to_hex(in[13]) + ascii_to_hex(in[14]);
	return ret;
}
int serialize(psModbusPack pmb,byte *out){
	int i=0;
    byte ret[COMPORT_BUFFER_LENGTH];
    byte reta[COMPORT_BUFFER_LENGTH];
    byte l;
	memset(ret,0,COMPORT_BUFFER_LENGTH);
	memset(reta,0,COMPORT_BUFFER_LENGTH);
	ret[i++] = pmb->colon;
	ret[i++] = pmb->addr/16;
	ret[i++] = pmb->addr%16;
	ret[i++] = pmb->func/16;
	ret[i++] = pmb->func%16;
	ret[i++] = pmb->response_size/16;
	ret[i++] = pmb->response_size%16;
    memcpy(ret+i,pmb->response,pmb->response_size);
    i+=pmb->response_size;
    //i+=str_hex_to_ascii(test,10,ret+i);
    //pmb->response_size=10;
    l=lrc(ret,i);
    pmb->lrc=l;
    i=str_hex_to_ascii(ret+7,i-7,reta+7);
    i+=7;
    reta[0]=':';
    reta[1]=hex_to_ascii[ret[1]];
    reta[2]=hex_to_ascii[ret[2]];
    reta[3]=hex_to_ascii[ret[3]];
    reta[4]=hex_to_ascii[ret[4]];
    reta[5]=hex_to_ascii[ret[5]];
    reta[6]=hex_to_ascii[ret[6]];
    reta[i++] = hex_to_ascii[l/16];
	reta[i++] = hex_to_ascii[l%16];

	reta[i++] = '\r';
	reta[i++] = '\n';
    memset(out,0,COMPORT_BUFFER_LENGTH);
    memcpy(out,reta,i);
    return i;
}
/*int checkLRC(psModbusPack pmb){
	int lrc;
	sModbusPack ch;
	byte c[18];
	memcpy(&ch,pmb,sizeof(sModbusPack));
	serialize(&ch,c);
	lrc = 16*ascii_to_hex(c[13]) + ascii_to_hex(c[14]);
	Print(c);
	Show5DigitLed(1,8);
	if(lrc==pmb->lrc){
		 leds(0xffff);
	}else{
		leds(lrc);
		leds(pmb->lrc);
	}
	return (lrc==pmb->lrc)?0:1;
}*/
int analyzeModBus(psModbusPack pmb,byte* outbuf){
	int ret =0, sl = 0, i=0;
    byte o[COMPORT_BUFFER_LENGTH];
    byte data[COMPORT_BUFFER_LENGTH];
    int registers[128];
    getRegisters(registers);
    memset(data,0,COMPORT_BUFFER_LENGTH);
    memset(o,0,COMPORT_BUFFER_LENGTH);
	switch(pmb->func){
		case 0x03:{ // read coil registers
            //EE_MultiRead(EEPROM_RUNTIME,pmb->reg,2*pmb->data,(byte*)(data));
            for(i=0;i<pmb->data;i++){
                byte bt[2];
                to_bytes(bt,gRegisters[pmb->reg+i]);
                memcpy(pmb->response+(2*i),bt,2);
            }
			pmb->response_size=2*pmb->data;
            sl=serialize(pmb,o);
            memcpy(outbuf,o,sl);
		}break;
		case 0x06:{ // write single register
            gRegisters[pmb->reg] = pmb->data;
            setRegisters(gRegisters);
		}break;
	}
    /*Show5DigitLed(1,10); leds(pmb->addr);//addr
	Show5DigitLed(1,15); leds(pmb->func);
	Show5DigitLed(1,13); leds(pmb->reg);
	Show5DigitLed(1,17); leds(pmb->data);*/
	return ret;
}
int readModbus(){
	int ret=0, i=0;
	byte inbuf[COMPORT_BUFFER_LENGTH],outbuf[COMPORT_BUFFER_LENGTH],response[COMPORT_BUFFER_LENGTH];//=":010311112222333344445555";
    byte *pbuf;
	sModbusPack mb;
	ret = Receive_Data(inbuf,0xa,TOTAL_TIMEOUT);
    if(ret<=0) return 0;
    pbuf =inbuf;
	while(*pbuf!=':'){
        pbuf++;
        i++;
        if(i>=ret)return 0;
    }
    ret = ret -i;
    memset(outbuf,0,COMPORT_BUFFER_LENGTH);
    memcpy(outbuf,pbuf,ret);
    outbuf[ret]='\r';
    outbuf[ret+1]='\n';
    mb=parse(pbuf);
    analyzeModBus(&mb,outbuf);
	//serialize(&mb,outbuf);
	ToComStr(COMPORT,outbuf);
	//ToComStr(COMPORT,":010311112222333344445555F2\r\n");
    //log2EEPROM(inbuf,outbuf,mb.response_size+8);
    Show5DigitLed(1,17);
    leds(outbuf[2]*256+outbuf[4]);
    //Show5DigitLed(1,14); leds(mb.response_size+8);

	Show5DigitLed(1,12);
	leds(mb.lrc);

    return ret;
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
