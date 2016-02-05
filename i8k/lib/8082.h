/*******************************************************************/
/*    i-8082 driver head file                                      */
/*                                                                 */
/*    Note: head file version is different to lib file version.    */
/*                                                                 */
/*******************************************************************/

/*
Ver 1.0.0 [2005,Dec,26] by Kevin
    First released version.
*/

int i8082_GetLibVersion(void); 
    /*
	return: version number. 
	        for example: 0x100; = Rev:1.0.0
	*/
	
void i8082_GetLibDate(unsigned char *LibDate);	

int i8082_GetFirmwareVersion(int Slot);
    /*
    Slot: 0~7
	return: version of the firmware that downloaded in the Lattice Chip. 
	        for example: 1= Rev:1.0.0
	                     2= Rev:2.0.0
	*/ 
 
	
int i8082_Init(int iSlot);
    /*
    iSlot: 0~7        
    return:  0 --> OK
            -1 --> 8082 not found
            
    Default settings:
        Signal Source= Isolated input  
    */
    
int i8082_ReadFreq(int Slot, int Channel, float *Freq);
    /*
    Read frequency value
    Slot: 0~7
    Channel: 0~7
    Freq = Frequency value. Unit: Hz
    
    return:  0 ==> Frequency is not updated (the value is old) 
            >0 ==> Frequency is updated (the value is new)
    */
    
int i8082_AutoScan(void);
    /*
	auto sacn the i8082
	This function is used to update the time tick values of 8082
	to avoid the time tick overflow.
	It updates one slot each time.
	
	Refer to the 'max. time interval to update data' comment
	in function i8082_Init. The period to update every channel 
	of each 8082 must be less than the max. interval listed 
	in i8082_Init.
	
	This function is not necessary. 
	1. If your program calls i8082_ReadFreq at period less than the 
	   max. time interval, then you don't need to call the function.
	2. If your program calls i8082_ReadFreq at a random period,
	   then you must call i8082_AutoScan.
	*/    
