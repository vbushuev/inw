#include "funcs.h"
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
