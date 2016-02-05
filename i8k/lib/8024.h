/************************/
/*  head file for 8024  */
/************************/
/*
Ver 2.0.0 [7,Oct,2004] by Bill
    1.Remove smoe function that useless for user.
      Such as:  
                void write_93c46(int slot,int addr,int data);
                int read_93c46(int slot,int addr);           
                void erase_93c46(int slot,int addr);         
      
              
    2.Change functions name:                               
          I8024_VoltageHexOut ==> I8024_VoltageOut_Hex
          I8024_CurrentHexOut ==> I8024_CurrentOut_Hex
    
      
*/    

/************************************************************************/
/*  	[8024 D/A chip specific]			                            */
/*      Voltage Output range:           -10.0V     0V        +10.0V     */
/*          (1)float format             -10.0V   0.0V        +10.0V     */
/*          (2)Hex format              -32768      0         32767      */
/*          (3)interger format(mv)     -10000mV    0mV      +10000mV    */
/*                                                                      */
/*      CurrentOutput range:                     0.0mA       +20.0mA    */
/*          (1)float format                      0.0mA       +20.0mA    */
/*          (2)Hex format                          0         32768      */
/*          (3)interger format(0.001mA)            0         20000mA    */
/************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern int Vmax24[8][4],Vmin24[8][4],Imax24[8][4],Imin24[8][4];
void I8024_Initial(int Slot);
//Voltage
void I8024_VoltageOut(int slot,int ch,float fData);
//fData: -10.0 ~ +10.0

void I8024_VoltageOut_Hex(int slot,int ch,int hData);
//iData: -32768  ~  +32767

void I8024_VoltageOut_mV(int slot,int ch,int iData);
//iData:  -10000 ~  +10000


//Current
void I8024_CurrentOut(int slot,int ch,float fData);
//fData:  0.0 ~ +20.0 
    
void I8024_CurrentOut_Hex(int slot,int ch,int hData);
//iData: 0  ~  +32767
  
void I8024_CurrentOut_0001mA(int slot,int ch,int iData);
//iData: 0  ~  +20000


#ifdef __cplusplus
 }
#endif

