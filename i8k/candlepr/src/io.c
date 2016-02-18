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
/*
	//Opens one file by name.
	config=GetFileInfoByName("config_2.ini");
 */
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
unsigned long FSeek(FILE_DATA far *file_pointer,char cMark,unsigned long lStart,int iTh){
    /*
    Seeks the iTh(th) cMark from the start position
    to the EOF(end of file).

    cMark: find the position next to the cMark.
    lStart: the start position to seek the cMark.
            (0: start position of the file)
    iTh: seek the iTh(th) cMark.

    Return value:
        0: can't find the cMark
       >0: the offset position next to the cMark
    */

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
