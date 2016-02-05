/*
 8073 library

-----------------------------------------------------------------------------------
 8073 hardware support
 A/D(12 bits) * 1 (input range : 0~5V or -5V~+5V)
 D/I * 4
 D/O * 4	(Open collector)
 MMC card * 1
-----------------------------------------------------------------------------------
version 1.01(start at [07/29/2003])
1. Change function name to start with "_8073_".
   (Also define old function name to the new function name for source level compatible.)

EEPROM(24LC16):
 total space is 2048 BYTES. divided into 8 blocks, every block has 256 bytes,
 the last 32 bytes(block 7, addr=0xE0~0xFF) is used for store A/D calibration data.
 other space can be used.
*/

#ifdef __cplusplus
extern "C" {
#endif

int _8073_GetLibVersion(void);
/*
Version 1.01 start to support this function.
return value:
	0x0101 for version 1.01,(0x0102 for ver. 1.02,... ,etc.)
*/

char *_8073_GetLibDate(void);
/*
Version 1.01 start to support this function.
return value:
	return a string for the library DATA.
	the date format will like : "Jul 30 2003"
*/

unsigned int Initial8073(int slot,unsigned long *BN,unsigned long *MS, int mode);
/*
  will call _8073_Initial_MMC() & _8073_Initial_AD().
*/

/* FOR MMC card --------------------------------------*/
#define Initial8073_MMC	_8073_Initial_MMC
#define	MMC_WRITE_BLOCK _8073_MmcWriteBlock
#define	MMC_READ_BLOCK	_8073_MmcReadBlock

extern int Slot_8073; /* [2014/04/24] for MMC/SD file system. */

unsigned int _8073_Initial_MMC(int slot,unsigned long *BlockNo,unsigned long *MemorySize);
/*
Before use a new MMC card must call this function.

argument:
	slot:[INPUT]
		for 84xx: 0~3, for 88xx: 0~7
	BlockNo:[OUTPUT]
		the block size of the MMC card.
	MemorySize[OUTPUT]
		the total memory size of the MMC card.
return value:
	0 for success, and store the block number to ClockNo & total memory size to MemorySize.
	1 for error(BlockNo & MemorySize are not changed).
*/

unsigned int _8073_MmcWriteBlock(int slot,unsigned char *data,unsigned long block,int TryCount);
/*
argument:
	slot:[INPUT]
		for 84xx: 0~3, for 88xx: 0~7
	data:[INPUT]
		the data buffer.
	block:[INPUT]
		the block number for data write to.
	TryCount:[INPUT]
		when write error will try TryCount times.
return value:
	0 for write successfully.
	1/2 for write fail.
*/

unsigned int _8073_MmcReadBlock(int slot,unsigned char *data,unsigned long block,int TryCount);
/*
argument:
	slot:[INPUT]
		for 84xx: 0~3, for 88xx: 0~7
	data:[OUTPUT]
		the data buffer.
	block:[INPUT]
		the block number for data write to.
	TryCount:[INPUT]
		when write error will try TryCount times.
return value:
	0 for read successfully.
	1/2 for read fail.
*/

/* for EEPROM ------------------------------------------*/
#define EE_WriteEnable_8073	_8073_EE_WriteEnable
#define EE_WriteProtect_8073	_8073_EE_WriteProtect
#define EE_RandomRead_8073	_8073_EE_RandomRead
#define EE_ReadNext_8073	_8073_EE_ReadNext
#define EE_MultiRead_8073	_8073_EE_MultiRead
#define EE_RandomWrite_8073	_8073_EE_RandomWrite
#define EE_MultiWrite_8073	_8073_EE_MultiWrite

void _8073_EE_WriteEnable(int slot);
/*
Set EEPROM to write enable mode, before write data to EEPROM must call this function
once.
*/

void _8073_EE_WriteProtect(int slot);
/*
Set EEPROM to  write protect mode.
*/

int _8073_EE_RandomRead(int slot,int Block,int Addr);
/*
Read one byte data from EEPROM.
argument:
	Block:[INPUT]
		0~7
	Addr:[INPUT]
		0~255
*/

int _8073_EE_ReadNext(int slot,int Block);
/*
  Read the next byte(since last read) data fro EEPROM.
*/

int _8073_EE_MultiRead(int slot,int StartBlock,int StartAddr,int no,char *databuf);
/*
  Read multi bytes data from EEPROM.(At max can read all 2048 bytes.)
*/
int _8073_EE_RandomWrite(int slot,int Block,int Addr,int Data);
/*
  Write one byte data to EEPROM at block/addr.
*/
int _8073_EE_MultiWrite(int slot,int Block,int Addr,int no,char *Data);
/*
  Write multi bytes data to EEPROM.
  Every time call this function can write 16 bytes Max.
  The write address will only change the Least 4 bits,for example,
  start address is 0x4A, write 10 bytes, the 10 bytes data will write to adddress
  0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x40(NOT 0x50),0x41(NOT 0x51),0x42(NOT 0x52),0x43(NOT 0x53)
*/

/* for DO(0-3) ------------------------------------------------------------*/
#define SetDO	_8073_SetDO
void _8073_SetDO(int slot,int dd);

/* for DI(0-3) ------------------------------------------------------------*/
#define GetDI	_8073_GetDI
int _8073_GetDI(int slot);

/* for A/D ----------------------------------------------------------------*/
#define _8073_MODE_10V_ 0
#define _8073_MODE_5V_	1
#define AnalogIn	_8073_GetAD_Hex
#define GetAngIn	_8073_GetAD_Calibration
#define Read_AD_CalibrationGain		_8073_Read_AD_CalibrationGain
#define Read_AD_CalibrationOffset	_8073_Read_AD_CalibrationOffset
#define Write_AD_CalibrationGain	_8073_Write_AD_CalibrationGain
#define Write_AD_CalibrationOffset	_8073_Write_AD_CalibrationOffset

void _8073_Initial_AD(int slot,int mode);
/*
  If want use the function _8073_GetAD() with calibration, must call _8073_Initial_AD() first.
argument:
	slot:[INPUT]
		for 84xx: 0~3, for 88xx: 0~7
	mode:[INPUT]
		0(_8073_MODE_10V_) for input range -5V ~ +5V
		1(_8073_MODE_5V_)  for input range 0V ~ +5V
*/

int _8073_GetAD_Hex(int slot);
/*
return value:
	the 12 bits A/D value, Min=0, Max=0xFFF.
*/
int _8073_GetAD_Hex_Calibration(int slot);
/*
return value:
	The A/D value with calibration, 16 bits,map -32768~32767 to -5V~+5V for both input mode.
*/

float _8073_GetAD_Calibration(int slot);
/*
return value:
	the A/D value with calibration transfer to Volt.
*/

float _8073_Read_AD_CalibrationGain(int slot, int mode);
/*
return value:
	the calibration GAIN value stored in EEPROM.
*/

float _8073_Read_AD_CalibrationOffset(int slot, int mode);
/*
return value:
	the calibration OFFSET value stored in EEPROM.
*/

int _8073_Write_AD_CalibrationGain(int slot, int mode, float fGain);
/*
  Store the calibration GAIN to EEPROM.

return value:
	0: success.
	1,2: write to EEPROM fail.
*/

int _8073_Write_AD_CalibrationOffset(int slot, int mode, float fOffset);
/*
  Store the calibration OFFSET to EEPROM.

return value:
	0: success.
	1,2: write to EEPROM fail.
*/

#ifdef __cplusplus
 }
#endif
