#include "funcs.h"
extern unsigned int gRegisters[128];
/*
 * Initialize inw
 */
int initInw(){
    int iRet=0;
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
	//TimerResetValue();

    EnableWDT();

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
    if(gRegisters[0x25]>8000){
        gRegisters[0x25] = TOTAL_TIMEOUT;
        setRegisters();
    }
    ClearSystemKey();
    return 0;
}
void deinitInw(){
    TimerClose();
    DisableWDT();
    closeComPort();
    setRegisters();
}
int compare_bit(unsigned long d1,unsigned long d2){
    unsigned long d = d1&d2;
    if(d == d2) return 1;
    return 0;
}
int check(unsigned long di_d,unsigned long do_d, sWait do_to){
    if( (do_d&H_01) && (do_to.value&H_01) ){// hobot moving left
        if ( !(di_d&H_03)
            || (di_d&H_06)
            || !(di_d&H_07)
        ) return ERROR_WRONG_COMAND;
    }
    else if( (do_d&H_02) && (do_to.value&H_02) ){// hobot moving right
        if ( !(di_d&H_09)
            || (di_d&H_12)
            || !(di_d&H_14)
        ) return ERROR_WRONG_COMAND;
    }
    else if( (do_d&H_04) && ((do_to.value&H_04)||(do_to.value&H_05)) ){// up left press down
        if ( !((di_d&H_15) || (di_d&H_02))
            || (di_d&H_06)
            || !(di_d&H_07)
        ) return ERROR_WRONG_COMAND;
    }
    else if( (do_d&H_05) && (do_to.value&H_06) && (do_to.type!=2)){// left down press - up
        //if ( !((di_d&H_15)|(di_d&H_02))
        if ( (di_d&H_01)
            || !(di_d&H_03)
        ) return ERROR_WRONG_COMAND;
    }
    else if( (do_d&H_08)){// уват левый
        if ( (di_d&H_01)
            || !(di_d&H_03)
        ) return ERROR_WRONG_COMAND;
    }
    else if( (do_d&H_10)){// cutter left
        if ( !((di_d&H_15)|(di_d&H_02))
            || !(di_d&H_03)
            || (di_d&H_06)
        ) return ERROR_WRONG_COMAND;
    }
    else if( (do_d&H_13) && ((do_to.value&H_10)||(do_to.value&H_11)) ){// up right press down
        if ( !((di_d&H_01) || (di_d&H_15))
            || (di_d&H_12)
            || !(di_d&H_14)
        ) return ERROR_WRONG_COMAND;
    }
    else if( (do_d&H_14) && (do_to.value&H_12)  && (do_to.type!=2)){// right down press - up
        //if ( !( (di_d&H_15)||(di_d&H_01) )
        if ( (di_d&H_02)
            || !(di_d&H_09)
        ) return ERROR_WRONG_COMAND;
    }
    else if( (do_d&H_16)){// ухват правый
        if ( (di_d&H_02)
            || !(di_d&H_09)
        ) return ERROR_WRONG_COMAND;
    }
    else if( (do_d&H_19)){// cutter right
        if ( !((di_d&H_15) || (di_d&H_01))
            || !(di_d&H_09)
            || (di_d&H_12)
        ) return ERROR_WRONG_COMAND;
    }
    return 0;
}
