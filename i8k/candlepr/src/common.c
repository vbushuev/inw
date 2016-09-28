#include <stdio.h>
#include <stdarg.h>
#include "funcs.h"
extern int gRegisters[128];
int exception(int e){
	e = -e;
	gRegisters[0x1f] = e;
	gRegisters[0x20] = 0x80;
	return 0;
}
int showError(){
	int e = gRegisters[0x1f];
	if(e!=0){
		/*sShow5DigitLed(1,16);
	    Show5DigitLed(2,16);
	    Show5DigitLed(3,16);
	    Show5DigitLed(4,16);
	    Show5DigitLed(5,16);
		gRegisters[0x27] = 5;
	}else{
		/* Screen ctrl
		 * 0 - Start screen
		 * 1 - Splash
		 * 2 - Left piston
		 * 3 - Right piston
		 * 4 - Global settings
		 * 5 - Main
		 * 6 - Settings
		 * 7 - Loading
		 * 8 - Alarm
		 */
		gRegisters[0x27] = 8;
    	//ledn(14,e);
	}
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
	int i;
	for(i = 0; i < len; i++) {
		LRC += buf[i];
	}
	LRC=-LRC;
	return LRC;
}
int getRuntime(psRuntimeValues prtv){
	byte buf[32];
	int i=0;
    EE_MultiRead(EEPROM_RUNTIME,0,32,(byte*)buf);
	/*int tV10;int tV19;
	int LC;int Lh;int Lr;
	int RC;int Rh;int Rr;
	int L;
	int R;
	int tstart;
	int ttaker;
	int CL;
	int CR;
	int TL;
	int TR;*/
	prtv->tV10= buf[i++]*256+buf[i++];
	prtv->tV19= buf[i++]*256+buf[i++];
	prtv->LC= buf[i++]*256+buf[i++];
	prtv->Lh= buf[i++]*256+buf[i++];
	prtv->Lr= buf[i++]*256+buf[i++];
	prtv->RC= buf[i++]*256+buf[i++];
	prtv->Rh= buf[i++]*256+buf[i++];
	prtv->Rr= buf[i++]*256+buf[i++];
	prtv->L= buf[i++]*256+buf[i++];
	prtv->R= buf[i++]*256+buf[i++];
	prtv->tstart= buf[i++]*256+buf[i++];
	prtv->ttaker= buf[i++]*256+buf[i++];
	prtv->CL= buf[i++]*256+buf[i++];
	prtv->CR= buf[i++]*256+buf[i++];
	prtv->TL= buf[i++]*256+buf[i++];
	prtv->TR= buf[i++]*256+buf[i++];
    return 0;
}
int setRuntime(sRuntimeValues rtv){
	int r=0;
	EE_WriteEnable();
    r=EE_MultiWrite(EEPROM_RUNTIME,0,sizeof(sRuntimeValues),(psRuntimeValues)&rtv);
    EE_WriteProtect();
    return (r==-1)?ERROR_EEPROM_WRITE:0;
}
int getTotal(psTotalValues ptv){
	int r=0;
    r=EE_MultiRead(EEPROM_TOTAL,0,sizeof(sTotalValues),(psTotalValues)ptv);
    return (r==-1)?ERROR_EEPROM_WRITE:0;
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
	Show5DigitLed(1,15);
	Show5DigitLed(2,13);
	Show5DigitLed(3,10);
	Show5DigitLed(4,13);
	Show5DigitLed(5,10);
	DelayMs(800);

	Show5DigitLed(1,len%16);

	Show5DigitLed(1,ascii_to_hex((int)str[5]));
	Show5DigitLed(2,ascii_to_hex((int)str[6]));
	Show5DigitLed(3,ascii_to_hex((int)str[7]));
	Show5DigitLed(4,ascii_to_hex((int)str[8]));
	Show5DigitLed(5,ascii_to_hex((int)str[9]));
}

