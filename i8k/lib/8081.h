/*******************************************************************/
/*    i-8081 driver head file                                      */
/*                                                                 */
/*    Note: head file version is different to lib file version.    */
/*                                                                 */
/*******************************************************************/

/*
Ver 1.0.0 [2005,Apr,07] by Kevin
    First released version.
*/

extern unsigned int  i8081_ChannelMode[8][8];
extern unsigned int  i8081_Isolation[8][8];
extern unsigned int  i8081_XOR[8][8];
extern unsigned int  i8081_LPF_Status[8][8];

int i8081_GetLibVersion(void); 
    /*
	return: version number. 
	        for example: 0x100; = Rev:1.0.0
	*/
	
void i8081_GetLibDate(unsigned char *LibDate);	
	
int i8081_GetFirmwareVersion(int Slot);
    /*
    Slot: 0~7
	return: version of the firmware that downloaded in the Lattice Chip. 
	        for example: 1= Rev:1.0.0
	                     2= Rev:2.0.0
	*/ 
	
int i8081_Init(int Slot);
/*
    Slot: 0~7
    return:  0 --> OK
            -1 --> 8092 not found
            
    Initial State:
        Operation mode: CW/CCW
        Signal source: Isolated input
        XOR: 0(Low active)
        Low Pass Filter: Cut-off frequency=4M/3 Hz, enabled
    */

int i8081_SetChannelMode(int Slot, int Channel,int Mode);
    /*
    Set channel mode
    Slot: 0~7
    Channel: 0~7
    Mode: 0=Pulse/Dir
          1=Up/Down
          2=A-B phase
          3=Stop counting
    */
        
int  i8081_SetSignalSource(int Slot, int Channel, int Source);
    /*
    Slot: 0~7
    Channel: 0~7
    Source: 0 ==> Isolated input
            1 ==> TTL input
    
    Return: 0 ==> No error
            -1==> Error
            
    Note: The signal source is selected by JP0~JP7 on the board.
    */  
            
int i8081_SetLowPassFilter_Level(int Slot, int Level);
    /*
    Set Low Pass Filter Level for all channels
    Slot: 0~7
    Level: 0~7
            Level   Cut-off Freq      Work Freq
              0        4M/3
              1        2M/3
              2        1M/3
              3      500K/3
              4      250K/3
              5      125K/3
              6       63K/3
              7       32K/3
              8       16K/3
              9        8K/3
             10        4K/3
             11        2K/3
             12        1K/3
             13       500/3
             14       250/3
             15       120/3

    Return: 0 = No error                      
            -1= Error                         
    */     
    
int i8081_SetLowPassFilter_Status(int Slot, int Channel,int Status);
    /*
    Set channel mode
    Slot: 0~7
    Channel: 0~7
    Status: 0=disabled
            1=enabled
    */                          
    
int  i8081_SetXorRegister(int Slot, int Channel, int XorReg);
    /*
    Slot: 0~7
    Channel: 0~7
    XorReg: 0 ==> Low active  (signal from High to Low, count changed)
            1 ==> High acitve (signal from Low to High, count changed)
    
    Return: 0 ==> No error
            -1==> Error
    */
    
int i8081_SetTTL_Level(int Slot, float HiLevel, float LowLevel);
    /*
    Set TTL voltage level (0.0~4.3V)
    
    Slot: 0~7
    HiLevel: 0.0~4.3 (Unit:V) When voltage >= HiLevel, TTL=1.
    LowLevel: 0.0~4.3 (Unit:V) When voltage <=LowLevel, TTL=0.
    
    return:  0 ==> No error
            -1 ==> HiLevel over flow (>4.3)
            -2 ==> LowLevel over flow (>4.3)
            -3 ==> HiLevle and LowLevel over flow. 
           -10 ==> Wrong: HiLevel < LowLevel.
                   (Correct should be HiLevel >= LowLevel)
    */
    
int i8081_ClrCnt(int Slot, int Channel);
    /*
    Clear Counter
    Slot: 0~7
    Channel: 0~7
    
    return: 0 ==> No error
            -1==> Error
    */
    
int i8081_ClrCnt_All(int Slot);
    /*
    Clear all Counters
    Slot: 0~7
    
    return: 0 ==> No error
            -1==> Error
    */          
    
int i8081_AutoScan(void);
    /*
	auto sacn the i8081
	This function is used to update the counter values,
	and the low16-bit hardware counter can carry to high 16-bit
	software counter correctly.
	*/    
	
int i8081_ReadCntUp(int Slot, int Channel, unsigned long *Cnt32U);
    /*
    Read Up Counter
    Slot: 0~7
    Channel: 0~7
    Cnt32U = 32-bit Up Counter
    
    return: 0 ==> No error
            -1==> Error
    */	