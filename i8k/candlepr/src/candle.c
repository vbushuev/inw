#include <stdio.h>
#include <stdarg.h>
#include "funcs.h"
extern int gRegisters[128];
extern int iCircleTime;
static dword diStopbits[] = {
    H_01,//A1
    H_02,//A2
    H_03,//A3
    H_00,//A4
    H_04,//A5
    H_05,//A6
    H_07,//A7
    H_08,//A8
    H_12,//A9
    H_00,//A10
    H_13,//A11
    H_14,//A12
    H_16,//A13
    H_17,//A14
    H_00, //A15
    H_00 //A16
};
int doCommand(sStep sstep){
    //dword do_, dword di_,int timer, int sstep.wait.value,dword finish
    int ret = 0, docirle = 1, i = 0;
    dword ict = 0, iTimeout =0 , curTime = 0, di_data = 0, do_data = 0, d=0;
    long enc = 0, enc_s = 0;

    // checks
    if(sstep.wait.type == 3 ){
        ict = TimerReadValue() - iCircleTime;
        TimerResetValue();
        iCircleTime = TimerReadValue();
        gRegisters[0x0c]++;
        gRegisters[0x0e] = ict;
        // счетчик пробега
        gRegisters[0x10]++;
        gRegisters[0x12] += ict;
        gRegisters[0x30] = 1;
        return 0;
    }
    else if(sstep.wait.type == 4 ){
        ict = TimerReadValue() - iCircleTime;
        TimerResetValue();
        iCircleTime = TimerReadValue();
        gRegisters[0x0d]++;
        gRegisters[0x0f] = ict;
        if(gRegisters[0x0d]>=gRegisters[0x08]){ // пока делаем одинаковое кол-во циклов для левой и правой
            gRegisters[0x20]=0x80;
        }
        // счетчик пробега
        gRegisters[0x11]++;
        gRegisters[0x13] += ict;
        gRegisters[0x30] = 0;
        return 0;
    }
    else if(sstep.wait.type==5){
        gRegisters[0x20]=0x80;
        return 0;
    }

    //readSignalss

    di_data = readSignals2();
    ret =check (di_data, sstep.command,sstep.wait);
    if(ret) return ret;
    //if((sstep.command&H_06)&&sstep.wait.type==2){clearEncoder(0);} // нижний поршень в верхнем положении левый(0)
    //if((sstep.command&H_15)&&sstep.wait.type==2){clearEncoder(1);} // нижний поршень в верхнем положении right
    //DelayMs(16);
    do_data = sstep.command;
    sendCommand(do_data);
    DelayMs(16);
    if(sstep.wait.type == 2){
        if((sstep.command&H_06)||(sstep.command&H_05))enc_s = readEncoder2(0);
        else enc_s = readEncoder2(1);
    }
    iTimeout = TimerReadValue();
    while(docirle){
        di_data = readSignals2();
        // if sensors A6 & A12 - clearEncoder
        if((di_data&H_06)){clearEncoder(0);enc_s=0;enc=0;} // нижний поршень в верхнем положении левый(0)
        if((di_data&H_12)){clearEncoder(1);enc_s=0;enc=0;} // нижний поршень в верхнем положении right
        d = di_data;
        for(i=0; i<sizeof(diStopbits); i++){
            if((d & 1) && (do_data&diStopbits[i]) ) do_data^=diStopbits[i];
            d>>=1;
        }
        if(do_data!=sstep.command)sendCommand(do_data);
        curTime = TimerReadValue()-iTimeout;
        if(sstep.wait.type==0 && (di_data&sstep.wait.value)){docirle = 0;}
        else if(sstep.wait.type == 1 && (curTime >= sstep.wait.value) ){docirle = 0;}
        else if(sstep.wait.type == 2){
            if((do_data&H_05) && !(di_data&H_06)){
                enc = readEncoder2(0);
                if( (enc_s - enc) >= sstep.wait.value)docirle=0;
                gRegisters[0x30] = 0;
            }
            else if((do_data&H_06)&& !(di_data&H_06)){
                enc = readEncoder2(0);
                if( (enc - enc_s) >= sstep.wait.value)docirle=0;
                gRegisters[0x30] = 0;
            }
            else if((do_data&H_14)&& !(di_data&H_12)){
                enc = readEncoder2(1);
                if( (enc_s - enc) >= sstep.wait.value)docirle=0;
                gRegisters[0x30] = 1;
            }
            else if((do_data&H_15)&& !(di_data&H_12)){
                enc = readEncoder2(1);
                if( (enc - enc_s) >= sstep.wait.value)docirle=0;
                gRegisters[0x30] = 1;
            }
            /*
            long lenc = 0,renc = 0;
            if(gRegisters[0x30]==0){
                readEncoder(0,&lenc);
                if(((sstep.wait.value-gRegisters[49])<=lenc)&&(lenc<=(sstep.wait.value+gRegisters[49])))docirle = 0;
            }
            else{
                readEncoder(1,&renc);
                //if(((sstep.wait.value-gRegisters[49])<=renc)&&(renc<=(sstep.wait.value+gRegisters[49])))docirle = 0;
            }*/
        }
        if( curTime >= gRegisters[0x25]){
            ret = ERROR_ALARM_DELAY;
            docirle = 0;
        }
        if(!docirle)sendCommand(sstep.finish);
        DelayMs(8);
    }
    //sendCommand(sstep.finish);
    return ret;
}
