/********************************
	i8093W Error Code definition	
*******************************/

#define OK 		 				0	
#define ID_ERROR 				-1
#define SLOT_OUT_RANGE			-2
#define CHANNEL_OUT_RANGE		-3
#define SELECT_CHANNEL_ERROR 	-4
#define MODE_ERROR				-5

#define		i8093_GetFirmwareVersion		i8093W_GetFirmwareVersion   			
#define		i8093_GetLibVersion     		i8093W_GetLibVersion        			
#define		i8093_GetLibDate        		i8093W_GetLibDate           			
#define		i8093_Init              		i8093W_Init                 			
#define		i8093_SetMode           		i8093W_SetMode              			
#define		i8093_GetMode           		i8093W_GetMode              			
#define		i8093_GetLineStatus     		i8093W_GetLineStatus        			
#define		i8093_GetIndex          		i8093W_GetIndex             			
#define		i8093_SetXOR            		i8093W_SetXOR               			
#define		i8093_GetXOR            		i8093W_GetXOR               			
#define		i8093_Read32BitEncoder          i8093W_Read32BitEncoder     
#define		i8093_ReadFreq					i8093W_ReadFreq       		
#define		i8093_ResetEncoder      		i8093W_ResetEncoder         			
#define     i8093_SetPresetValue            i8093W_SetPresetValue		

/********************************
	i8093_GetFirmwareVersion can get the Lattice version 
	return Lattice version	
*******************************/
short i8093W_GetFirmwareVersion(int slot);


/********************************
	i8093W_Get_LibVersion can get the version i8093W Lib
	return Lib version	
*******************************/
short i8093W_GetLibVersion(void);  
 


/********************************
	i8093W_Get_LibDate can get the build date of i-8080 Lib
	1. dateStr the date string that build this lib	
*******************************/
void i8093W_GetLibDate( char *LibDate); 

/********************************
	i8093W_Init will check the basic condition of i-8093
	1. slot 0 ~ 7 
	return value: please refer to i8093W Error Code definition		
*******************************/
int i8093W_Init(int slot);



/********************************
	i8093W_SetMode can the Encoder mode of 8093
	1. slot 0 ~ 7 
	2. ch 0 ~ 2	
	3. mode 1: cw/ccw ; mode 2: pulse/dir ; mode 3: A/B phase
	return value: please refer to i8093W Error Code definition	
*******************************/
int i8093W_SetMode(int slot, int ch, int Mode);



/********************************
	i8093W_GetMode can the Encoder mode of 8093
	1. slot 0 ~ 7 
	2. ch 0 ~ 2	
	3. mode 1: cw/ccw ; mode 2: pulse/dir ; mode 3: A/B phase
	return value: please refer to i8093W Error Code definition		
*******************************/
int i8093W_GetMode(int slot, int ch, int* Mode);



/********************************
	i8093W_GetLineStatus can check the line status of A , B , C Pin status of 8093
	1. slot 0 ~ 7 
	2. ch 0 ~ 2	
	3: A , B , C Pin status 0:not activated 1:activated
	return value: please refer to i8093W Error Code definition		
*******************************/
int i8093W_GetLineStatus(int slot, int ch, int* A_Status,int* B_Status,int* C_Status);


/********************************
	i8093W_GetIndex can check the Index status of C Pin of 8093
	1. slot 0 ~ 7 
	2. ch 0 ~ 2	
	3. index 0:not activated 1:activated
	return value: please refer to i8093W Error Code definition		
*******************************/
int i8093W_GetIndex(int slot, int ch,int* index);



/********************************
	i8093W_SetXOR can set the Xor Pin status of i-8093
	1. slot 0 ~ 7 
	2. ch 0 ~ 2	
	3. Xor 0:not activated 1:activated
	return value: please refer to i8093W Error Code definition		
*******************************/
int i8093W_SetXOR(int slot, int ch, int Xor);


/********************************
	i8093W_GetXOR can get the Xor Pin status of i-8093
	1. slot 0 ~ 7 
	2. ch 0 ~ 2	
	3. Xor 0:not activated 1:activated
	return value: please refer to i8093W Error Code definition		
*******************************/
int i8093W_GetXOR(int slot, int ch, int* Xor);




