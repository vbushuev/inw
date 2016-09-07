#ifndef _INW_H_
#define _IWN_H_
#include "errors.h"

#define ECSLOT 0
#define DISLOT 1
#define DOSLOT 2
#define COMPORT 1
#define COMPORT_BUFFER_LENGTH 256
#define TOTAL_TIMEOUT 5000

#define ALARM_DELAY 1500



#define EEPROM_RUNTIME 0
#define EEPROM_TOTAL 1
#define EEPROM_REGISTERS 2

#define H_00 0x00000000
#define H_01 0x00000001
#define H_02 0x00000002
#define H_03 0x00000004
#define H_04 0x00000008
#define H_05 0x00000010
#define H_06 0x00000020
#define H_07 0x00000040
#define H_08 0x00000080
#define H_09 0x00000100
#define H_10 0x00000200
#define H_11 0x00000400
#define H_12 0x00000800
#define H_13 0x00001000
#define H_14 0x00002000
#define H_15 0x00004000
#define H_16 0x00008000
#define H_17 0x00010000
#define H_18 0x00020000
#define H_19 0x00040000
#define H_20 0x00080000

typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;


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
    byte need;
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
