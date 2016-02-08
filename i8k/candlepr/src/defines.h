
#define ECSLOT 0
#define DISLOT 1
#define DOSLOT 2

#define ALARM_DELAY 1500

#define ERROR_ALARM_DELAY -1

#define EEPROM_RUNTIME 0
#define EEPROM_TOTAL 1

typedef unsigned char  byte;

#pragma pack(push,1)
typedef struct {
    byte tV10;
	byte tV19;
	int LC;
	int Lh;
	int RC;
	int Rh;
	int Lr;
	int Rr;
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
