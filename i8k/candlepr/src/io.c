#include "funcs.h"
extern int gRegisters[128];
/******************************************************************************
 * DI/DO
 ******************************************************************************/
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
/******************************************************************************
 * Encoder section
 ******************************************************************************/
int clearEncoder(int piston){
	i8080_ClrCnt(ECSLOT,piston);
	gRegisters[0x23+piston] = 0;
	gRegisters[0x29+2*piston] = 0; // save old position;
	gRegisters[0x28+2*piston] = 0; // current position;
	return 0;
}
int InitEncoder(){
	int channel;
	for (channel=0; channel<8; channel++){
		i8080_SetXorRegister(ECSLOT,channel,0); // XOR=0 (Low Actived)
		i8080_SetChannelMode(ECSLOT,channel,1); // Up/Down counter mode
			//mode 0: Pulse/Dir counter mode
				//     1: Up/Down counter mode
				//     2: frequency mode
				//     3: Up counter mode

		i8080_SetLowPassFilter_Status(ECSLOT,channel,0); //Disable LPF
		i8080_SetLowPassFilter_Us(ECSLOT,channel,1); //Set LPF width= 0.001 ms
	}
	//Clear all count at beginning.
	for (channel=0; channel<8; channel++) i8080_ClrCnt(ECSLOT,channel); // the last one
	return 0;
}
int readEncoder(int channel,long *data){
    int Overflow;
	long count;
    i8080_ReadCntUpDown(ECSLOT,channel,&count,&Overflow);
	*data = (long)Overflow * 0x80000000 + count;
    return 0;
}
int Encoder(int piston,unsigned long *data){
	long value;
	readEncoder(0+piston,&value);
	gRegisters[0x29+2*piston] = gRegisters[0x28+2*piston]; // save old position;
	gRegisters[0x28+2*piston] = value; // current position;
	gRegisters[0x23+piston] += value;
	*data = gRegisters[0x23+piston];
	//if ((long)gRegisters[0x23+piston]<0) return ERROR_ENCODER_UBNORMAL;
	return 0;
}
int Encoder2(int piston,unsigned long *data){
	long up,down;
	readEncoder(0+piston*2,&up);
	readEncoder(1+piston*2,&down);
	if(up<0){
		long t;
		t = down;
		down = -up;
		up = t;
	}
	else if(down<0){
		long t;
		t = up;
		up = -down;
		down = t;
	}
	gRegisters[0x29+4*piston] = gRegisters[0x28+4*piston]; // save old position;
	gRegisters[0x28+4*piston] = (dword)down; // current position;
	gRegisters[0x2b+4*piston] = gRegisters[0x2a+4*piston]; // save old position;
	gRegisters[0x2a+4*piston] = (dword)up; // current position;
	gRegisters[0x23+piston] += (gRegisters[0x28+4*piston] - gRegisters[0x29+4*piston]);
	gRegisters[0x23+piston] -= (gRegisters[0x2b+4*piston] - gRegisters[0x2a+4*piston]);
	*data = gRegisters[0x23+piston];
	//if ((long)gRegisters[0x23+piston]<0) return ERROR_ENCODER_UBNORMAL;
	return 0;
}

/******************************************************************************
 * ComPort section
 ******************************************************************************/