/*********************************
	i8093W_Read32BitEncoder(int slot,int ch, long* EnCodeVal) will get hardware 24 Bit value + 8 bit software value and get total 32 bit Encoder value , 
	1. slot 0 ~ 7 
	2. ch 0 ~ 2		
	3. EnCode32	 
	 positive direction 0 + 1 ===> 0x00000001 +++++ ===> 0xffffffff +1 ===> 0	 
	 negative direction 0 -1  ===> 0xffffffff ----- ===> 0x00000000 -1 ===>  0	
*********************************/
int i8093W_Read32BitEncoder(int slot,int ch, long* EnCode32);



/********************************
	i8093W_ResetEncoder can will check the basic condition of i-8093
	1. slot 0 ~ 7 
	2. ch 0 ~ 2		
	return value: please refer to i8093W Error Code definition		
*******************************/
int i8093W_ResetEncoder(int slot, int ch);



/********************************
 i8093W_SetPresetValue can set the preset value of i-8093
 1. slot 0 ~ 7 
 2. ch 0 ~ 2 
 3. PresetVal 0 ~ 0xffffffff
 return value: please refer to i8093W Error Code definition 
 Note1: preset value is supported at lib version 0x1007 or above 
*******************************/
int i8093W_SetPresetValue(int slot, int ch, long presetVal);



/********************************
 8093W firmware must be 3 or above to support i8093W_ReadFreq function
 i8093W_ReadFreq read the frequency value of i-8093
 1. slot 0 ~ 7 
 2. ch 0 ~ 2  
 3. Freq  readback frequency of i-8093,
 Note1: if there is no encoder input, the frequency will be 0.093 not 0
 
 return value: please refer to i8093W Error Code definition  
*******************************/
int i8093W_ReadFreq(int slot,int ch,float* freq);

/********************************
 8093W firmware must be 4 or above to support i8093W_SetIndexLatchStatus function
 i8093W_SetIndexLatchStatus to disable/enable the index latch function of i-8093

 1. slot 0 ~ 7 
 2. ch 0 ~ 2  
 3. ifEnableLatch  to enable or disable the index latch function of i-8093,
 Note1: normally we can use i8093W_GetIndex to read the index (z ) status, but sometimes it is not easy 
 to let the encoder to match the right position, so 8093W add this function to latch the z index status 
 Note2: if enable the latch index function and the z index latched, the encoder value will also be latched 
 at the position where z index latched point, so we can adjust the encoder position to the latched point.
 Note3: disable the index latch function, i8093W_Read32BitEncoder can read the normal encoder value as usual.
 
 return value: please refer to i8093W Error Code definition  
*******************************/
int i8093W_SetIndexLatchStatus(int slot, int ch, int ifEnableLatch);




/********************************
 8093W firmware must be 4 or above to support i8093W_ClearLatchedIndex function
 i8093W_ClearLatchedIndex clear Index Latch status of i-8093
 1. slot 0 ~ 7 
 2. ch 0 ~ 2  
 3. Freq  readback frequency of i-8093,
 Note1: read frequency function supported by firmware version 3 or later version.
 Note2: if there is no encoder input, the frequency will be 0.093 not 0
 
 return value: please refer to i8093W Error Code definition  
*******************************/
int i8093W_ClearLatchedIndex(int slot, int ch);





/********************************
 8093W firmware must be 4 or above to support i8093W_GetIndexLatchStatus function
 i8093W_GetIndexLatchStatus read the index latched status and enable latch configuration of i-8093
 1. slot 0 ~ 7 
 2. ch 0 ~ 2  
 
 3. *latchedStatus  check z index latched or not
 4. *ifEnableLatch  readback enable latched  of i-8093,
 Note1: read frequency function supported by firmware version 3 or later version.
 Note2: if there is no encoder input, the frequency will be 0.093 not 0
 
 return value: please refer to i8093W Error Code definition  
*******************************/
int i8093W_GetIndexLatchStatus(int slot, int ch, int* latchedStatus, int* ifEnableLatch);