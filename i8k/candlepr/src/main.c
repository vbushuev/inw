//#include <icpdas.h>
//#include <8000e.h>
//#include <8084W.h>
//#include "candle_structs.h"
#include "funcs.h"
int gRegisters[128];
void main(void){
	int ret;
	// Timer value
	int iTime,counter=0,ioLength,io;
	// scenario control
	int sstep=0, currentScenario = 0;
	unsigned long wait_di = 0;
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
	// scenarios
	sStep *startScenario,
		*initScenario,
		*mainScenario;
	// Start
	ret = initInw(&rtv,&tv);
	if(ret)if(exception(ret))goto Return;
	ret = loadInitScenario(rtv,initScenario);
	if(ret)if(exception(ret))goto Return;
	ret = loadStartScenario(rtv,startScenario);
	if(ret)if(exception(ret))goto Return;
	ret = loadMainScenario(rtv,mainScenario);
	if(ret)if(exception(ret))goto Return;

	// Help messages to Console
	// inwPrint("Please command (q-quit,c-clear)=");
	//test_check();
	// endless circle for main thread
	for(;;){
	//while(counter<10){
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
		 readModbus();
		 /**************************************************************************
 		 * analyze section
 		 **************************************************************************/
		 switch(currentScenario){
			 case 0:
			 	do_data = initScenario[sstep].command;
				wait_di = initScenario[sstep].wait.value;

		 }
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
		 // Self analyze
		 iTime=TimerReadValue();
		 inwPrint("Program works %i seconds\r\n",iTime);
		 // for debug mode only
		 counter++;
		 DelayMs(800);
	}
	Return:
		deinitInw();

	return;
}
