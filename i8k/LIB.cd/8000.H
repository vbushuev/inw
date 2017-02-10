#ifndef __MINIOS7__
#define __MINIOS7__

typedef unsigned int  uint;
typedef unsigned int  WORD;
typedef unsigned char uchar;
typedef unsigned char BYTE;
typedef unsigned long ulong;
typedef unsigned long DWORD;

#ifdef __TURBOC__
  #if (__TURBOC__ < 0x0300)
    #define inpw   inport
    #define outpw  outport
  #endif
#endif

#define    NoError	    0
#define    InitPinIsOpen    0
#define    InitPinIsNotopen 1
#define    QueueIsEmpty     0
#define    QueueIsNotEmpty  1
#define    PortError	   -1
#define    DataError	   -2
#define    ParityError	   -3
#define    StopError	   -4
#define    TimeOut	   -5
#define    QueueEmpty	   -6
#define    QueueOverflow   -7
#define    PosError	   -8
#define    AddrError	   -9
#define    BlockError	   -10
#define    WriteError	   -11
#define    SegmentError    -12
#define    BaudRateError   -13
#define    CheckSumError   -14
#define    ChannelError    -15
#define    BaudrateError   -16
#define    TriggerLevelError   -17
#define    DateError    -18
#define    TimeError    -19
#define	   OutOfMemory	-20
#define    TimeIsUp        1

