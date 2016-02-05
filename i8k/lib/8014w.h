
#ifdef __cplusplus
extern "C" {
#endif

#define NoError 					0
#define ID_ERROR					-1
#define SLOT_ERROR					-2
#define CHANNEL_ERROR				-3
#define GAIN_ERROR					-4


#define FIFO_EMPTY      -5
#define FIFO_LATCHED    -6
#define FIFO_OVERFLOW   -7
#define TX_NOTREADY     -8
#define FIFO_ISR_ERROR  -9
#define MAX_FIFO		4096



short i8014W_Init(int slot);

short i8014W_GetLibVersion(void);
void i8014W_GetLibDate(char libDate[]);


short i8014W_GetFirmwareVer_L1(int slot);

short i8014W_GetFirmwareVer_L2(int slot);

short i8014W_GetSingleEndJumper(int slot);


void i8014W_ReadGainOffset(int slot,short gain,unsigned short* gainValue, short* offsetValue);




short i8014W_ReadAIHex(int slot,short ch,short gain, short* hVal);

short i8014W_ReadAI(int slot,short ch,short gain, float* fVal);


void i8014W_ClearFIFO(int slot);
void  i8014W_UnLockFIFO(int slot);

void i8014W_ConfigMagicScan(int slot,short chArr[],short gainArr[],short scanChCount,float sampleRate,short scanMode,short triggerSource,short triggerState, float* realSampleRate);
short i8014W_StartMagicScan(int slot);
short i8014W_StopMagicScan(int slot);



short i8014W_ReadFIFO(int slot, short hexData[], short readCount,short* dataCountFromFIFO);// <=== get data from FIFO ,long* dataCnt

short i8014W_ReadFIFO_BlockMode(int slot, short hexData[], long readCount ,long* dataCountFromFIFO);

short i8014W_ReadFIFO_InISR(int slot, short hexData[], short triggerLevel,short* dataCountFromFIFO);



// Do need this function 
//short i8014W_IsFIFO(int slot, long* DataCount);

void i8014W_CalibrateDataHex(int slot, short iGain,short dataFromFIFO, short* calibratedAI);

void i8014W_CalibrateData(int slot, short iGain,short dataFromFIFO, float* calibratedAI);




short i8014W_InstallMagicScanISR(int slot,void (*isr)(int slot),short triggerLevel); //<=== give a callback function for interrupt
short i8014W_UnInstallMagicScanISR(int slot); //<=== give a callback function for interrupt 
void i8014W_ClearInt(int slot);

#ifdef __cplusplus
 }
#endif

