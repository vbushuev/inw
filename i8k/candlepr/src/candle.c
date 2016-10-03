#include <stdio.h>
#include <stdarg.h>
#include "funcs.h"
extern int gRegisters[128];
extern int iCircleTime;
static int diStopbits[] = {
    H_01,//A1
    H_02,//A2
    H_03,//A3
    H_04,//A4
    H_04,//A5
    H_05,//A6
    H_07,//A7
    H_08,//A8
    H_12,//A9
    H_13,//A10
    H_13,//A11
    H_14,//A12
    H_16,//A13
    H_17,//A14
    H_01|H_02,//A15
    H_00,//A16
    H_00,//A17
    H_00,//A18
    H_00,//A19
    H_00,//A20
    H_00,//A21
    H_00,//A22
    H_00,//A23
    H_00,//A24
    H_00,//A25
    H_00,//A26
    H_00,//A27
    H_00,//A28
    H_00,//A29
    H_00,//A30
    H_00,//A31
    H_00//A32
};
int makeBitArray(dword d,int*a){
    int i =0;
    int r[32];
    for(i=0; i<32; i++){
        r[i]=d & 1;
        d>>=1;
    }
    a = &r;
    return 0;
}
int doCommand(sStep sstep){
    //dword do_, dword di_,int timer, int sstep.wait.value,dword finish
    int ret = 0, docirle = 1, i = 0;
    int *p;
    int bdo[32];
    dword ict = 0, iTimeout =0 , curTime = 0, curTimeout = 0,di_data = 0, do_data = 0;
    do_data = sstep.command;
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

    ret = readSignals(&di_data);
    if(ret) return ret;
    ret =check (di_data, sstep.command,sstep.wait);
    if(ret) return ret;
    if((sstep.command&H_06)&&sstep.wait.type==2){clearEncoder(0);} // нижний поршень в верхнем положении левый(0)
    if((sstep.command&H_15)&&sstep.wait.type==2){clearEncoder(1);} // нижний поршень в верхнем положении right
<<<<<<< HEAD
    DelayMs(16);
=======
>>>>>>> origin/rev-00
    sendCommand(do_data);
    iTimeout = TimerReadValue();
    while(docirle){
        readSignals(&di_data);
        // if sensors A6 & A12 - clearEncoder
        if((di_data&H_06))clearEncoder(0); // нижний поршень в верхнем положении левый(0)
        if((di_data&H_12))clearEncoder(1); // нижний поршень в верхнем положении right
        makeBitArray(di_data,p);
        for(i=0;i<32;i++){
            if(p[i]==1 && (do_data&diStopbits[i])) do_data^=diStopbits[i];
        }
        if(do_data!=sstep.command)sendCommand(do_data);
        if(sstep.wait.type==0 && (di_data&sstep.wait.value)){
            docirle = 0;
        }
        else if(sstep.wait.type == 1){
            curTimeout = TimerReadValue()-iTimeout;
            if( curTimeout >= sstep.wait.value) docirle = 0;
        }
        else if(sstep.wait.type == 2){
            long lenc = 0,renc = 0;
            if(gRegisters[0x30]==0){
                readEncoder(0,&lenc);
                if(((sstep.wait.value-gRegisters[49])<=lenc)&&(lenc<=(sstep.wait.value+gRegisters[49])))docirle = 0;
            }
            else{
                readEncoder(1,&renc);
                if(((sstep.wait.value-gRegisters[49])<=renc)&&(renc<=(sstep.wait.value+gRegisters[49])))docirle = 0;
            }
        }
        curTime = TimerReadValue()-iTimeout;
        if( curTime >= gRegisters[0x25]){
            ret = ERROR_ALARM_DELAY;
            docirle = 0;
        }
        //DelayMs(8);
    }
    sendCommand(sstep.finish);
    return ret;
}