#ifdef __cplusplus
extern "C" {
#endif

/* Fro library version, date,...*/
void InitLib(void);
void GetLibDate(char *date);
unsigned GetLibVersion(void);

/* FOR WDT */
void EnableWDT(void);
void DisableWDT(void);
void RefreshWDT(void);

/* FOR INIT* pin */
int ReadInitPin(void);

/* For SCLK pin */
void ClockHigh(void);
void ClockHighLow(void);
void ClockLow(void);

/* For LED (L1 of S-MMI)*/
void LedOn(void);
void LedOff(void);
void LedToggle(void);

/* For L1/L2/L3 on I-8000 S-MMI */
#define LED_OFF     0
#define LED_ON      1
#define LED_TOGGLE  2
void SetLedL1(int mode);
void SetLedL2(int mode);
void SetLedL3(int mode);

/* FOR 5* 7-segments LED */
extern unsigned char ShowData[19];
void pascal Show5DigitLed(int pos, int data);
void pascal Show5DigitLedWithDot(int pos, int data);
void Init5DigitLed(void);
void pascal Show5DigitLedSeg(int pos, unsigned char data);
void pascal Set5DigitLedTestMode(int mode);
void pascal Set5DigitLedIntensity(int mode);
void Disable5DigitLed(void);
void Enable5DigitLed(void);

/* For STDIO */
void Putch(int data);
void Puts(char *str);
int Getch(void);
int Gets(char *str);
int Kbhit(void);
int LineInput(char *buf,int maxlen);
void ResetScanBuffer(void);
void SetScanBuffer(unsigned char *buf,int len);
int Scanf(char *fmt, ...); /* for TC/BC only */
int Print(const char *fmt, ...);
int _Printf(const char *fmt, ...); /* for TC/BC only */
int UngetchI(int key);
int Ungetch(int key);

/* For RTC/NVRAM */
void GetTime(int *hour,int *minute,int *sec);
void GetDate(int *year,int *month,int *day);
int GetWeekDay(void);
int SetDate(int year,int month,int day);
int SetTime(int hour,int minute,int sec);
int ReadNVRAM(int addr);
int WriteNVRAM(int addr, int data);
void ReadRTC(int addr, int *data);
void WriteRTC(int addr, int data);

/* for Timer */
extern const unsigned long far *TimeTicks;

/* For old version EEPROM functions compatible */
#define WriteEEP	EE_RandomWrite
#define ReadEEP		EE_RandomRead
#define EnableEEP	EE_WriteEnable
#define ProtectEEP	EE_WriteProtect

/* for EEPROM(24LC16/1024)*/
void EE_WriteProtect(void);
void EE_WriteEnable(void);
/* for 24LC16 use */
unsigned char EE_RandomRead(int Block,unsigned Addr);
unsigned char EE_ReadNext(int Block);
int EE_MultiRead(int StartBlock,unsigned StartAddr,int no,char *databuf);
int EE_RandomWrite(int Block,unsigned Addr,int Data);
int EE_MultiWrite(int Block,unsigned Addr,int no,char *Data);
int EE_MultiWrite_A(int Block,unsigned Addr,unsigned no,char *Data);

/* for 24LC1024 (only when 24LC16 is replaced by 24LC1024 can be used)*/
unsigned char EE1024_RandomRead(int Block,unsigned Addr);
unsigned char EE1024_ReadNext(int Block);
int EE1024_MultiRead(int StartBlock,unsigned StartAddr,int no,char *databuf);
int EE1024_RandomWrite(int Block,unsigned Addr,int Data);
int EE1024_MultiWrite(int Block,unsigned Addr,int no,char *Data);

/* for 24LC16 or 24LC1024 use, NEED call InitEEPROM() first.*/
extern int EepType; /* 1024:24LC1024,16:24LC16; */
extern int EepBlockOffset;
extern unsigned EepAddrOffset;
void InitEEPROM(void);
extern unsigned char (*EE1_RandomRead)(int Block,unsigned Addr);
extern unsigned char (*EE1_ReadNext)(int Block);
extern int (*EE1_MultiRead)(int StartBlock,unsigned StartAddr,int no,char *databuf);
extern int (*EE1_RandomWrite)(int Block,unsigned Addr,int Data);
extern int (*EE1_MultiWrite)(int Block,unsigned Addr,int no,char *Data);

/* for IP/MASK/GATEWAY/MAC */
void GetIp(unsigned char *ip);
#define GetMac	GetEid
void GetEid(unsigned char *id);
void GetMask(unsigned char *mask);
void GetGateway(unsigned char *gate);
void SetIp(unsigned char *ip);
#define SetMac	SetEid
void SetEid(unsigned char *id);
void SetMask(unsigned char *mask);
void SetGateway(unsigned char *gate);

/* for system */
extern unsigned long far *IntVect;
int IsMiniOS7(void);
int Is8000(void);
int IsResetByPowerOn(void);
int IsResetByWatchDogTimer(void);

/* for FLASH MEMORY */
int FlashReadId(void);
int FlashErase(unsigned int FlashSeg);
int FlashWrite(unsigned int seg, unsigned int offset, char data);

#define FlashRead FlashReadB
unsigned char FlashReadB(unsigned seg, unsigned offset);
unsigned FlashReadI(unsigned seg, unsigned offset);
unsigned long FlashReadL(unsigned seg, unsigned offset);
void far *_MK_FP_(unsigned s,unsigned off);

/* Timer functions */
int TimerOpen(void);
int TimerClose(void);
void TimerResetValue(void);
unsigned long TimerReadValue(void);
int StopWatchReset(int channel);
int StopWatchStart(int channel);
int StopWatchStop(int channel);
int StopWatchPause(int channel);
int StopWatchContinue(int channel);
int StopWatchReadValue(int channel,unsigned long *value);
int CountDownTimerStart(int channel,unsigned long count);
int CountDownTimerReadValue(int channel,unsigned long *value);
void InstallUserTimer(void (*fun)(void));
void InstallUserTimer1C(void (*fun)(void));

/* StopWatch [計時碼表] */

#ifndef _T_STOPWATCH_
#define _T_STOPWATCH_
typedef struct {
 ulong ulStart,ulPauseTime;
 uint  uMode;  /* 0: pause, 1:run(start) */
} STOPWATCH;
#endif

/* CountDown Timer[倒數計時] */
#ifndef _T_COUNTDOWNTIMER_
#define _T_COUNTDOWNTIMER_
typedef struct {
 ulong ulTime,ulStartTime,ulPauseTime;
 uint  uMode;  /* 0: pause, 1:run(start) */
} COUNTDOWNTIMER;
#endif

void T_StopWatchStart(STOPWATCH *sw);
ulong T_StopWatchGetTime(STOPWATCH *sw);
void T_StopWatchPause(STOPWATCH *sw);
void T_StopWatchContinue(STOPWATCH *sw);

void T_CountDownTimerStart(COUNTDOWNTIMER *cdt,ulong timems);
void T_CountDownTimerPause(COUNTDOWNTIMER *cdt);
void T_CountDownTimerContinue(COUNTDOWNTIMER *cdt);
int T_CountDownTimerIsTimeUp(COUNTDOWNTIMER *cdt);
ulong T_CountDownTimerGetTimeLeft(COUNTDOWNTIMER *cdt);

/* Timer functions II */
void T2_UpdateCurrentTimeTicks(void); /* every loop must call T2_UpdateCurrentTimeTicks() to get new time.*/
void T2_StopWatchStart(STOPWATCH *sw);
ulong T2_StopWatchGetTime(STOPWATCH *sw);
void T2_StopWatchPause(STOPWATCH *sw);
void T2_StopWatchContinue(STOPWATCH *sw);

void T2_CountDownTimerStart(COUNTDOWNTIMER *cdt,ulong timems);
void T2_CountDownTimerPause(COUNTDOWNTIMER *cdt);
void T2_CountDownTimerContinue(COUNTDOWNTIMER *cdt);
int T2_CountDownTimerIsTimeUp(COUNTDOWNTIMER *cdt);
ulong T2_CountDownTimerGetTimeLeft(COUNTDOWNTIMER *cdt);

extern const unsigned long far *TimeTicks;
void Delay(unsigned ms); /* delay unit is ms, use CPU Timer 1. */
void Delay_1(unsigned ms); /* delay unit is 0.1 ms ,use CPU Timer 1.*/
void Delay_2(unsigned ms); /* delay unit is 0.01 ms ,use CPU Timer 1.*/
void DelayMs(unsigned t);/* delay unit is ms, use system timeticks. */

/* for MiniOS7 FLASH file system */
#ifndef __FILE_DATA__
#define __FILE_DATA__
typedef struct  {
  unsigned mark;   /* 0x7188 -> is file */
  unsigned char fname[12];
  unsigned char year;
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char minute;
  unsigned char sec;
  unsigned long size;
  char far *addr;
  unsigned CRC;
  unsigned CRC32;
} FILE_DATA;
#endif

#ifndef _DISK_AB_
#define _DISK_AB_

typedef struct {
  unsigned sizeA:3;
  unsigned sizeB:3;
  unsigned sizeC:3;
  unsigned sum:7;
} SIZE_AB;
#endif

extern SIZE_AB SizeAB;
extern FILE_DATA far *fdata;
extern unsigned DiskAStartSeg,DiskBStartSeg;

#define DISKA	0
#define DISKB	1

/* int GetFileNo(void); */
#define GetFileNo()	GetFileNo_AB(DISKA)

/* int GetFileName(int no,char *fname); */
#define GetFileName(no,fname)	GetFileName_AB(DISKA,no,fname)

/* FILE_DATA far * GetFileInfoByNo(int no) */
#define GetFileInfoByNo(no)	GetFileInfoByNo_AB(DISKA,no)

/* FILE_DATA far * GetFileInfoByName(char *fname) */
#define GetFileInfoByName(fname)	GetFileInfoByName_AB(DISKA,fname)

/* char far * GetFilePositionByNo(int no) */
#define GetFilePositionByNo(no)	GetFilePositionByNo_AB(DISKA,no)

/* char far * GetFilePositionByName(char *fname) */
#define GetFilePositionByName(fname)	GetFilePositionByName(DISKA,fname)

int GetFileNo_AB(int disk);
int GetFileName_AB(int disk,int no,char *fname);
FILE_DATA far * GetFileInfoByNo_AB(int disk,int no);
FILE_DATA far *GetFileInfoByName_AB(int disk,char *fname);
char far * GetFilePositionByNo_AB(int disk,int no);
char far * GetFilePositionByName_AB(int disk,char *fname);

void far *AddFarPtrLong(void far * ptr1,unsigned long size);
void ReadSizeAB(void);
void MoveToStartAddr(int disk);

/* for COM0 (used by 87K module) */
/* WITHOUT CTS & RTS control */
#define	COM1	0
#define	COM2	1
#define	COM3	2
#define	COM4	3

#define FLOW_CONTROL_DISABLE	0
#define FLOW_CONTROL_ENABLE	1
#define FLOW_CONTROL_AUTO_BY_HW 2
#define FLOW_CONTROL_AUTO_BY_SW	3

#define	ClearTxBuffer0		ClearTxBuffer_0
#define GetTxBufferFreeSize0	GetTxBufferFreeSize_0
#define PushDataToCom0		PushDataToCom_0

#define CheckInputBufSize0	CheckInputBufSize_0
#define InstallCom0		InstallCom_0
#define RestoreCom0		RestoreCom_0
#define SetBaudrate0		SetBaudrate_0
#define SetDataFormat0		SetDataFormat_0
#define ClearCom0		ClearCom_0
#define ClearCom0_DMA		ClearCom_DMA_0
#define DataSizeInCom0		DataSizeInCom_0
#define IsCom0			IsCom_0
#define IsCom0OutBufEmpty	IsComOutBufEmpty_0
#define ReadCom0		ReadCom_0
#define ToCom0Bufn		ToComBufn_0
#define ToCom0Str		ToComStr_0
#define SetCom0Timeout		SetComTimeout_0
#define ToCom0			ToCom_0
#define IsTxBufEmpty0		IsTxBufEmpty_0
#define WaitTransmitOver0	WaitTransmitOver_0
#define ReadCom0n		ReadComn_0
#define printCom0		printCom_0
#define SetBreakMode0		SetBreakMode_0
#define SendBreak0		SendBreak_0
#define IsDetectBreak0		IsDetectBreak_0

void ClearTxBuffer_0(void);
int GetTxBufferFreeSize_0(void);
int PushDataToCom_0(int data);
void CheckInputBufSize_0(void);
int InstallCom_0(unsigned long baud, int data, int parity,int stop);
int RestoreCom_0(void);
int SetBaudrate_0(unsigned long baud);
int SetDataFormat_0(int databit,int parity,int stopbit);
int ClearCom_0(void);
int ClearCom_DMA_0(void);
int DataSizeInCom_0(void);
int IsCom_0(void);
int IsComOutBufEmpty_0(void);
int ReadCom_0(void);
int ToComBufn_0(char *buf,int no);
int ToComStr_0(char *str);
void SetComTimeout_0(unsigned t);
int ToCom_0(int data);
int IsTxBufEmpty_0(void);
int WaitTransmitOver_0(void);
int ReadComn_0(unsigned char *buf,int no);
int printCom_0(char *fmt,...);

int DataSizeInCom_DMA_0(void);
int ReadComn_DMA_0(unsigned char *buf,int maxsize);
int InstallCom_DMA_0(unsigned long baud, int data, int parity,int stop);
int ClearCom_DMA_0(void);
int IsCom_DMA_0(void);
int DataSizeInCom_DMA_0(void);
int ReadCom_DMA_0(void);

void SetBreakMode_0(int mode);
void SendBreak_0(unsigned TimeMs);
int IsDetectBreak_0(void);
void SetComPortBufferSize_0(int in_size,int out_size);

/* for COM0 use DMA */
unsigned Com0GetDataSize(void);
void Com0SetInputBuf(char far *ptr,unsigned cnt);
void Com0SendCmd(char far *cmd,unsigned length);
void OpenCom0UseDMA(void);
void CloseCom0UseDMA(void);

/* for COM1 */
/* WITHOUT CTS & RTS control */

#define	ClearTxBuffer1		ClearTxBuffer_1
#define GetTxBufferFreeSize1	GetTxBufferFreeSize_1
#define PushDataToCom1		PushDataToCom_1

#define CheckInputBufSize1	CheckInputBufSize_1
#define InstallCom1		InstallCom_1
#define RestoreCom1		RestoreCom_1
#define SetBaudrate1		SetBaudrate_1
#define SetDataFormat1		SetDataFormat_1
#define ClearCom1		ClearCom_1
#define ClearCom1_DMA		ClearCom_DMA_1
#define DataSizeInCom1		DataSizeInCom_1
#define IsCom1			IsCom_1
#define IsCom1OutBufEmpty	IsComOutBufEmpty_1
#define ReadCom1		ReadCom_1
#define ToCom1Bufn		ToComBufn_1
#define ToCom1Str		ToComStr_1
#define SetCom1Timeout		SetComTimeout_1
#define ToCom1			ToCom_1
#define IsTxBufEmpty1		IsTxBufEmpty_1
#define WaitTransmitOver1	WaitTransmitOver_1
#define ReadCom1n		ReadComn_1
#define printCom1		printCom_1
#define SetBreakMode1		SetBreakMode_1
#define SendBreak1		SendBreak_1
#define IsDetectBreak1		IsDetectBreak_1

void ClearTxBuffer_1(void);
int GetTxBufferFreeSize_1(void);
int PushDataToCom_1(int data);
void CheckInputBufSize_1(void);
int InstallCom_1(unsigned long baud, int data, int parity,int stop);
int RestoreCom_1(void);
int SetBaudrate_1(unsigned long baud);
int SetDataFormat_1(int databit,int parity,int stopbit);
int ClearCom_1(void);
int ClearCom_DMA_1(void);
int DataSizeInCom_1(void);
int IsCom_1(void);
int IsComOutBufEmpty_1(void);
int ReadCom_1(void);
int ToComBufn_1(char *buf,int no);
int ToComStr_1(char *str);
void SetComTimeout_1(unsigned t);
int ToCom_1(int data);
int IsTxBufEmpty_1(void);
int WaitTransmitOver_1(void);
int ReadComn_1(unsigned char *buf,int no);
int printCom_1(char *fmt,...);

int DataSizeInCom_DMA_1(void);
int ReadComn_DMA_1(unsigned char *buf,int maxsize);
int InstallCom_DMA_1(unsigned long baud, int data, int parity,int stop);
int ClearCom_DMA_1(void);
int IsCom_DMA_1(void);
int DataSizeInCom_DMA_1(void);
int ReadCom_DMA_1(void);

void SetBreakMode_1(int mode);
void SendBreak_1(unsigned TimeMs);
int IsDetectBreak_1(void);
void SetComPortBufferSize_1(int in_size,int out_size);

/*
 Functions for COM2
*/
#define InstallCom2	InstallCom_2
#define RestoreCom2	RestoreCom_2
#define IsCom2		IsCom_2
#define ToCom2		ToCom_2
#define ToCom2Str	ToComStr_2
#define ToCom2Bufn	ToComBufn_2
#define printCom2	printCom_2
#define ClearTxBuffer2	ClearTxBuffer_2
#define SetCom2FifoTriggerLevel		SetComFifoTriggerLevel_2
#define SetBaudrate2	SetBaudrate_2
#define ReadCom2	ReadCom_2
#define ClearCom2	ClearCom_2
#define DataSizeInCom2  DataSizeInCom_2
#define WaitTransmitOver2	WaitTransmitOver_2
#define IsTxBufEmpty2		IsTxBufEmpty_2
#define IsCom2OutBufEmpty	IsComOutBufEmpty_2
#define ReadCom2n		ReadComn_2
#define SetDataFormat2		SetDataFormat_2
#define SetBreakMode2		SetBreakMode_2
#define SendBreak2		SendBreak_2
#define IsDetectBreak2		IsDetectBreak_2

int InstallCom_2(unsigned long baud, int data, int parity, int stop);
int RestoreCom_2(void);
int IsCom_2(void);
int ToCom_2(int data);
int ToComStr_2(char *str);
int ToComBufn_2(char *buf,int no);
int printCom_2(char *fmt,...);
void ClearTxBuffer_2(void);
int SetComFifoTriggerLevel_2(int level);
int SetBaudrate_2(unsigned long baud);
int ReadCom_2(void);
int ClearCom_2(void);
int DataSizeInCom_2(void);
int WaitTransmitOver_2(void);
int IsTxBufEmpty_2(void);
int IsComOutBufEmpty_2(void);
int SetDataFormat_2(int databit,int parity,int stopbit);
int ReadComn_2(unsigned char *buf,int n);
void SendBreak_2(unsigned timems);
void SetBreakMode_2(int mode);
int IsDetectBreak_2(void);

/*
 Functions for COM3
*/
#define InstallCom3	InstallCom_3
#define RestoreCom3	RestoreCom_3
#define IsCom3		IsCom_3
#define ToCom3		ToCom_3
#define ToCom3Str	ToComStr_3
#define ToCom3Bufn	ToComBufn_3
#define printCom3	printCom_3
#define ClearTxBuffer3	ClearTxBuffer_3
#define SetCom3FifoTriggerLevel		SetComFifoTriggerLevel_3
#define SetBaudrate3	SetBaudrate_3
#define ReadCom3	ReadCom_3
#define ClearCom3	ClearCom_3
#define DataSizeInCom3  DataSizeInCom_3
#define WaitTransmitOver3	WaitTransmitOver_3
#define IsTxBufEmpty3		IsTxBufEmpty_3
#define IsCom3OutBufEmpty	IsComOutBufEmpty_3
#define ReadCom3n		ReadComn_3
#define SetDataFormat3		SetDataFormat_3
#define SetRtsActive3		SetRtsActive_3
#define SetRtsInactive3		SetRtsInactive_3
#define GetCtsStatus3		GetCtsStatus_3
#define SetBreakMode3		SetBreakMode_3
#define SendBreak3		SendBreak_3
#define IsDetectBreak3		IsDetectBreak_3

int InstallCom_3(unsigned long baud, int data, int parity, int stop);
int RestoreCom_3(void);
int IsCom_3(void);
int ToCom_3(int data);
int ToComStr_3(char *str);
int ToComBufn_3(char *buf,int no);
int printCom_3(char *fmt,...);
void ClearTxBuffer_3(void);
int SetComFifoTriggerLevel_3(int level);
int SetBaudrate_3(unsigned long baud);
int ReadCom_3(void);
int ClearCom_3(void);
int DataSizeInCom_3(void);
int WaitTransmitOver_3(void);
int IsTxBufEmpty_3(void);
int IsComOutBufEmpty_3(void);
int SetDataFormat_3(int databit,int parity,int stopbit);
int ReadComn_3(unsigned char *buf,int n);
void SetRtsActive_3(void);
void SetRtsInactive_3(void);
int GetCtsStatus_3(void);
void SendBreak_3(unsigned timems);
void SetBreakMode_3(int mode);
int IsDetectBreak_3(void);

/*
 Functions for COM4
*/
#define InstallCom4	InstallCom_4
#define RestoreCom4	RestoreCom_4
#define IsCom4		IsCom_4
#define ToCom4		ToCom_4
#define ToCom4Str	ToComStr_4
#define ToCom4Bufn	ToComBufn_4
#define printCom4	printCom_4
#define ClearTxBuffer4	ClearTxBuffer_4
#define SetCom4FifoTriggerLevel		SetComFifoTriggerLevel_4
#define SetBaudrate4	SetBaudrate_4
#define ReadCom4	ReadCom_4
#define ClearCom4	ClearCom_4
#define DataSizeInCom4  DataSizeInCom_4
#define WaitTransmitOver4	WaitTransmitOver_4
#define IsTxBufEmpty4		IsTxBufEmpty_4
#define IsCom4OutBufEmpty	IsComOutBufEmpty_4
#define ReadCom4n		ReadComn_4
#define SetDataFormat4		SetDataFormat_4
#define SetRtsActive4		SetRtsActive_4
#define SetRtsInactive4		SetRtsInactive_4
#define GetCtsStatus4		GetCtsStatus_4
#define SetBreakMode4		SetBreakMode_4
#define SendBreak4		SendBreak_4
#define IsDetectBreak4		IsDetectBreak_4

int InstallCom_4(unsigned long baud, int data, int parity, int stop);
int RestoreCom_4(void);
int IsCom_4(void);
int ToCom_4(int data);
int ToComStr_4(char *str);
int ToComBufn_4(char *buf,int no);
int printCom_4(char *fmt,...);
void ClearTxBuffer_4(void);
int SetComFifoTriggerLevel_4(int level);
int SetBaudrate_4(unsigned long baud);
int ReadCom_4(void);
int ClearCom_4(void);
int DataSizeInCom_4(void);
int WaitTransmitOver_4(void);
int IsTxBufEmpty_4(void);
int IsComOutBufEmpty_4(void);
int SetDataFormat_4(int databit,int parity,int stopbit);
int ReadComn_4(unsigned char *buf,int n);
void SetRtsActive_4(void);
void SetRtsInactive_4(void);
int GetCtsStatus_4(void);
void SendBreak_4(unsigned timems);
void SetBreakMode_4(int mode);
int IsDetectBreak_4(void);

/* For COM4 without use INTERRUPT */
int IsCom_4_1(void);
int ReadCom_4_1(void);
int ReadComn_4_1(unsigned char *buf,int no);
int ToCom_4_1(int data);
void ReadDataFromUartFifo(void);
void SendDataToUartFifo(void);
void EnableMonitorCom4(void);
void DisableMonitorCom4(void);

/* for I-8000 module(slot/back plane) */
typedef union {
  struct {
   unsigned char do8,di8;
  } data8;
  unsigned      data16;
} DIODATA;

typedef union {
  DIODATA data;
  struct {
   unsigned do16,di16;
  } data16;
  unsigned long  data32;
} DIODATA32;


extern int NumberOfSlot;
extern int SlotAddr[8]; /*  ={0x80,0xa0,0xc0,0xe0,0x140,0x160,0x180,0x1a0};*/
extern int __Last87kSlot;
extern int __InChangeSlot;

#define _PARALLEL 0x80
#define _SCAN     0x40
#define _32BIT    0x20
#define _8BIT     0x10

#define _DI32     0xE3
#define _DO32     0xE0
#define _DI16DO16 0xE2
#define _DI16     0xC3
#define _DO16     0xC0
#define _DI8DO8   0xC2
#define _DI8      0x93
#define _DO8      0x90

extern unsigned far *BA56912; /*=(unsigned far *)0x0040007EL; */
extern unsigned char far * const ModuleType; /*=(unsigned char far *)0x00400038L; */
extern unsigned char far * const NameOfModule; /*=(unsigned char far *)0x00400030L; */
extern unsigned long far * const LedData32; /*(unsigned long far *)0x0040004CL; */

extern DIODATA32 far *DIOData32;

int GetNetId(void);
void SetA9(int mode);
void SetA12(int mode);
int GetB1A5A6(void);
void RestoreB1A5A6(int a5a6);
void SetB1A5(int mode);
void SetB1A6(int mode);
void SetB1A5A6(int mode); /* mode: bit0-->A5, Bit1-->A6 */
void ChangeToSlot(int slot);
void ClrSlotInt(int mask);
void ClrAllSlotInt(void);
void DisableNMI(void);
void EnableNMI(void);
int GetSlotInt(void);
int GetNumberOfSlot(void);
void ShowLED16(int slot ,unsigned cdata);
void ShowLED8(int slot ,unsigned char cdata);
void ShowLED32(int slot ,unsigned long cdata);
void DO_32(int slot,unsigned long cdata);	/* For 32 bits output */
void DIO_DO_16(int slot,unsigned cdata);	/*  For:  16 bits output on DI16/DO16 */
void DO_16(int slot,unsigned int cdata);	/* For 16 bits output */
void DO_8(int slot,unsigned char cdata);	/* For 8 bits output */
void DIO_DO_8(int slot,unsigned char cdata);	/*  For:  8 bits output on DI8/DO8 or 4 bits output on DI4/DO4 */
unsigned long int DI_32(int slot);	/* For 32 bits input */
unsigned int DI_16(int slot);		/* For 16 bits input */
unsigned char DI_8(int slot);		/*For 8 bits input */
/*
[11/03/2003] add function for universal DI/O card.(such as 8050)
*/
#define UDIO_DI16	DI_16
void UDIO_DO16(int slot,unsigned cdata);
unsigned UDIO_ReadConfig_16(int slot);
void UDIO_WriteConfig_16(int slot,unsigned config);

/*
 functions for COM port on 8142/8142I/8144/8144I/8112/8114
*/
int SetInBufSIze(int size);
int SetOutBufSIze(int size);
int _SetBaudrate(int slot,int port,unsigned long baud);
int _SetDataFormat(int slot,int port,int data, int parity,int stop);
int RestoreCom8000(int slot);
int InstallCom8000(int slot);
int IsCom8000(int slot,int port);
int ToCom8000(int slot,int port,int data);
int ToCom8000Str(int slot,int port,char *str);
int ToCom8000nBytes(int slot,int port,char *buf,int no);
int ReadCom8000(int slot,int port);
int ReadCom8000nBytes(int slot,int port,char *buf,int maxno);
int ClearCom8000(int slot, int port);
void ShowErrLedCom8000(int slot,int data);
/*
	bit vallue: 0: LED ON, 1:LED OFF
	bit 0(0x01) for com port 1
	bit 1(0x02) for com port 2
	bit 2(0x04) for com port 3(for 8144(I)/8114 only)
	bit 3(0x08) for com port 4(for 8144(I)/8114 only)
*/
#define _MSR_dCTS	0x01
#define _MSR_dDSR	0x02
#define _MSR_TERI	0x04
#define _MSR_dDCD	0x08
#define _MSR_CTS	0x10
#define _MSR_DSR	0x20
#define _MSR_RI		0x40
#define _MSR_DCD	0x80
#define _MCR_DTR	1
#define _MCR_RTS	2

int GetCom8000_MSR(int slot, int port);
/*
  Get current MSR.(Modem Status Register)

  On error return PortError(-1).
  On success return 8 bits status. The bit definition is as above.(_MSR_xxx)
*/
int SetCom8000_MCR(int slot, int port,int mcr);
/*
  Set the MCR(Modem Control Register) bit 0(DTR) & bit 1(RTS).
  On error return PortError(-1).
  On success return NoError(0).

  Only support to set DTR & RTS bit.
  for set RTS active just call SetCom8000_MCR(slot, port,_MCR_DTR+_MCR_RTS); (also active DTR)
  for set RTS inactive just call SetCom8000_MCR(slot, port,_MCR_DTR); (only active DTR)
  call SetCom8000_MCR(slot, port,0); to inactive both RTS & DTR.
*/


/*
  for S-MMI key
*/
#define SKEY_SET    1
#define SKEY_DOWN   2
#define SKEY_MODE   3
#define SKEY_UP     4

extern unsigned char far * const KeyStatus;
#define SKEY_SET_DOWN   0x08
#define SKEY_DOWN_DOWN  0x04
#define SKEY_UP_DOWN    0x02
#define SKEY_MODE_DOWN  0x01

int IsSystemKey(void);
void ClearSystemKey(void);
int GetSystemKey(void);

/*
  For Send command to I-7000/I-87K series.
*/
extern char hex_to_ascii[16];
int ascii_to_hex(char ascii);

int SendCmdTo7000(int iPort, unsigned char *cCmd, int iChksum);
/*
(INPUT)iPort:can be 0,1,3,4.
(INPUT)lTimeout: unit is ms.
(INPUT) cCmd: cmd for send to COM port(I-7000/I-87K).
              DON'T add '\r' at the end of cCmd, SendCmdTo7000() will add check sum(if needed) & '\r' after cCmd .
(INPUT) iChksum: 0: disable, 1: enable.
*/

int ReceiveResponseFrom7000(int iPort, unsigned char *cCmd, long lTimeout, int iChksum);
/*
(INPUT)iPort:can be 0,1,3,4.
(INPUT)lTimeout: unit is check times.
(OUTPUT) cCmd: response from COM port(I-7000/I-87K).
(INPUT) iChksum: 0: disable, 1: enable.
*/

int ReceiveResponseFrom7000_1(int iPort, unsigned char *cCmd, long lTimeout, int iChksum);
/*
(INPUT)iPort:can be 0,1,3,4.
(INPUT)lTimeout: unit is ms. (*****)
(OUTPUT) cCmd: response from COM port(I-7000/I-87K).
(INPUT) iChksum: 0: disable, 1: enable.
*/


/* for ALL COM PORT */
int printCom(int port,char *fmt,...);
int IsDetectBreak(int port);
int SendBreak(int port,unsigned timems);
int SetBreakMode(int port,int mode);
int ClearCom(int port);
int ClearTxBuffer(int port);
int InstallCom(int port, unsigned long baud, int data, int parity,int stop);
int ToComBufn(int port,char *buf,int no);
int RestoreCom(int port);
int ToComStr(int port,char *str);
int DataSizeInCom(int port);
int IsCom(int port);
int ReadComn(int port,unsigned char *buf,int n);
int ReadCom(int port);
int SetBaudrate(int port,unsigned long baud);
int ToCom(int port,int data);
int IsTxBufEmpty(int port);
int GetTxBufferFreeSize(int port);
int WaitTransmitOver(int port);
int SetRtsActive(int port);
int SetRtsInactive(int port);
int GetCtsStatus(int port);

/*  function table for up functions except printCom */
/*
  For example if want to call:
   if(IsCom(port)){
   	data=ReadCom(port);
   }
  also can use :
   if(IsCom_[port]()){
   	data=ReadCom_[port]();
   }

  IsCom(port)/ReadCom(port) just for backword compatible, it also will call IsCom_[port]()/ReadCom_[port]()

  and so on.
*/
extern int (*IsDetectBreak_[5])(void);
extern void (*SendBreak_[5])(unsigned timems);
extern void (*SetBreakMode_[5])(int mode);
extern int (*ClearCom_[5])(void);
extern void (*ClearTxBuffer_[5])(void);
extern int (*InstallCom_[5])(unsigned long baud, int data, int parity,int stop);
extern int (*ToComBufn_[5])(char *buf,int no);
extern int (*RestoreCom_[5])(void);
extern int (*ToComStr_[5])(char *str);
extern int (*DataSizeInCom_[5])(void);
extern int (*IsCom_[5])(void);
extern int (*ReadComn_[5])(unsigned char *buf,int n);
extern int (*ReadCom_[5])(void);
extern int (*SetBaudrate_[5])(unsigned long baud);
extern int (*ToCom_[5])(int data);
extern int (*IsTxBufEmpty_[5])(void);
extern int (*GetTxBufferFreeSize_[5])(void);
extern int (*WaitTransmitOver_[5])(void);
extern void (*SetRtsActive_[5])(void);
extern void (*SetRtsInactive_[5])(void);
extern int (*GetCtsStatus_[5])(void);

int GetComportNumber(void);

void InitLib(void);
/*
  MUST CALL InitLib() first before use "DIOData32".
*/
void GetLibDate(char *date);
unsigned GetLibVersion(void);

extern int TriggerLevel[4];

/*
 Current version is 2.01 (return 0x0201)
*/

/* [92/09/19] add */
void SetComPortBufferSize_3(int in_size,int out_size);
void SetComPortBufferSize_4(int in_size,int out_size);
/*
When use SetComPortBufferSize_0/1/2/3/4() to set the size of com port input/output buffer,
must call before call InstallCom_0/1/2/3/4().
*/

void InstallComInputData_2(int (*DoInputData)(unsigned char data));
void InstallComInputData_3(int (*DoInputData)(unsigned char data));
void InstallComInputData_4(int (*DoInputData)(unsigned char data));
/*
After call InstallComInputData_2/3/4(), When the COM port ISR for COM2/3/4 receive a data,
it will call the function installed(DoInputData), and pass the received data to it.
And IsCom() will always return 0.
*/

/* [11/06/2003] add Software flow control(Xon/Xoff) for COM1~COM4 except COM2
	COM2 is RS-485, used in half-duplex mode, need not software flow control.
*/
void SetXonXoffControlMode_1(int mode);
void SetXonXoffControlMode_3(int mode);
void SetXonXoffControlMode_4(int mode);
/*
  mode=0 --> disable Xon/Xoff control
  mode=1 --> enable Xon/Xoff control
*/

/*
 [2003/12/01]
 Add function for debug, using STDIO COM PORT.
 Even after all InstallCom_1() also can use these 3 functions to send message to STDIO COM port.
*/
void pascal _dPutch(int data1);
void _dPuts(char *str);
int _dPrint(char *fmt,...);

/*
 [2003/12/10]
 Add function for read system timeticks.
*/
long GetTimeTicks(void);
long GetTimeTicks_ISR(void); /* use this one in ISR */
/*
  The return value is *TimeTicks.
*/

int InstallUserTimerFunction_us(unsigned time,void (*fun)(void));
/*
  time unit is 0.1 us.

  for example:
	If want timer generate interrupt for every 0.5ms(500 us=5000*0.1us)
	(That is to say system will call your function once every 0.5 ms)
	just use

	void fun(void)
	{
		...
	}
	...
	InstallUserTimerFunction_us(5000,fun);
*/

int InstallUserTimerFunction_ms(unsigned time,void (*fun)(void));
/*
  time unit is ms.

  for example:
	If want timer generate interrupt for every 1 second(1 sec=1000 ms)
	(That is to say system will call your function once every 1 sec.)
	just use

	void fun(void)
	{
		...
	}
	...
	InstallUserTimerFunction_ms(1000,fun);
*/

void StopUserTimerFun(void);

/* 2004/02/26 add function usr burst mode to read date/time from RTC chip(DS-1302) */
typedef struct {
	int year;
	char month,day,weekday;
	char hour,minute,sec;
}TIME_DATE;

void GetTimeDate(TIME_DATE *timedate);
int SetTimeDate(TIME_DATE *timedate);
/*
  when call SetTimeDate(), need set the right year,month,day and the function
  will auto set the weekday.
*/

#ifdef __cplusplus
}
#endif

#endif
