#include <stdio.h>
#include <stdarg.h>
#include "funcs.h"
extern int gRegisters[128];
int diStopbits[] = {
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
int operation(sStep stp){
    int ret = 0, iTimeout = 0;
    dword di_data = 0;
    long enc = 0;
    //readSignalss
    ret = readSignals(&di_data);
    if(ret) return ret;
    ret =check (di_data, stp.command,stp.wait);
    if(ret) return ret;
    // if sensors A6 & A12 - clearEncoder
    if(compare_bit(di_data,H_06))clearEncoder(0); // нижний поршень в верхнем положении левый(0)
    if(compare_bit(di_data,H_12))clearEncoder(1); // нижний поршень в верхнем положении левый(0)
    //send
    sendCommand(stp.command);
    iTimeout = TimerReadValue();
    while((TimerReadValue()-iTimeout) <= 60000){
        if(stp.wait.type==2){
            ret = Encoder((gRegisters[0x30]==0)?0:1,&enc);
            if(ret)break;
            if( (compare_bit(stp.comand,H_14)||compare_bit(stp.comand,H_05)){
                if(stp.wait.value >= enc)break;
            }
            elseif(stp.wait.value <= enc)break;
        }
        else if(stp.wait.type==1){
            if( (TimerReadValue()-iTimeout)>= stp.wait.value) break;
        }
        else if(stp.wait.type==0){
            ret = readSignals(&di_data);
            if(compare_bit(di_data,H_01))break;
            if((){
                ret = ERROR_ALARM_DELAY;
                break;
            }
        }
    }
    sendCommand(stp.finish);
    return ret;
}
int makeBitArray(dword d,int*a){
    int i =0;
    int r[32];
    for(i=0; i<32; i++){
        r[i]=d & 1;
	    d>>=1;
    }
    memcpy(a,&d,32);
    return 0;
}
int doCommand(sStep sstep){
    //dword do_, dword di_,int timer, int encoder,dword finish
    int ret = 0, iTimeout = 0, int wait = 0, int iwaitTimeout;
    dword di_data = 0, do_data = sstep.command;
    long lenc = 0,renc = 0;
    int bdo[32];
    // checks
    if(sstep.wait.type == 3 ){
        ict = TimerReadValue() - iCircleTime;
        iCircleTime = TimerReadValue();
        sstage=3;
        gRegisters[0x0c]++;
        gRegisters[0x0e] = ict;
        // счетчик пробега
        gRegisters[0x10]++;
        gRegisters[0x12] += ict;
        gRegisters[0x30] = 1;
        return 0;
    }
    else if(sstep.wait.type == 4 ){
        int ict = 0;
        ict = TimerReadValue() - iCircleTime;
        iCircleTime = TimerReadValue();
        sstage=3;
        gRegisters[0x0d]++;
        gRegisters[0x0f] = ict;
        if(gRegisters[0x0d]>=gRegisters[0x08]){ // пока делаем одинаковое кол-во циклов для левой и правой
            finished = 1;
            todo = 0;
            gRegisters[0x20]=0;
        }
        // счетчик пробега
        gRegisters[0x11]++;
        gRegisters[0x13] += ict;
        gRegisters[0x30] = 0;
        return 0;
    }
    else if(currentScenario[sstep].wait.type==5){
        gRegisters[0x20]=0;
        return 0;
    }
    iwaitTimeout = gRegisters[0x25];
    //readSignalss
    ret = readSignals(&di_data);
    if(ret) return ret;
    ret =check (di_data, do_data,wait);
    if(ret) return ret;
    // if sensors A6 & A12 - clearEncoder
    if(compare_bit(di_data,H_06))clearEncoder(0); // нижний поршень в верхнем положении левый(0)
    if(compare_bit(di_data,H_12))clearEncoder(1); // нижний поршень в верхнем положении right
    sendCommand(do_data);
    iTimeout = TimerReadValue();
    while( (TimerReadValue()-iTimeout) <= iwaitTimeout ){
        int bdi[32];
        ret = readSignals(&di_data);
        makeBitArray(di_data,&bdi);
        for(int i=0;i<32;i++){
            if(bdi[i]==1) do_data^=bdi[i];
        }
        sendCommand(do_data);
        if(sstep.wait.type==0 && compare_bit(di_data,di_)){
            break;
        }
        else if(sstep.wait.type == 1){
            if(( TimerReadValue()-iTimeout) >= sstep.wait.value) break;
        }
        else if(sstep.wait.type == 2){
            ret = Encoder(1,&renc);
            ret = Encoder(0,&lenc);
            if( compare_bit(do_,H_05) ) {
                if(encoder>=lenc)break;
            }
            else if (compare_bit(do_,H_06)){
                if(encoder<=lenc)break;
            }
            else if (compare_bit(do_,H_14)){
                if(encoder>=renc)break;
            }
            else if (compare_bit(do_,H_15)){
                if(encoder<=renc)break;
            }
        }
    }
    sendCommand(sstep.finish);
    return ret;
}