int initComPort(){
    InstallCom(COMPORT,115200,8,0,1);
    return 0;
}
int closeComPort(){
    RestoreCom(COMPORT);
    return 0;
}
int Receive_Data(unsigned char* cInBuf,char cTerminator,long lTimeout){

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
/******************************************************************************
 * IniFie section
 ******************************************************************************/
unsigned long FSeek(FILE_DATA far *file_pointer,char cMark,unsigned long lStart,int iTh){


    char c;
    unsigned long i;
    int iOrder;

    i=lStart;
    iOrder=1;

    while(i<(file_pointer->size-1))
    {
        c=file_pointer->addr[i];
        if(c==cMark)
            if(iOrder==iTh)
                return ++i; /* return the position next to the cMark. */
            else
                iOrder++;
        i++;
    }
    return 0;
}
long GetProFileInt(FILE_DATA far *file_pointer,char* sKeyName,long lDefault){
    /*
    If can't find the sKeyName, return the lDefault.
    If can't find the value of the sKeyName, return the lDefault.
    */

    unsigned long lPointer=0;
    char sTemp[20];
    long lValue;
    int i;
    strlwr(sKeyName);

    /* Find  out the Item */
    while(lPointer<file_pointer->size)
    {
        lPointer=FSeek(file_pointer,'*',lPointer,1); /* To find the 1st '*' from lPointer. */
        if(lPointer==0) /* cannot find '*' */
            return lDefault;
        sscanf(file_pointer->addr+lPointer,"%s",&sTemp);
        strlwr(sTemp);
        for(i=0;i<strlen(sTemp)-1;i++)
            if(sTemp[i]=='=')
            {
                sTemp[i]=0;
                break;
            }
        /* Read the Value of the Item. */
        if(!strcmp(sKeyName,sTemp))
        {
            lPointer=FSeek(file_pointer,'=',lPointer,1); /* To find the 1st '=' from lPointer. */
            if(lPointer==0) /* cannot find '=' */
                return lDefault;
            sscanf(file_pointer->addr+lPointer,"%ld",&lValue);
            return lValue;
        }
    }
    /* EOF */
    return lDefault;
}
float GetProFileFloat(FILE_DATA far *file_pointer,char* sKeyName,float fDefault){
    /*
    Gets float of sKeyName, the sKeyName in text file must be after one '*'.
    If can't find the sKeyName, return the fDefault.
    If can't find the value of the sKeyName, return the fDefault.
    */

    unsigned long lPointer=0;
    char sTemp[20];
    float fValue;

    strlwr(sKeyName);

    /* Find  out the Item. */
    while(lPointer<file_pointer->size)
    {
        lPointer=FSeek(file_pointer,'*',lPointer,1);/* To find the 1st '*' from lPointer */
        if(lPointer==0) /* cannot find '*' */
            return fDefault;
        sscanf(file_pointer->addr+lPointer,"%s",&sTemp);
        strlwr(sTemp);

        /* Read the Value of the Item. */
        if(!strcmp(sKeyName,sTemp))
        {
            lPointer=FSeek(file_pointer,'=',lPointer,1); /* To find the 1st '=' from lPointer */
            if(lPointer==0) /* cannot find '=' */
                return fDefault;
            sscanf(file_pointer->addr+lPointer,"%f",&fValue);
            return fValue;
        }
    }
    /* EOF */
    return fDefault;
}
int GetProFileStr(FILE_DATA far *file_pointer,char* sKeyName,char* sResult,char* sDefault){
    /*
    If can't find the sKeyName, sResult=sDefault.
    If can't find the string of the sKeyName, sResult=sDefault.
    Return value: length of sResult
    */

    unsigned long lPointer=0;
    char sTemp[20];
    int i,iLen;

    strlwr(sKeyName);

    /* Find  out the Item */
    while(lPointer<file_pointer->size)
    {
        lPointer=FSeek(file_pointer,'*',lPointer,1); /* To find the 1st '*' from lPointer */
        if(lPointer==0) /* cannot find '*' */
        {
            strcpy(sResult,sDefault);
            return strlen(sDefault);
        }
        sscanf(file_pointer->addr+lPointer,"%s",&sTemp);
        strlwr(sTemp);
        for(i=0;i<strlen(sTemp)-1;i++)
            if(sTemp[i]=='=')
            {
                sTemp[i]=0;
                break;
            }
        /* Read the String of the Item */
        if(!strcmp(sKeyName,sTemp))
        {
            lPointer=FSeek(file_pointer,'=',lPointer,1); /* To find the 1st '*' from lPointer */
            if(lPointer==0) /* cannot find '=' */
            {
                strcpy(sResult,sDefault);
                return strlen(sResult);
            }
            sscanf(file_pointer->addr+lPointer,"%s",&sTemp);
            if(strlen(sTemp)==0)    /* cannot find string */
                strcpy(sResult,sDefault);
            else
                strcpy(sResult,sTemp);
            return strlen(sResult);
        }
    }
    /* EOF */
    strcpy(sResult,sDefault);
    return strlen(sResult);
}
