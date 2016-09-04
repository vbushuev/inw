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
    gRegisters[0x23] = 0;
    gRegisters[0x24] = 0;
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
int compare_bit(unsigned long d1,unsigned long d2){
    unsigned long d = d1&d2;
    if(d == d2) return 1;
    return 0;
}
int check(unsigned long di_d,unsigned long do_d, unsigned long do_to){
    int r = 0;
    if( compare_bit(do_d,H_01) && compare_bit(do_to,H_01) ){// hobot moving left
        if ( !compare_bit(di_d,H_03)
            || compare_bit(di_d,H_06)
            || !compare_bit(di_d,H_07)
        ) return ERROR_WRONG_COMAND;
    }
    if( compare_bit(do_d,H_02) && compare_bit(do_to,H_02) ){// hobot moving right
        if ( !compare_bit(di_d,H_09)
            || compare_bit(di_d,H_12)
            || !compare_bit(di_d,H_14)
        ) return ERROR_WRONG_COMAND;
    }
    if( compare_bit(do_d,H_04) && (compare_bit(do_to,H_04)||compare_bit(do_to,H_05)) ){// up left press down
        if ( !(compare_bit(di_d,H_15) || compare_bit(di_d,H_02))
            || compare_bit(di_d,H_06)
            || !compare_bit(di_d,H_07)
        ) return ERROR_WRONG_COMAND;
    }
    if( compare_bit(do_d,H_13) && (compare_bit(do_to,H_10)||compare_bit(do_to,H_11)) ){// up right press down
        if ( !(compare_bit(di_d,H_01) || compare_bit(di_d,H_15))
            || compare_bit(di_d,H_12)
            || !compare_bit(di_d,H_14)
        ) return ERROR_WRONG_COMAND;
    }
    if( compare_bit(do_d,H_05) && compare_bit(do_to,H_06) ){// left down press - up
        if ( !(compare_bit(di_d,H_15) || compare_bit(di_d,H_02))
            || !compare_bit(di_d,H_03)
        ) return ERROR_WRONG_COMAND;
    }
    if( compare_bit(do_d,H_14) && compare_bit(do_to,H_12) ){// right down press - up
        if ( !(compare_bit(di_d,H_15) || compare_bit(di_d,H_01))
            || !compare_bit(di_d,H_09)
        ) return ERROR_WRONG_COMAND;
    }
    if( compare_bit(do_d,H_08)){// right down press - up
        if ( !(compare_bit(di_d,H_15) || compare_bit(di_d,H_02))
            || !compare_bit(di_d,H_03)
        ) return ERROR_WRONG_COMAND;
    }
    if( compare_bit(do_d,H_16)){// right down press - up
        if ( !(compare_bit(di_d,H_15) || compare_bit(di_d,H_01))
            || !compare_bit(di_d,H_09)
        ) return ERROR_WRONG_COMAND;
    }
    if( compare_bit(do_d,H_10)){// right down press - up
        if ( !(compare_bit(di_d,H_15) || compare_bit(di_d,H_02))
            || !compare_bit(di_d,H_03)
        ) return ERROR_WRONG_COMAND;
    }
    if( compare_bit(do_d,H_19)){// right down press - up
        if ( !(compare_bit(di_d,H_15) || compare_bit(di_d,H_01))
            || !compare_bit(di_d,H_09)
        ) return ERROR_WRONG_COMAND;
    }
    return r;
}
