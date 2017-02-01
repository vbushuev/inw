#include <stdio.h>
#include <stdarg.h>
#include "funcs.h"
extern unsigned int gRegisters[128];
int doCommand(sStep sstep){
    //dword do_, dword di_,int timer, int sstep.wait.value,dword finish
    int ret = 0,encoderTimeout = 40,i=0;
    dword di_data = 0, do_data = 0, did = 0;//ict = 0
    long  iTimeout =0 , curTime = 0, commonTO=5000;
    //long enc = 0, enc_s = 0;
    dword enc = 0, enc_s = 0;
    dword enc_ls = 0, enc_l = 0;
    dword enc_rs = 0, enc_r = 0;
    encoderTimeout = (gRegisters[0x32]>0&&gRegisters[0x32]<200)?gRegisters[0x32]:encoderTimeout;
    //commonTO = gRegisters[0x25];
    di_data = readSignals2();//Delay(8);
    // checks
    if(sstep.wait.type == 0 && (di_data&sstep.wait.value)){
        return 0;
    }
    if(sstep.wait.type == 3 ){
        //ict = TimerReadValue() - iCircleTime;
        //TimerResetValue();
        //iCircleTime = TimerReadValue();
        gRegisters[0x0c]++;
        //gRegisters[0x0e] = ict;
        // счетчик пробега
        gRegisters[0x10]++;
        //gRegisters[0x12] += ict;
        gRegisters[0x30] = 1;
        return 0;
    }
    else if(sstep.wait.type == 4 ){
        //ict = TimerReadValue() - iCircleTime;
        //TimerResetValue();
        //iCircleTime = TimerReadValue();
        gRegisters[0x0d]++;
        //gRegisters[0x0f] = ict;
        // счетчик пробега
        gRegisters[0x11]++;
        //gRegisters[0x13] += ict;
        gRegisters[0x30] = 0;

        if(gRegisters[0x0d]>=gRegisters[0x08]){ // пока делаем одинаковое кол-во циклов для левой и правой
            gRegisters[0x20]=0x81;
        }
        return 0;
    }
    else if(sstep.wait.type==5){
        gRegisters[0x20]=0x80;
        return 0;
    }

    //readSignalss


    ret = check(di_data, sstep.command,sstep.wait);
    if(ret) return ret;
    //if((sstep.command&H_06)&&sstep.wait.type==2){clearEncoder(0);} // нижний поршень в верхнем положении левый(0)
    //if((sstep.command&H_15)&&sstep.wait.type==2){clearEncoder(1);} // нижний поршень в верхнем положении right

    do_data = sstep.command;

    sendCommand(do_data);
    //Delay(8);
    if((sstep.wait.type == 2) && ((sstep.command&H_05)||(sstep.command&H_14) )){
        encoderTimeout = (gRegisters[0x32]>0&&gRegisters[0x32]<200)?gRegisters[0x32]:encoderTimeout;
        Delay(encoderTimeout);
        //if((sstep.command&H_06)||(sstep.command&H_05))enc_s = readEncoder2(0);
        enc_ls= readEncoder(0);
        enc_rs= readEncoder(1);
        if(sstep.command&H_05) enc_s = readEncoder2(0);
        else if(sstep.command&H_14) enc_s = readEncoder2(1);
    }
    //iTimeout = GetTimeTicks();

    TimerResetValue();
    iTimeout = TimerReadValue();
    for(;;){
        curTime = TimerReadValue();
        if( (curTime-iTimeout) >= commonTO){
            ret = ERROR_ALARM_DELAY;
            break;
        }
        di_data = readSignals2();

        did = di_data;
        for(i=0; i<14; i++){
            if(did & 1) {
                switch(i){
                    case 0:if(do_data&H_01){do_data^=H_01;}break;//A1
                    case 1:if(do_data&H_02){do_data^=H_02;}break;//A2
                    case 2:if(do_data&H_03){do_data^=H_03;}break;//A3
                    case 3:if((do_data&H_04)&&(sstep.wait.value&H_04)){do_data^=H_04;}break;//A4
                    case 4:if(do_data&H_04){do_data^=H_04;}break;//A5
                    case 5:if(do_data&H_05){do_data^=H_05;}break;//A6
                    case 6:if(do_data&H_07){do_data^=H_07;}break;//A7
                    case 7:if(do_data&H_08){do_data^=H_08;}break;//A8
                    case 8:if(do_data&H_12){do_data^=H_12;}break;//A9
                    case 9:if((do_data&H_12)&&(sstep.wait.value&H_10)){do_data^=H_12;}break;//A10
                    case 10:if(do_data&H_13){do_data^=H_13;}break;//A11
                    case 11:if(do_data&H_14){do_data^=H_14;}break;//A12
                    case 12:if(do_data&H_16){do_data^=H_16;}break;//A13
                    case 13:if(do_data&H_17){do_data^=H_17;}break;//A14
                }
                sendCommand(do_data);
            }
            did>>=1;
        }
        // if sensors A6 & A12 - clearEncoder
        if(di_data&H_06){clearEncoder(0);enc_s=0;enc=0;enc_ls=0;enc_l=0;}//Delay(8);} // нижний поршень в верхнем положении левый(0)
        if(di_data&H_12){clearEncoder(1);enc_s=0;enc=0;enc_rs=0;enc_r=0;}//Delay(8);} // нижний поршень в верхнем положении right
        //if((di_data&H_06)&&(do_data&H_06)){clearEncoder(0);enc_s=0;enc=0;}//Delay(8);} // нижний поршень в верхнем положении левый(0)
        //if((di_data&H_12)&&(do_data&H_15)){clearEncoder(1);enc_s=0;enc=0;}//Delay(8);} // нижний поршень в верхнем положении right

        if(sstep.wait.type==0 && ((di_data&sstep.wait.value)==sstep.wait.value)){break;}
        else if(sstep.wait.type == 1 && ((curTime-iTimeout)>= sstep.wait.value) ){break;}
        else if(sstep.wait.type == 2){
            enc_l=readEncoder2(0);
            enc_r=readEncoder2(1);
            if((do_data&H_05) && !(di_data&H_06)){
                enc = readEncoder2(0);
                if( (enc_ls - enc_l) >= sstep.wait.value){break;}
                if( (enc_s - enc) >= sstep.wait.value){break;}
                gRegisters[0x30] = 0;
            }
            else if((do_data&H_06)&& !(di_data&H_06)){
                enc = readEncoder2(0);
                if( (enc_l - enc_ls) >= sstep.wait.value){break;}
                gRegisters[0x30] = 0;
            }
            else if((do_data&H_14)&& !(di_data&H_12)){
                enc = readEncoder2(1);
                if( (enc_rs - enc_r) >= sstep.wait.value){break;}
                gRegisters[0x30] = 1;
            }
            else if((do_data&H_15)&& !(di_data&H_12)){
                enc = readEncoder2(1);
                if( (enc_r - enc_rs) >= sstep.wait.value){break;}
                gRegisters[0x30] = 1;
            }
        }
        RefreshWDT();
    }
    sendCommand(sstep.finish);
    RefreshWDT();
    return ret;
}
/*
static const dword diStopbits[] = {
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
    H_00 //A15
};*/
