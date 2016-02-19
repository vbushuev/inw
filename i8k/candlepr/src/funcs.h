#ifndef _FUNCS_H_
#define _FUNCS_H_
#include <8000e.h>
#include <8080.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"

void inwPrint(char *s,...);
int initInw();
void deinitInw();
/*****
 * Common
 *****/
int log(byte*msg,...);
int logCOM(byte*in,int il,byte*out,int ol);
int exception(int e);
int to_bytes(byte *s,int v);
/*****
 * I\O
 *****/
int sendCommand(unsigned long command);
int readSignals(unsigned long *data);
int readEncoderL(long *up,long *down);
int readEncoderR(long *up,long *down);
int Receive_Data(unsigned char* cInBuf,char cTerminator,long lTimeout);
int Receive_Data_Length(unsigned char* cInBuf,int iLength,long lTimeout);
/*
    Seeks the iTh(th) cMark from the start position
    to the EOF(end of file).

    cMark: find the position next to the cMark.
    lStart: the start position to seek the cMark.
            (0: start position of the file)
    iTh: seek the iTh(th) cMark.

    Return value:
        0: can't find the cMark
       >0: the offset position next to the cMark
 */
unsigned long FSeek(FILE_DATA far *file_pointer,char cMark,unsigned long lStart,int iTh);
/*
    Gets integer of sKeyName, the sKeyName in text file must be after one '*'.
    If can't find the sKeyName, return the lDefault.
    If can't find the value of the sKeyName, return the lDefault.
 */
long GetProFileInt(FILE_DATA far *file_pointer,char* sKeyName,long lDefault);
/*
    Gets float of sKeyName, the sKeyName in text file must be after one '*'.
    If can't find the sKeyName, return the fDefault.
    If can't find the value of the sKeyName, return the fDefault.
 */
float GetProFileFloat(FILE_DATA far *file_pointer,char* sKeyName,float fDefault);
/*
    Gets string of sKeyName, the sKeyName in text file must be after one '*'.
    If can't find the sKeyName, sResult=sDefault.
    If can't find the string of the sKeyName, sResult=sDefault.
    Return value: length of sResult

    I use GetProFileStr to replace GetProFileVal.
    float GetProFileVal(FILE_DATA far *file_pointer,char* sKeyName,float fDefault);
    Gets float value of sKeyName, the sKeyName in text file must be after one '*'.
    If can't find the sKeyName, return the fDefault.
    If can't find the value of the sKeyName, return the fDefault.
 */
int GetProFileStr(FILE_DATA far *file_pointer,char* sKeyName,char* sResult,char* sDefault);


//int readEncoder(int channel,long *data);
// EEPROM
int getRuntime(psRuntimeValues prtv);
int setRuntime(sRuntimeValues rtv);
int getTotal(psTotalValues ptv);
int setTotal(sTotalValues tv);
int getRegisters();
int setRegisters();

// scenario
int loadMainScenario( sStep * sc);
int loadStartScenario( sStep * sc);
int loadInitScenario( sStep * sc);

int readModbus();
int str_hex_to_ascii(byte* in,int len,byte* out);
void leds(int s);
void ledstr(char *str,int len);
void ledsOff();
    // Uses COM port to receive data with a terminative char.
    // If time between two byte grater than lTimeout, abort receiving.

#endif
