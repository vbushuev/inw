#include "funcs.h"
int sendCommand(unsigned long command){
	DO_32(DOSLOT,command);
	inwPrint("DO data = %-08lX\n\r",command);
	return 0;
}
int readSignals(unsigned long *data){
	unsigned long di;
	di = DI_32(DISLOT);
	memcpy(data,&di,4);
	inwPrint("DI data = %08X\n\r",data);
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
    i8080_ReadCntUpDown(ECSLOT,channel,data,&Overflow);
    inwPrint("[%i]=%04x\r\n",channel,data);
    return 0;
}
int initComPort(){
    InstallCom(COMPORT,115200,8,0,1);
    return 0;
}
int closeComPort(){
    RestoreCom(COMPORT);
    return 0;
}
/*=====================================================================*/
/*==== Following functions are used to receive data from COM port.  ===*/
/*==== You can copy the functions to your own program.              ===*/
/*=====================================================================*/
/*
The following two functions are implemented by [non-block] method.

Non-Block method: After calling the function, if there is no data in the
			   COM port input buffer, the CPU skip the function and
			   go to execute next code.
*/
int Receive_Data(unsigned char* cInBuf,char cTerminator,long lTimeout){
	/*
	 Uses COM port to receive data with a terminative char.

	 COMPORT:    COM port number to receive data.
			   0:COM0, 1:COM1, 2:COM2  .....
	 *cInBuf:  Input buffer to receive data.
	 cTerminator: what is the last byte ?
	 lTimeout: timeout to receive data. (Unit: ms)
			   The timeout is measured from last received byte
			   to the terminator.
	 return: >0 :length of received data
			  0 :doen't receive any data
			 -1 :timeout
	*/
 	unsigned char cChar;
 	int iIndex=0;
 	unsigned long lStartTime;
	if(IsCom(COMPORT)){
		lStartTime=GetTimeTicks();
	 	for(;;){
			while(IsCom(COMPORT)) {//check COM port
				cChar=ReadCom(COMPORT);
			 	if(cChar==cTerminator){/* the terminal char is 0x0D */
					cInBuf[iIndex]=0;  /* Add the zero end to the data. */
				 	return iIndex;     /* return data length                */
				}/* (doesn't include the cTerminator) */
			 	else cInBuf[iIndex++]=cChar;
			 	lStartTime=GetTimeTicks();  /* refresh data timeout */
		 	}
		 	if((GetTimeTicks()-lStartTime)>=lTimeout){
				cInBuf[iIndex]=0; /* Add the zero end to the data. */
			 	return ERROR_COM_TIMEOUT;  /* receive data timeout */
		 	}
		 	RefreshWDT();
	 	}
 	}
 	else return 0;
}
int Receive_Data_Length(unsigned char* cInBuf,int iLength,long lTimeout){
	/*
	 Uses COM port to receive string (fixed data length).

	 COMPORT:    COM port number to receive data.
			   0:COM0, 1:COM1, 2:COM2  .....
	 *cInBuf:  Input buffer to receive data.
	 iLength:  how many bytes to receive?
	 lTimeout: timeout to receive data. (Unit: ms)
			   The timeout is measured from last received byte
			   to receive whole data.
	 return: >0 :length of received data
			  0 :doen't receive any data
			 -1 :timeout
	*/
	unsigned char cChar;
	int iIndex=0;
 	unsigned long lStartTime;
 	if(IsCom(COMPORT)){
		lStartTime=GetTimeTicks();
	 	for(;;){
		 	while(IsCom(COMPORT)){//check COM port
			 	cInBuf[iIndex++]=ReadCom(COMPORT);
			 	if(iIndex>=iLength){
					cInBuf[iIndex]=0;
				 	return iIndex;     /* return data length */
			 	}
				lStartTime=GetTimeTicks();  /* refresh data timeout */
		 	}
		 	if((GetTimeTicks()-lStartTime)>=lTimeout) return ERROR_COM_TIMEOUT;  /* receive data timeout */
			RefreshWDT();
	 	}
 	}
 	else return 0;
}
