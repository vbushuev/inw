/*
I-8017H,I-8017HS,I-8017HW Library
[Version 1.06]
Version 1.0.6 [20,Nov,2008] by Martin
	New features:
		when using i8017H_ReadAI and i8017H_ReadAIHex to get analog input data, 
		user does not need to call i8017H_Set_ChannelGainMode.	
		please refer to demo program at i8017H_ReadAI.
		
	Add functions
		i8017H_GetSingleEndJumper
		i8017H_GetFirmwareVersion
		i8017H_ReadGainOffset_Info
		i8017H_ReadAI
		i8017H_ReadAIHex
	New Demo 8017h_ReadAI to present
		i8017H_GetSingleEndJumper
		i8017H_GetFirmwareVersion
		i8017H_ReadGainOffset_Info
		i8017H_ReadAI	
		
	
	What are the new features for i8017h_ReadAI,i8017H_ReadAIHex
		1. support error code 
		2. don't need to call i8017H_Set_ChannelGainMode before using these two function
			i8017h_ReadAI and i8017h_ReadAIHex will call i8017H_Set_ChannelGainMode automatically 
			if channel or gain is different from previous.
		3. i8017h_ReadAIHex read 16 bit hex format and calibated analog input value.
		
	To use i8017H_GetSingleEndJumper
		1. 8017HS and 8017HW have two input signal modes, default is "Differential Mode", others is "Single Ended Mode"
		i8017H_GetSingleEndJumper can get the jumper location, 
		if jumper=0 means "Differential Mode" ==> isolated circuit between channels, and has 8 channels for input.
		if jumper=1 means "Single Ended Mode" ==>does not have isolated circuit between channels, and has 15 channels for input.

	To use i8017H_GetFirmwareVersion, i8017H_ReadGainOffset_Info
		1. i8017H_GetFirmwareVersion gets the firmware version of 8017H,8017HS and 8017HW.
		2. i8017H_ReadGainOffset_Info gets the calibration gain values offset values  for each measurement range.	
			

	

*/

/************************************************************************/
/*  Head file of 8017H, 8017HS       [2005,12,27] Updated by Kevin      */
/*                                                                      */
/*  8017H is a 8-channel hi speed 14-bit A/D module                     */
/*  The library provides 5 modes to get A/D values                      */
/*    1): 1 channel A/D convert to Float                                */
/*          (1 sampling point)                                          */
/*          Func: i8017H_Get_AD_FValue                                  */
/*                                                                      */
/*    2): 1 channel A/D convert to HEX                                  */
/*          (1 sampleing point)                                         */  
/*          Func: i8017H_Get_AD_HValue                                  */
/*                                                                      */
/*    3): 1 channels polling mode A/D convert to Hex                    */  
/*          (multi sampling points)                                     */
/*          Func: i8017H_AD_Polling                                     */
/*                                                                      */
/*    4): 1 channel interrupt mode A/D convert to Hex                   */
/*          (multi sampling points)                                     */
/*          (Sampling rate: 50K Hz Max.)                                */
/*          Func: i8017H_AD_TimerINT                                    */
/*                                                                      */
/*    5): 8 channels interrupt-scan mode A/D convert to Hex             */
/*          (multi sampling points)                                     */
/*          (Sampling rate: 10K Hz Max.)                                */
/*          Func: i8017H_AD_TimerINT_Scan                               */
/************************************************************************/



