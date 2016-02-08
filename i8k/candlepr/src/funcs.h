#ifndef _FUNCS_H_
#define _FUNCS_H_
#include <8000e.h>
#include <8084W.h>

#define ECSLOT 0
#define DISLOT 1
#define DOSLOT 2

#define ALARM_DELAY 1500

#define ERROR_ALARM_DELAY -1
int initInw();
int exception(int e);
int sendCommand(unsigned long command);
int readSignals(unsigned long *data);
int readEncoderL(long *up,long *down);
int readEncoderR(long *up,long *down);
int readEncoder(int channel,long *data);
#endif
