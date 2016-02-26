#include "funcs.h"
extern int gRegisters[128];
/*
 * Initialize inw
 */
int initInw(){
    int iRet=0,channel;
    /*
    byte tV10;byte tV19;
   int LC;int Lh;int Lr;
   int RC;int Rh;int Rr;
   int L;int R;int tstart;int ttaker;
   int CL;int CR;int TL;int TR;
     */
    // Always do that to init lib i8000
	InitLib();
    // Init leds
    Init5DigitLed();
    Show5DigitLed(1,16);
    Show5DigitLed(2,16);
    Show5DigitLed(3,16);
    Show5DigitLed(4,16);
    Show5DigitLed(5,16);

    // i8000 has only one systimer
	TimerOpen();
	TimerResetValue();

	//InitEncoder
	iRet=i8080_InitDriver(ECSLOT);
    if (iRet==(-1)) exception(ERROR_ENCODER_NOTFOUND);
	InitEncoder();

	// Init ComPort
    initComPort();
    // Registers from EEPROM
    getRegisters();
    gRegisters[0x21] = 0;
    gRegisters[0x1f] = 0;
    gRegisters[0x27] = 0;
    gRegisters[0x25] = (gRegisters[0x25]*1000<TOTAL_TIMEOUT)?TOTAL_TIMEOUT:gRegisters[0x25];
    ClearSystemKey();
    return 0;
}
void deinitInw(){
    TimerClose();
    closeComPort();
    setRegisters();
}
