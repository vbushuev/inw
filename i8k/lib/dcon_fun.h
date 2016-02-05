// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DCON_FUN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DCON_FUN_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

/*/
#ifdef DCON_FUN_EXPORTS
#define DCON_FUN_API __declspec(dllexport)
#else
#define DCON_FUN_API __declspec(dllimport)
#endif


// This class is exported from the DCON_FUN.dll
class DCON_FUN_API CDCON_FUN {
public:
	CDCON_FUN(void);
	// TODO: add your methods here.
};

extern DCON_FUN_API int nDCON_FUN;

DCON_FUN_API int fnDCON_FUN(void);

/*/
//==========================================================================
//==========================================================================


#define    _NoError                      0
#define    _ResultStrCheckError          12
#define    _UnderInputRange              19
#define    _ExceedInputRange             20

#ifdef I8000	
	
	#define _EXP short
    short Send_Receive_Cmd(unsigned char iComPort,char OutBuf[],char InBuf[],short iTimeOut,short iCheckSum,unsigned short *WT);
#else	       
	#ifdef DCON_FUN_EXPORTS
		#define _EXP __declspec(dllexport) short __stdcall
    #else
		#define _EXP __declspec(dllimport) short __stdcall
	#endif
#endif
	
#ifdef	__cplusplus  // for C++ compile use
extern "C" {
#endif	/*	__cplusplus	*/

_EXP DCON_Write_DO(unsigned char cComPort,short iAddress,short iSlot,short iDO_TotalCh,
						unsigned long lDO_Value,short iCheckSum,short iTimeOut);
/****************************************************************************
    iComPort: 1 ~ 255
    iAddress: 0 ~ 255: the RS-485 Address of module 
    iSlot: 0~7: for 8K and 87K series on i-8000 system 
            -1: 7K and 87K (on 87K IO Expansion bus)
    iDO_TotalCh: 1~32. total DO channels        
    lDO_Value: DO value. 
                for example: 0x32==> ch5=1, ch4=1, ch1=1, 
                                     others are 0.  
    iCheckSum: 0: disabled 
               1: enabled
    iTimeOut: timeout value to wait response. unit= ms.
              
    return: 0 = No Error
            other = error code.                                     
****************************************************************************/						

_EXP DCON_Write_DO_Bit(unsigned char cComPort,short iAddress,short iSlot,short iChannel,
							short iDO_TotalCh,short iBitValue,short iCheckSum,short iTimeOut);
/***********************************************************************
    iComPort: 1 ~ 255
    iAddress: 0 ~ 255: the RS-485 Address of module 
    iSlot: 0~7: for 8K and 87K series on i-8000 system 
            -1: 7K and 87K (on 87K IO Expansion bus)0
    iChannel: 0~31
    iDO_TotalCh: 1~32, Total DO channels
    iBitValue: 0   = Off
               <>0 = On        
    iCheckSum: 0: disabled 
               1: enabled
    iTimeOut: timeout value to wait response. unit= ms.
              
    return: 0 = No Error
            other = error code.                                     
*************************************************************************/

_EXP DCON_Read_DI_Counter(unsigned char cComPort,short iAddress,short iSlot,short iChannel,
							 short iDI_TotalCh,short iCheckSum,short iTimeOut,
							 unsigned long *lCounter_Value);
/*******************************************************************
    iComPort: 1 ~ 255
    iAddress: 0 ~ 255: the RS-485 Address of module 
    iSlot: 0~7: for 87K series on i-8000 system 
            -1: 7K and 87K (on 87K IO Expansion bus)
    iChannel: 0~15        
    iCheckSum: 0: disabled 
               1: enabled
    iTimeOut: timeout value to wait response. unit= ms.
    *lCounter_Value: counter value.
              
    return: 0 = No Error
            other = error code. 
**********************************************************************/

_EXP DCON_Clear_DI_Counter(unsigned char cComPort,short iAddress,short iSlot,short iChannel,
							 short iDI_TotalCh,short iCheckSum,short iTimeOut);
/******************************************************************
iComPort: 1 ~ 255
iAddress: 0 ~ 255: the RS-485 Address of module 
iSlot: 0~7: for 8K and 87K series on i-8000 system 
        -1: 7K and 87K (on 87K IO Expansion bus)
iChannel: 0~15 
iDI_TotalCh: 1~32. total DI channels
iCheckSum: 0: disabled 
           1: enabled
iTimeOut: timeout value to wait response. unit= ms.
          
return: 0 = No Error
        other = error code. 
******************************************************************/    

_EXP DCON_Write_AO(unsigned char cComPort,short iAddress,short iSlot,short iChannel,
				   short iAO_TotalCh,float fValue,short iCheckSum,short iTimeOut);
/***************************************************************************
    iComPort: 1 ~ 255
    iAddress: 0 ~ 255: the RS-485 Address of module 
    iSlot: 0~7: for 8K and 87K series on i-8000 system 
            -1: 7K and 87K (on 87K IO Expansion bus)
    iChannel: 0~3
    iAO_TotalCh: 1~4. total AO channels
    fValue: AO value
    iCheckSum: 0: disabled 
               1: enabled
    iTimeOut: timeout value to wait response. unit= ms.
              
    return: 0 = No Error
            other = error code.  
*****************************************************************************/

_EXP DCON_Read_AO(unsigned char cComPort,short iAddress,short iSlot,
						short iChannel,short iAO_TotalCh,short iCheckSum,
						short iTimeOut,float *fValue);
/***********************************************************************************  
   iComPort: 1 ~ 255
   iAddress: 0 ~ 255: the RS-485 Address of module 
   iSlot: 0~7: for 8K and 87K series on i-8000 system 
           -1: 7K and 87K (on 87K IO Expansion bus)
   iAO_TotalCh: 1~4. total AO channels,used to define command and response 
   
   iCheckSum: 0: disabled 
              1: enabled
   iTimeOut: timeout value to wait response. unit= ms.
    
   iDataFormat: 0: Engineering format
                 1: Percent format
                 2: Hex format
                     
   iValue: Hex format 
   fValue: float format   
    
   return: 0 = No Error
           15= timeout (No response)     


    Note: user can readback AO output in either float value or hex format ,
          according to the data format of module.
          but user must declare both the variable in function 
    for example:
    float fVal;
    int iVal,format;
    short ret;
    ....
    format=0;
    fVal=5.0;
    iVal=0;
    ret= DCON_Read_AO(1,1,-1,0,4,0,300,format,&fVal,&iVal);
        
    //then the current AO value will be fVal, the iVal will get the default 0    
**********************************************************************************/

_EXP DCON_Read_AI(unsigned char cComPort,short iAddress,short iSlot,short iChannel,
						short iAI_TotalCh,short iCheckSum,short iTimeOut,short iDataFormat,
						float *fValue,short *iValue);
/*****************************************************************************
    iComPort: 1 ~ 255
    iAddress: 0 ~ 255: the RS-485 Address of module 
    iSlot: 0~7: for 8K and 87K series on i-8000 system 
            -1: for 7K and 87K (on 87K IO Expansion bus)
    iChannel: 0~7
    iAI_TotalCh: 1~ N, total AI channels
    iCheckSum: 0: disabled 
               1: enabled
    iTimeOut: timeout value to wait response. unit= ms.
    
    *fValue:the AI value (engineering , % or ohmn data type).
    
    return: 0 = No Error
            other = error code.
******************************************************************************/

_EXP DCON_Read_Counter(unsigned char cComPort,short iAddress,short iSlot,short iChannel,
					 short iCheckSum,short iTimeOut,unsigned long *lCounter_Value);
/**********************************************************************
    iComPort: 1 ~ 255
    iAddress: 0 ~ 255: the RS-485 Address of module 
    iSlot: 0~7: for 8K and 87K series on i-8000 system 
            -1: 7K and 87K (on 87K IO Expansion bus)
    iChannel: 0~15        
    iCheckSum: 0: disabled 
               1: enabled
    iTimeOut: timeout value to wait response. unit= ms.
    *lCounter_Value: counter value.
              
    return: 0 = No Error
            other = error code. 
***********************************************************************/

_EXP DCON_Clear_Counter(unsigned char iComPort,short iAddress,short iSlot,short iChannel,
										short iCheckSum,short iTimeout);
/*********************************************************************
    iComPort: 1 ~ 255
    iAddress: 0 ~ 255: the RS-485 Address of module 
    iSlot: 0~7: for 8K and 87K series on i-8000 system 
            -1: 7K and 87K (on 87K IO Expansion bus)
    iChannel: 0~15        
    iCheckSum: 0: disabled 
               1: enabled
    iTimeOut: timeout value to wait response. unit= ms.
              
    return: 0 = No Error
            other = error code. 
***********************************************************************/

_EXP DCON_Read_DIO(unsigned char cComPort,short iAddress,short iSlot,
								short iDI_TotalCh,short iDO_TotalCh,short iCheckSum,short iTimeOut,
								unsigned long* lDI_Value,unsigned long* lDO_Value,char* cDI_BitValue,
								char* cDO_BitValue);
/*********************************************************************************       
   iComPort: 1 ~ 255
   iAddress: 0 ~ 255: the RS-485 Address of module 
   iSlot: 0~7: for 8K and 87K series on i-8000 system 
           -1: 7K and 87K (on 87K IO Expansion bus)
   iDI_TotalCh: 0~32. total DI channels,used to define the input bytes 
   iDO_TotalCh: 0~32. total DO channels,used to define the output bytes
   iCheckSum: 0: disabled 
              1: enabled
   iTimeOut: timeout value to wait response. unit= ms.
   *lDI_Value: DI value. 
               for example: 0x32==> ch5=1, ch4=1, ch1=1, 
                                    others are 0.       
   *lDO_Value: DO read back value.
               for example: 0x32==> ch5=on, ch4=on, ch1=on, 
                                    others are off.                                      
   *cDI_BitValue: DI value.
                  for example: {0,1,0,0,1,1} 
                  ==> bit0=0,bit1=1,bit2=0,bit3=0,bit4=1,bit5=1.
   *cDO_BitValue: DO read back value.
                  for example: {0,1,0,0,1,1} 
                  ==> bit0=0,bit1=1,bit2=0,bit3=0,bit4=1,bit5=1.
    
    return: 0 = No Error
            15= timeout (No response)  
**********************************************************************************/                                 

_EXP DCON_READ_DIO_Latch(unsigned char cComPort,short iAddress,short iSlot,
							short iDI_TotalCh,short iDO_TotalCh,short iLatchType,
							short iCheckSum,short iTimeOut,unsigned long *lDI_Latch_Value,
							unsigned long *lDO_Latch_Value,char *cDI_Latch_BitValue,
							char *cDO_Latch_BitValue);
/***************************************************************************
    iComPort: 1 ~ 255
    iAddress: 0 ~ 255: the RS-485 Address of module 
    iSlot: 0~7: for 8K and 87K series on i-8000 system 
            -1: 7K and 87K (on 87K IO Expansion bus)
    iDI_TotalCh: 0~32. total DI channels,used to define input bytes
    iDO_TotalCh: 0~32. total DO channels,used to define the output bytes       
    iLatchType: 0 = Logic Low Latched
                1 = Logic High Latched
    iCheckSum: 0: disabled 
               1: enabled
    iTimeOut: timeout value to wait response. unit= ms.
    *lDI_Latch_Value: DI latch value. 
                for example: 0x32==> ch5=1, ch4=1, ch1=1, 
                                     others are 0.       
    *lDO_Latch_Value: DO read back latch value.
                for example: 0x32==> ch5=on, ch4=on, ch1=on, 
                                     others are off.                                      
    *cDI_Latch_BitValue: DI latch value.
                   for example: {0,1,0,0,1,1} 
                   ==> bit0=0,bit1=1,bit2=0,bit3=0,bit4=1,bit5=1.
    *cDO_Latch_BitValue: DO read back latch value.
                   for example: {0,1,0,0,1,1} 
                   ==> bit0=0,bit1=1,bit2=0,bit3=0,bit4=1,bit5=1.                                                    
                   
    return: 0 = No Error
            15= timeout (No response)      
****************************************************************************/

_EXP DCON_Clear_DIO_Latch(unsigned char cComPort,short iAddress,short iSlot,
							   short iCheckSum,short iTimeOut);
/*******************************************************************
    iComPort: 1 ~ 255
    iAddress: 0 ~ 255: the RS-485 Address of module 
    iSlot: 0~7: for 8K and 87K series on i-8000 system 
            -1: 7K and 87K (on 87K IO Expansion bus)       
    iCheckSum: 0: disabled 
               1: enabled
    iTimeOut: timeout value to wait response. unit= ms.
    
              
    return: 0 = No Error
            15= timeout (No response)  
			

**********************************************************************/

_EXP DCON_Read_Overflow(unsigned char cComPort,short iAddress,short iSlot,
        short iChannel,short iCheckSum,short iTimeOut,
        short *iOverflow);
/*******************************************************************
    iComPort: 1 ~ 255
    iAddress: 0 ~ 255: the RS-485 Address of module 
    iSlot: 0~7: for 8K and 87K series on i-8000 system 
            -1: 7K and 87K (on 87K IO Expansion bus)       
    iCheckSum: 0: disabled 
               1: enabled
    iTimeOut: timeout value to wait response. unit= ms.
    
              
    return: 0 = No Error
            15= timeout (No response)  
			

**********************************************************************/

int DCON_Get_Lib_Version();
/*********************************************************
User use this function to read the version of lib.
*********************************************************/ 
        
#ifdef	__cplusplus
}
#endif	/*	__cplusplus	*/
