//#include <icpdas.h>
#include <8000e.h>
#include <8084W.h>
//#include "candle_structs.h"
//#include "funcs.h"

#define ECSLOT 0
#define DISLOT 1
#define DOSLOT 2

#define ALARM_DELAY 1500

#define ERROR_ALARM_DELAY -1


int waitUntil(unsigned long flag){
	unsigned long rd_data=0,iTime=0,uli=0;
	TimerResetValue();
	for(;;){
		rd_data = DI_32(DISLOT);
		Print("read:[%04x]\r\n",rd_data);
		Print("need:[%04x]\r\n",flag);
		uli = rd_data&flag;
		Print(" && :[%04x]\r\n",uli);
		if(uli==flag)return 0;
		iTime=TimerReadValue();
		if(iTime>=ALARM_DELAY)return ERROR_ALARM_DELAY;
	}
}
int exception(int e){
	Print("Error occurs %i",e);
	return 0;
}
int sendCommand(unsigned long command){
	//DO_32(DOSLOT,command);
	return 0;
}
int readSignals(unsigned long *data){
	unsigned long di;
	di = DI_32(DISLOT);
	memcpy(data,&di,4);
	Print("DI data = %-08lX\n\r",data);
	return 0;
}
//ifdef INW_TEST
void test_check(){
	 unsigned long a = 3L,b = 1;
	 Print("Welcome to Inw production!\r\n");
	 Print("  A:[%04x]\r\n",a);
	 Print("  B:[%04x]\r\n",b);
	 Print(" <<:[%04x]\r\n",a<<2);
	 Print(" >>:[%04x]\r\n",a>>1);
 	 Print("  &:[%04x]\r\n",a&b);
	 Print("  |:[%04x]\r\n",a|b);
	 //Print("  A:[%04x]\r\n  B:[%04x]\r\n  C:[%04x]\r\n",a,b,c);
	 Print("\r\n\r\n");
}
void main(void){
	int ret;
	// Timer value
	int iTime,counter=0;
	// DI_DO modules reading buffer
	unsigned long di_data=0,do_data=0;
	// Console reading buffer;
	unsigned char consoleTemp[10];
	// Standart I/O buffer
	char ioTemp[256];
	// Always do that to init lib i8000
	InitLib();
	// i8000 has only one systimer
	TimerOpen();
	TimerResetValue();
	// Help messages to Console
	Print("Please command (q-quit,c-clear)=");
	// endless circle for main thread
	//for(;;){
	while(counter<1){
		/**************************************************************************
		 * reading section
		 **************************************************************************/
		 // DI module reading
		 ret = readSignals(&di_data);
		 if(ret)if(exception(ret))goto Return;
		 /**************************************************************************
 		 * analyze section
 		 **************************************************************************/
		 /**************************************************************************
 		 * program checks and locks section
 		 **************************************************************************/
		 /**************************************************************************
 		 * command section
 		 **************************************************************************/
		 ret = sendCommand(do_data);
		 if(ret)if(exception(ret))goto Return;
		 /**************************************************************************
 		 * external control section
 		 **************************************************************************/
		 /*if(Kbhit()){
			 LineInput(consoleTemp,10);
			 if(memcmp(consoleTemp,"q",1))goto Return;
			 if(memcmp(consoleTemp,"c",1)){
				 TimerResetValue();
			 }
		 }*/
		 // Self analyze
		 iTime=TimerReadValue();
		 Print("Program works %i seconds\r\n",iTime);
		 // for debug mode only
		 counter++;
	}
	Return:
		TimerClose();
	return;
}