#ifdef __cplusplus
extern "C" {
#endif


#define NoError 				 0
#define ID_ERROR				-1
#define SLOT_ERROR				-2
#define CHANNEL_ERROR				-3
#define GAIN_ERROR				-4
#define INT_MODE_ERROR  			-5
#define NOT_SUPPORT_ERROR			-6
#define NOT_Calibration				-7
#define Bad_Calibration			 	-8




/***********************************************************************/
/* With following definations, old code can be compiled with the new   */
/* functions without any change.                                       */ 
#define Gain8017H   i8017H_Gain
#define ActGain     i8017H_ActGain
#define Offset8017H i8017H_Offset
#define ActOffset   i8017H_ActOffset

#define Init_8017H                  i8017H_Init
#define Set_8017H_Channel_Gain_Mode i8017H_Set_ChannelGainMode  
#define Get_AD_FValue               i8017H_Get_AD_FValue 
#define Get_AD_IValue();            i8017H_Get_AD_FValue(4);
#define Get_AD_HValue               i8017H_Get_AD_HValue
#define I8017H_AD_POLLING           i8017H_AD_Polling
#define I8017H_AD_INT               i8017H_AD_TimerINT
#define I8017H_AD_INT_SCAN          i8017H_AD_TimerINT_Scan
#define Set_8017H_LED               i8017H_Set_LED  
#define ARRAY_HEX_TO_FLOAT          i8017H_Array_HexToFloat
#define HEX_TO_FLOAT                i8017H_HexToFloat  
/***********************************************************************/


extern unsigned int i8017H_Gain[8][5],i8017H_ActGain;
extern short i8017H_Offset[8][5],i8017H_ActOffset;
extern int i8017H_Input_Mode_SingleEnd[8]; 
            //Input mode: 1=Single End
            //            0=Differential
            
            


/* function to get the library version of i-8017h
/*
/*	      return: version number. 
/*        for example: 0x106; = Rev:1.0.6
*/
short i8017H_GetLibVersion(void);


/* function to get the library built date
/*
/*	      return: none
*/	
void i8017H_GetLibDate(char libDate[]);  //unsigned char -> char  




/* function to get the lattice version of i-8017h at specific slot
/*
/*        iSlot: 0 ~ 7
/*	      return: Error code . 
/*		  
*/	
short i8017H_GetFirmwareVersion(int iSlot,short* firmware);
	 

/* function to get the calibrated gain and offset value for i-8017h at specific slot for certain input range
/*
/*        iSlot: 0 ~ 7
/*        iGain:
/*			0: +/- 10.0V 
/*			1: +/- 5.0V 
/*			2: +/- 2.5V 
/*			3: +/- 1.25V 
/*			4: +/- 20mA 
/*        iGainValue: the calibated gain value 
/*        iOffsetValue: the calibated offset value 
/*
/*		  return: Error code
*/	
short i8017H_ReadGainOffset_Info(int slot,int Gain,unsigned short* GainValue, short* offsetValue);



/* function to get the single ended/differential jumper status for i-8017h at specific slot
/*
/*	      selectJumper: 1 Single Ended; 0 Differential. 
/*        iSlot: 0 ~ 7
/*		  return: Error code
*/	
short i8017H_GetSingleEndJumper(int iSlot,short* selectJumper);

/* function to get connector status between D sub and 8017DW  (open or close) for i-8017h at specific slot
/*
/*	      D Sub status: 1  means "Open"; 0 means "Close". 
/*        iSlot: 0 ~ 7
/*		  return: Error code
/*            function for I-8017DW module only, in the others 8017 series module, 
/*            the value of D Sub Status will always 1 (Open)
*/	
short i8017H_Get_D_Sub_Status(int iSlot,short* D_Sub_Status);



/* function to initialize i-8017h at specific slot
/*
/*	      return: None. 
/*        iSlot: 0 ~ 7
*/	 
void i8017H_Init(int iSlot);






/* function to have programmable LED control for i-8017h at specific slot
/*
/*	      return: None. 
/*        iSlot: 0 ~ 7
/*		  iLedValue: 0 ~ 0xffff
*/	
void i8017H_SetLED(int iSlot,unsigned short iLedValue);




/* function to read float format analog input data from i-8017h at specific slot
/*
/*        iSlot: 0 ~ 7
/*        iChannel: 0 ~ 7 if Differential mode; 0 ~ 15 if Single Ended mode
/*        iGain:
/*			0: +/- 10.0V 
/*			1: +/- 5.0V 
/*			2: +/- 2.5V 
/*			3: +/- 1.25V 
/*			4: +/- 20mA 
/*		  fValue: float format analog input data. 
/*
/*		  return: Error code
*/	
short i8017H_ReadAI(int iSlot,int iChannel,int iGain,float* fValue);
short i8017H_ReadAI_AVG(int slot,int iChannel,int iGain,unsigned short agerageCnt,float* fValue);
short i8017H_GetRawAD();

/* function to read 16 bit hex format analog input data from i-8017h at specific slot
/*
/*        iSlot: 0 ~ 7
/*        iChannel: 0 ~ 7 if Differential mode; 0 ~ 15 if Single Ended mode
/*        iGain:
/*			0: +/- 10.0V 
/*			1: +/- 5.0V 
/*			2: +/- 2.5V 
/*			3: +/- 1.25V 
/*			4: +/- 20mA 
/*		  iValue: 16 bit hex format analog input data. 
/*
/*		  return: Error code
*/
short i8017H_ReadAIHex(int iSlot,int iChannel,int iGain,short* iValue);

short i8017H_ReadAIHex_AVG(int slot,int iChannel,int iGain,unsigned short agerageCnt,short* iValue);



short i8017H_INTISR(int slot,void (*isr)(void),float sampleRate) ;
void i8017H_EndINT(int slot);

short i8017H_GetRawAD_Scan(int slot,int nextCh, int nextGain);
/*
    Sets 8017 MUX configuration.
    
    iSlot: 0~3 or 0~7
    iChannel: 0~7
    iGain : 0 ==>   +/- 10.0  V
            1 ==>   +/-  5.0  V
            2 ==>   +/-  2.5  V
            3 ==>   +/-  1.25 V
            4 ==>   +/- 20.0  mA
    iMode: 0 ==> polling mode 
                (for i8017_Get_AD_Value and i8017_Get_AD_Value_Hex)
           1 ==> timer interrupt mode
    
    [English comment]       
    If the channel, range or mode you want to sample is different 
    to previous sampling, you must call the function first to change 
    the MUX configuration.
    Note: You need to consider to call the function only when using
            1. i8017H_Get_AD_FValue 
            2. i8017H_Get_AD_FValue 
          Because other functions that to sample multi-data already 
          include function to set MUX. 
          
    [Chinese comment]
    設定 8017 的 MUX。
    假如你要取樣的 Channel,輸入範圍或模式和上一次取樣時的是不一樣的，
    那麼你必須先執行這個 Function 來改變 MUX 的設定，然後再開始執行取
    樣的 Function。
    注意：只有使用 
            1. i8017H_Get_AD_FValue 
            2. i8017H_Get_AD_FValue
          時，才需要考慮是不是要先執行這個 Function。因為其他多點取樣
          的 Function 中，已作好多工器 (MUX) 的設定了。
*/
void i8017H_Set_ChannelGainMode(int iSlot,int iChannel,int iGain,int iMode);







/* 
    Get A/D value (both voltage and current) in floating format
    iGain : 0 ==>   +/- 10.0  V
            1 ==>   +/-  5.0  V
            2 ==>   +/-  2.5  V
            3 ==>   +/-  1.25 V
            4 ==>   +/- 20.0  mA
*/
float i8017H_Get_AD_FValue(int iGain);



    
/*
    Get A/D value in (both voltage and current) hexdecimal format
    8017H is 14-bit resolution. It uses 14-bit 2's format to store values.
    When A/D value between 0000 ~ 1FFF ==>       0     ~ + max value
                   between 2000 ~ 3FFF ==> - max value ~ 0 (a little less than 0)
    Algorithm to convert 14-bit integer (iValue) to float (fValue)
        fValue=iValue/8192*span         ,fValue>=0, iValue between 0000~1FFF 
        fValue=(8192-iValue)/8192*span  ,fValue<0,  iValue between 2000~3FFF
*/
int i8017H_Get_AD_HValue(void);





/*
    Uses polling mode to sample A/D values of one chnnel.
    iSlot: 0~3 or 0~7
    iChannel: 0~7
    iGain:  0 ==>   +/- 10.0  V
            1 ==>   +/-  5.0  V
            2 ==>   +/-  2.5  V
            3 ==>   +/-  1.25 V
            4 ==>   +/- 20.0  mA
    iDataCount: 1~8192
    iDataPointer: pointer to the data buffer
    
    Note: The data is an uncalibrated 16-bit value.
          You can use ARRAY_HEX_TO_FLOAT or HEX_TO_FLOAT
          to calibrate the data and convert to float value.
*/
int i8017H_AD_Polling(int iSlot,int iChannel,int iGain,int iDataCount,int *iDataPointer);






void i8017H_Array_HexToFloat(int *HexValue,float *FloatValue,int iSlot,int iGain,int iDataCount);

float i8017H_HexToFloat(int HexValue,int iSlot,int iGain);






/*
    Uses timer interrupt to sample A/D values of one chnnel.
    iSlot: 0~3 or 0~7
    iChannel: 0~7
    iGain:  0 ==>   +/- 10.0  V
            1 ==>   +/-  5.0  V
            2 ==>   +/-  2.5  V
            3 ==>   +/-  1.25 V
            4 ==>   +/- 20.0  mA
    iDataCount: 1~8192
    iSampleCLK: 200 ~ 50K (unit: Hz)
    iDataPointer: pointer to the data buffer
    
    Note: The data is an uncalibrated 16-bit value.
          You can use ARRAY_HEX_TO_FLOAT or HEX_TO_FLOAT
          to calibrate the data and convert to float value.
*/
int i8017H_AD_TimerINT(int iSlot,int iChannel,int iGain,int iDataCount,unsigned int iSampleCLK,int *iDataPointer);



    
/*
    Uses timer interrupt to sample A/D values of 8 chnnels.
    iSlot: 0~3 or 0~7
    iChannel: 0~7
    iGain:  0 ==>   +/- 10.0  V
            1 ==>   +/-  5.0  V
            2 ==>   +/-  2.5  V
            3 ==>   +/-  1.25 V
            4 ==>   +/- 20.0  mA
    iDataCount: 1~8192
    iSampleCLK: 200 ~ 50K (unit: Hz)
    iDataPointer: pointer to the data buffer
                  data in the buffer is ranged as
                  Diff Input Mode:
                    ch0 ch1 ch2...ch7 ch0 ch1....ch7 ch0 ch1 ...
                  Single end Input Mode:  
                    ch0 ch1 ch2...ch7 ch0 ch1....ch15 ch0 ch1 ...
    
    Note: The data is an uncalibrated 16-bit value.
          You can use ARRAY_HEX_TO_FLOAT or HEX_TO_FLOAT
          to calibrate the data and convert to float value.     
*/
int i8017H_AD_TimerINT_Scan(int iSlot,int iGain,int iDataCount,unsigned int iSampleCLK,int *iDataPointer);



void i8017H_Set_MUXReadyTime(int delayTime);
short i8017H_Get_MUXReadyTime(void);


#ifdef __cplusplus
 }
#endif
