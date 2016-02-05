/***********************************************************************
head file for 8048 

Library:
            8000l.lib [08/06/2004] 
Ver 1.0.0 [  Feb,2005] by Bill
   

***********************************************************************/
extern unsigned long i8048_RisingEventCount[4][8]; 
extern unsigned long i8048_FallingEventCount[4][8];  

void i8048_Init(int Slot);           
//slot:(0~3)               

void i8048_UnFreezeCPU(int Slot);
//Send EOI to 8259==>EOI: End of interrupt

void i8048_UnFreezeINT(int Slot);
//Clear Gi and all channels are latched by 8259                    

void i8048_Set_RisingReg(int Slot,int Channel,int Enable);       
//Set Rising enable register 

void i8048_Set_FallingReg(int Slot,int Channel,int Enable);
//Set falling enable register                              

void i8048_InstallISR(int Slot,unsigned long *ISR_Function,int Priority);

int i8048_Read_RisingEvent(int Slot,int Channel);               
//Read rising interrupt status

int i8048_Read_FallingEvent(int Slot,int Channel);              
//Read falling interrupt status

int i8048_Read_RisingReg(int Slot,int Channel);                 
//Read rising enable register

int i8048_Read_FallingReg(int Slot,int Channel);                
//Read falling enable register

int i8048_DI_ALL(int Slot);                       
/*
    isolated:
    +---------------------------+--------------+--------------+-    
    |JPn selected isolated input|Digital Input |     LED      |     
    +---------------------------+--------------+--------------+-    
    |Input is OPEN              |     High     |     OFF      |     
    +---------------------------+--------------+--------------+     
    |Input is 3.5 ~ 30V         |     Low      |     ON       |     
    +---------------------------+--------------+--------------+     
    |Input is 0 ~ 1V            |     High     |     OFF      |     
    +---------------------------+--------------+--------------+     
    
    TTL:
    +---------------------------+--------------+--------------+-        
    |JPn selected TTL input     |Digital Input |     LED      |         
    +---------------------------+--------------+--------------+-        
    |Input is OPEN              |     High     |     OFF      |         
    +---------------------------+--------------+--------------+         
    |Input is 0 ~ 0.8V          |     Low      |     ON       |         
    +---------------------------+--------------+--------------+         
    |Input is 2 ~ 5V            |     High     |     OFF      |         
    +---------------------------+--------------+--------------+         

*/
int i8048_DI_Ch(int Slot,int Channel);
//Read single DI channel status
