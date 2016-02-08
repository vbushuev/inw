#include "funcs.h"

int exception(int e){
	Print("Error occurs %i",e);
	return 0;
}
int sendCommand(unsigned long command){
	//DO_32(DOSLOT,command);
	return 0;
}
int readSignals(unsigned long *data){
	unsigned long di;
	di = DI_32(DISLOT);
	memcpy(data,&di,4);
	Print("DI data = %-08lX\n\r",data);
	return 0;
}

int readEncoderL(long *up,long *down){
    int ret=0;
    ret = readEncoder(0,up);
    if(ret)return ret;
    ret = readEncoder(1,down);
    return ret;
}
int readEncoderR(long *up,long *down){
    int ret=0;
    ret = readEncoder(2,up);
    if(ret)return ret;
    ret = readEncoder(3,down);
    return ret;
}
int readEncoder(int channel,long *data){
    int Overflow;
    i8084W_ReadCntUpDown(ECSLOT,channel,data,&Overflow);
    Print("[%i]=%010ld ",channel,data);
    return 0;
}
int getRuntime(psRuntimeValues prtv){
    EE_MultiRead(EEPROM_RUNTIME,0,sizeof(sRuntimeValues),(psRuntimeValues)prtv);
    return 0;
}
int setRuntime(sRuntimeValues rtv){
    EE_WriteEnable();
    EE_MultiWrite(EEPROM_RUNTIME,0,sizeof(sRuntimeValues),(psRuntimeValues)&rtv);
    EE_WriteProtect();
    return 0;
}
int getTotal(psTotalValues ptv){
    EE_MultiRead(EEPROM_TOTAL,0,sizeof(sTotalValues),(psTotalValues)ptv);
    return 0;
}
int setTotal(sTotalValues tv){
    EE_WriteEnable();
    EE_MultiWrite(EEPROM_TOTAL,0,sizeof(sTotalValues),(psTotalValues)&tv);
    EE_WriteProtect();
    return 0;
}
int initInw(psRuntimeValues prtv, psTotalValues ptv){
    int iRet=0,channel;
    // Always do that to init lib i8000
	InitLib();
    // i8000 has only one systimer
	TimerOpen();
	TimerResetValue();
	//InitEncoder
	iRet=i8084W_InitDriver(ECSLOT);
    if (iRet==(-1)){
		Print("Initiate 8084 on slot%d error!\n\r",ECSLOT);
		Print("  Cannot find 8084.\r\n");
	}
	else{
		Print("Initiate 8084 on slot%d ok.\n\r",ECSLOT);
        if(iRet>0){
            Print("  Some Pulse/Dir channels have one count offset.\n\r");
            Print("  Return code:%02X\n\r",iRet);
        }
        for (channel=0; channel<8; channel++){
            i8084W_SetXorRegister(ECSLOT,channel,0); // XOR=0 (Low Actived)
            i8084W_SetChannelMode(ECSLOT,channel,1); // Up/Down counter mode
                //mode 0: Pulse/Dir counter mode
                //     1: Up/Down counter mode
                //     2: frequency mode
                //     3: Up counter mode

            i8084W_SetLowPassFilter_Status(ECSLOT,channel,0); //Disable LPF
            i8084W_SetLowPassFilter_Us(ECSLOT,channel,1); //Set LPF width= 0.001 ms
        }

        //Clear all count at beginning.
        for (channel=0; channel<8; channel++){
            i8084W_ClrCnt(ECSLOT,channel);
        }
    }
    iRet = getTotal(ptv);
    if(iRet) return iRet;
    iRet = getRuntime(prtv);
    if(iRet) return iRet;
    return 0;
}
/*
 * Start scenario
 */
/*
 * Work scenario automate
 */
 int loadMainScenario(sRuntimeValues rtv,sStep * sc){
     sStep scenatioW[]={
         {0x00000002,{0,0x00000002},0x00000000,0},  //00    убираем засыпку V2 -> A2
         {0x00000008,{0,0x00000008},0x00000000,0},  //01    верхний поршень прижимает V4 -> A4
         {0x00000410,{2,0},0x00000000,0},      //02    нижний поршень прижимает и отпускает фитиль(V11) V5->LS
         {0x00000008,{0,0x00000010},0x00000000,0},  //03    верхний дожимает V4 -> A5
         {0x00000004,{0,0x00000004},0x00000000,0},  //04    убираем вверхний V3->A3
         {0x00000080,{0,0x00000080},0x00000000,0},  //05    выдвигаем ухват V8->A8
         {0x00000410,{0,0x00000020},0x00000000,0},  //06    нижний поршень выдвигает и отпускает фитиль (V11) V5->A5
         {0x00000100,{1,0},0x00000100,0},  //07    зажимаем ухват и чуть ждем  V9
         {0x00000120,{2,0},0x00000100,0},      //08    опускаем нижний поршень V6->LH
         {0x00000300,{1,0},0x00000100,0},    //09    отрезаем свечу V10 на время tV10
         {0x00000140,{0,0x00000040},0x00000000,0},  //10    убираем ухват и отпускаем зажим V7->A7 V9=off
         {0x00000140,{0,0x00000040},0x00000000,0},  //11    ставим засыпаться левый цилиндр засыпку V1 -> A1

         {0x00000000,{3,0x00000000},0x00000000,0},  //12    Произвести фиксацию замеров для левого

         {0x00001000,{0,0x00000200},0x00000000,0},  //13    верхний поршень прижимает V13 -> A10
         {0x00082000,{2,0},0x00000000,0},      //14    нижний поршень прижимает и отпускает фитиль(V20) V14->RS
         {0x00001000,{0,0x00000400},0x00000000,0},  //15    верхний дожимает V13 -> A11
         {0x00000800,{0,0x00000100},0x00000000,0},  //16    убираем вверхний V12->A9
         {0x00008000,{0,0x00001000},0x00000000,0},  //17    выдвигаем ухват V16->A13
         {0x00002000,{0,0x00000800},0x00000000,0},  //18    нижний поршень выдвигает и отпускает фитиль (V20) V4->A12
         {0x00020000,{1,0},0x00020000,0},  //19    зажимаем ухват и чуть ждем  V18
         {0x00024000,{2,0},0x00020000,0},      //20    опускаем нижний поршень V15->RH
         {0x00060000,{1,0},0x00020000,0},    //21    отрезаем свечу V19 на время tV19
         {0x00030000,{0,0x00002000},0x00000000,0},  //22    убираем ухват и отпускаем зажим V17->A14 V18=off

         {0x00000000,{4,0x00000000},0x00000000,0}   //23    Произвести фиксацию замеров для левого
     };
     scenatioW[2].wait.value = rtv.LC;
     scenatioW[7].wait.value = rtv.ttaker;
     scenatioW[8].wait.value = rtv.Lh;
     scenatioW[9].wait.value = rtv.tV10;
     scenatioW[14].wait.value = rtv.RC;
     scenatioW[19].wait.value = rtv.ttaker;
     scenatioW[20].wait.value = rtv.Rh;
     scenatioW[21].wait.value = rtv.tV19;
     memcpy(sc,scenatioW,sizeof(scenatioW));
     return 0;
}
