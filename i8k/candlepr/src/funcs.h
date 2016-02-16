#ifndef _FUNCS_H_
#define _FUNCS_H_
#include <8000e.h>
#include <8080.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"

void inwPrint(char *s,...);
int initInw(psRuntimeValues prtv, psTotalValues ptv);
int exception(int e);
int sendCommand(unsigned long command);
int readSignals(unsigned long *data);
int readEncoderL(long *up,long *down);
int readEncoderR(long *up,long *down);
//int readEncoder(int channel,long *data);
// EEPROM
int getRuntime(psRuntimeValues prtv);
int setRuntime(sRuntimeValues rtv);
int getTotal(psTotalValues ptv);
int setTotal(sTotalValues tv);

int loadMainScenario(sRuntimeValues rtv, sStep * sc);
int loadStartScenario(sRuntimeValues rtv, sStep * sc);
int loadInitScenario(sRuntimeValues rtv, sStep * sc);
int Receive_Data(int iPort,unsigned char* cInBuf,char cTerminator,long lTimeout);
int readModbus();
void leds(int s);
void ledstr(char *str,int len);
void ledsOff();
    // Uses COM port to receive data with a terminative char.
    // If time between two byte grater than lTimeout, abort receiving.

#endif
