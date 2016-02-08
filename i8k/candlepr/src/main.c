//#include <icpdas.h>
//#include <8000e.h>
//#include <8084W.h>
//#include "candle_structs.h"
#include "funcs.h"



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

//ifdef INW_TEST

void test_check(){
	 unsigned long a = 3L,b = 1;
	 unsigned char tbyte='A';
	 int tint = 1;
	 char tchar = 'B';
	 sRuntimeValues rtv = {
		 50,
		 50,
		 100,
		 150,

		 80,
		 120,
		 60,
		 40,

		 100,
		 120,
		 500,
		 100,

		 0,
		 0,
		 0,
		 0
	 };
	 sTotalValues tv = {0,0,0,0};
	 int *tpointer = &tint;
	 Print("Welcome to Inw production!\r\n");
	 Print("  A:[%04x]\r\n",a);
	 Print("  B:[%04x]\r\n",b);
	 Print(" <<:[%04x]\r\n",a<<2);
	 Print(" >>:[%04x]\r\n",a>>1);
 	 Print("  &:[%04x]\r\n",a&b);
	 Print("  |:[%04x]\r\n",a|b);
	 Print("Sizeof byte: %i\r\n",sizeof(tbyte));
	 Print("Sizeof int: %i\r\n",sizeof(tint));
	 Print("Sizeof char: %i\r\n",sizeof(tchar));
	 Print("Sizeof pointer: %i\r\n",sizeof(tpointer));
	 Print("Sizeof long: %i\r\n",sizeof(a));
	 Print("Sizeof sRuntimeValues: %i\r\n",sizeof(rtv));
	 Print("Sizeof sTotalValues: %i\r\n",sizeof(tv));
	 Print("Value of sRuntimeValues.tstart: %i\r\n",rtv.tstart);
	 getTotal(&tv);
	 Print("Value of sTotalValues.CRtotal: %i\r\n",tv.CRtotal);

	 //Print("  A:[%04x]\r\n  B:[%04x]\r\n  C:[%04x]\r\n",a,b,c);
	 Print("\r\n\r\n");
}
void main(void){
	int ret;
	// Timer value
	int iTime,counter=0;
	// DI_DO modules reading buffer
	unsigned long di_data=0,do_data=0;
	long encLUp,encLDown,encRUp,encRDown;
	// Console reading buffer;
	unsigned char consoleTemp[10];
	// Standart I/O buffer
	char ioTemp[256];
	// Settings
	sRuntimeValues rtv;
	sTotalValues tv;
	ret = initInw(&rtv,&tv);
	if(ret)if(exception(ret))goto Return;
	// Help messages to Console
	Print("Please command (q-quit,c-clear)=");
	//test_check();
	// endless circle for main thread
	//for(;;){
	while(counter<1){
		/**************************************************************************
		 * reading section
		 **************************************************************************/
		 // DI module reading
		 ret = readSignals(&di_data);
		 if(ret)if(exception(ret))goto Return;
		 // Read Encoders
		 ret = readEncoderL(&encLUp,&encLDown);
		 if(ret)if(exception(ret))goto Return;
		 ret = readEncoderR(&encRUp,&encRDown);
		 if(ret)if(exception(ret))goto Return;
		 // read Panel
		 
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