void ledn(int n,unsigned int s){
	int p1,p2,p3,p4;
	//if(gRegisters[0x1f]!=0) return;
	Show5DigitLedWithDot(1,n);
	p1 = s/4096;
	p2 = (s-4096*p1)/256;
	p3 = (s-4096*p1-p2*256)/16;
	p4 = (s-4096*p1-p2*256-p3*16);
	Show5DigitLed(2,p1);
	Show5DigitLed(3,p2);
	Show5DigitLed(4,p3);
	Show5DigitLed(5,p4);
	//DelayMs(800);
}
void leds(int s){
	int p1,p2,p3,p4;
	if(s<0){
		Show5DigitLed(2,17);
		s=-s;
	}
		p1 = s/4096;
		p2 = (s-4096*p1)/256;
		p3 = (s-4096*p1-p2*256)/16;
		p4 = (s-4096*p1-p2*256-p3*16);
		Show5DigitLed(2,p1);
		Show5DigitLed(3,p2);
		Show5DigitLed(4,p3);
		Show5DigitLed(5,p4);
	DelayMs(800);
}

void leds2(int s){
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
int str_hex_to_ascii(byte* in,int len,byte* out){
	byte s[256];
	int i=0;
	memset(s,0,256);
	for(i=0;i<len;i++){
		s[2*i]=hex_to_ascii[in[i]/16];
		s[2*i+1]=hex_to_ascii[in[i]%16];
	}
	memcpy(out,s,2*len);
	return 2*len;
}
int to_bytes(byte *s,int v){
	byte c[2];
	c[0]=v/256;
	c[1]=v%256;
	memcpy(s,c,2);
	return 0;
}
int to_bytes_i(byte *s,int v){
	byte c[2];
	c[1]=v/256;
	c[0]=v%256;
	memcpy(s,c,2);
	return 0;
}
int getRegisters(){
	int r=0;
    r=EE_MultiRead_L(0,256,(int*)gRegisters);
    return (r==-1)?ERROR_EEPROM_WRITE:0;
}
int setRegisters(){
	int r=0;
    EE_WriteEnable();
    r=EE_MultiWrite_L(0,256,(int*)gRegisters);
    EE_WriteProtect();
    return (r==-1)?ERROR_EEPROM_WRITE:0;
}
/*int log(byte*msg,...){
	//число параметров это функции переменно, концом списк служит значение 0
	int i=0,r=0;
	char str[1024];
	va_list ap; //тип для хранения информации, передаваемой в va_start, va_arg, va_end
	byte far * arg; //очередной целочисленный аргумент
	va_start(ap, msg); //инициализирует список параметров
	memset(str,0,sizeof(str));
	i+=strlen(msg);
	memcpy(str,msg,i);
	while ((arg = va_arg(ap,int)) != 0) { //извлекает следующий параметр типа int
		memcpy(str+i,(byte*)arg,strlen(arg));
		i+=strlen(arg);
	}
	va_end(ap); //очищаем и закрываем список параметров
	EE_WriteEnable();
	r=EE_MultiWrite_L(256, 1024, str);
	EE_WriteProtect();
	return (r==-1)?ERROR_EEPROM_WRITE:0;
}*/
int logCOM(byte*in,int il,byte*out,int ol){
	int i=0,r=0;
	char str[1024];
	memset(str,0,sizeof(str));
	memcpy(str+i,"READ[",5); i+=5;
	memcpy(str+i,in,il);i+=il;
	memcpy(str+i,"]WRITE[",7); i+=7;
	memcpy(str+i,out,ol);i+=ol;
	memcpy(str+i,"]",1); i+=1;
	EE_WriteEnable();
	r=EE_MultiWrite_L(256, 1024, str);
	EE_WriteProtect();
	return (r==-1)?ERROR_EEPROM_WRITE:0;
}
unsigned int CRC16 (const BYTE *nData, WORD wLength){
	static const int wCRCTable[] = {
		0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
   		0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
   		0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
	    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
	    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
	    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
	    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
	    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
	    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
	    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
	    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
	    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
	    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
	    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
	    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
	    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
	    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
	    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
	    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
	    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
	    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
	    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
	    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
	    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
	    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
	    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
	    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
	    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
	    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
	    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
	    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
	    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };
	byte nTemp;
	unsigned int wCRCWord = 0xFFFF;
	while (wLength--){
		nTemp = *nData++ ^ wCRCWord;
      	wCRCWord >>= 8;
      	wCRCWord  ^= wCRCTable[nTemp];
   	}
   	return wCRCWord;
} // End: CRC16
