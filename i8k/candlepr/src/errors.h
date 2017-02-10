#ifndef _ERRORS_H_
#define _ERRORS_H_

#define ERROR_ALARM_DELAY -1

#define ERROR_EEPROM_READ -2 //0010
#define ERROR_EEPROM_WRITE -3 //0011
#define ERROR_WRONG_COMAND -4
#define ERROR_UNABLED_COMMAND -5
#define ERROR_NOT_INITIATED -6
#define ERROR_UNKNOWN -7
#define ERROR_ENCODER_NOTFOUND -17 //1 0001 - inwPrint("Initiate 8080 on slot%d error!\n\r",ECSLOT);
#define ERROR_ENCODER_UBNORMAL -18 //1 0010 - Ubnormal encoder state, need calibration

#define ERROR_DIO_TIMEOUT -33//10 00001

#define ERROR_COM_TIMEOUT -49//11 00001



#endif
