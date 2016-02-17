#include "funcs.h"
int exception(int e){
	inwPrint("Error occurs %i\r\n",e);
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

void inwPrint(char *s,...){

	return;
}
