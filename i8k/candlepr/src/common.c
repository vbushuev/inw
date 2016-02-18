#include <stdio.h>
#include <stdarg.h>
#include "funcs.h"
extern int gRegisters[128];
int exception(int e){
	inwPrint("Error occurs %i\r\n",e);
	Show5DigitLed(1,14);
    leds(e);
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
int log(byte*msg,...){
	//число параметров это функции переменно, концом списк служит значение 0
	int i=0,r=0;
	char str[1024];
	va_list ap; //тип для хранения информации, передаваемой в va_start, va_arg, va_end
	byte* arg; //очередной целочисленный аргумент
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
}
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
