#ifndef _FUNCS_H_
#define _FUNCS_H_
#include <8000e.h>
#include <8080.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"

void inwPrint(char *s,...);
int initInw(psRuntimeValues prtv, psTotalValues ptv);
void deinitInw();

int exception(int e);
/*****
 * I\O
 *****/
int sendCommand(unsigned long command);
int readSignals(unsigned long *data);
int readEncoderL(long *up,long *down);
int readEncoderR(long *up,long *down);
int Receive_Data(unsigned char* cInBuf,char cTerminator,long lTimeout);
int Receive_Data_Length(unsigned char* cInBuf,int iLength,long lTimeout);
//int readEncoder(int channel,long *data);
// EEPROM
int getRuntime(psRuntimeValues prtv);
int setRuntime(sRuntimeValues rtv);
int getTotal(psTotalValues ptv);
int setTotal(sTotalValues tv);

int loadMainScenario(sRuntimeValues rtv, sStep * sc);
int loadStartScenario(sRuntimeValues rtv, sStep * sc);
int loadInitScenario(sRuntimeValues rtv, sStep * sc);

int readModbus();
int str_hex_to_ascii(byte* in,int len,byte* out);
void leds(int s);
void ledstr(char *str,int len);
void ledsOff();
    // Uses COM port to receive data with a terminative char.
    // If time between two byte grater than lTimeout, abort receiving.

#endif
