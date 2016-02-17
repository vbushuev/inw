#ifndef _INW_H_
#define _IWN_H_
#include "errors.h"

#define ECSLOT 0
#define DISLOT 1
#define DOSLOT 2
#define COMPORT 1
#define COMPORT_BUFFER_LENGTH 100
#define TOTAL_TIMEOUT 500

#define ALARM_DELAY 1500



#define EEPROM_RUNTIME 0
#define EEPROM_TOTAL 1
#define EEPROM_REGISTERS 2


typedef unsigned char  byte;

#pragma pack(push,1)
// script struct
/* wait type
 * type:
 *  0 - wait for signal
 *  1 - timer
 *  2 - encoder
 *  3 - произвести замеры счетчиков и таймеров для левого
 *  4 - произвести замеры счетчиков и таймеров для правого
 * value
 */
typedef struct {
    byte type;
    unsigned long value;
} sWait;
/* One step of script struct
 * command - initiat command
 * wait - event signal wait;
 * finish - finish command if ==0 do nothing
 * timer - kill timer for wait
 */
typedef struct {
    unsigned long command;
    sWait wait;
    unsigned long finish;
    unsigned long timer;
} sStep,*psStep;
// runtime values
typedef struct {
    int tV10;int tV19;
	int LC;int Lh;int Lr;
	int RC;int Rh;int Rr;
	int L;
	int R;
	int tstart;
	int ttaker;
	int CL;
	int CR;
	int TL;
	int TR;
} sRuntimeValues, *psRuntimeValues;
// total counters
typedef struct {
    unsigned long CLtotal;
    unsigned long CRtotal;
    unsigned long TLtotal;
    unsigned long TRtotal;
} sTotalValues, *psTotalValues;
// modbus ASCII struct
typedef struct {
    char colon;
    int addr;
    int func;
    unsigned long reg;
    unsigned long data;
    byte response[256];
    int response_size;
    byte lrc;
}sModbusPack, *psModbusPack;
/* Program state
 *
 */
typedef struct {
    byte state;

} sProgramState,*psProgramState;
#pragma pack(pop)
#endif
