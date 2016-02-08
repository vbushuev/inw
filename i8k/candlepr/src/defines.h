
#define ECSLOT 0
#define DISLOT 1
#define DOSLOT 2

#define ALARM_DELAY 1500

#define ERROR_ALARM_DELAY -1

#define EEPROM_RUNTIME 0
#define EEPROM_TOTAL 1

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
    byte tV10;byte tV19;
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
#pragma pack(pop)
