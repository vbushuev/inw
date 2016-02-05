#include "funcs.h"
int initInw(){
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
		Print("  Cannot find 8084.");
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
    return 0;
}
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
