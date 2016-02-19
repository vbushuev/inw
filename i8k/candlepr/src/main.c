//#include <icpdas.h>
//#include <8000e.h>
//#include <8084W.h>
//#include "candle_structs.h"
#include "funcs.h"
int gRegisters[128];
void main(void){
	int ret;
	// Timer value
	int counter=0,ioLength,io,iTimeout=0,iCircleTime=0,iStepTime;
	// scenario control
	int sstep=0, sstage = 0, todo = 0, finished = 1, currentPiston=0 /* 0 -left, 1 -right*/,bDO = 0;
	// DI_DO modules reading buffer
	unsigned long di_data=0,do_data=0;
	long encLUp,encLDown,encRUp,encRDown;
	// Console reading buffer;
	unsigned char consoleTemp[10];
	// Standart I/O buffer
	char ioTemp[256];
	// Settings
	//sRuntimeValues rtv;
	//sTotalValues tv;
	// scenarios
	sStep *currentScenario = 0;
	// Start
	ret = initInw();
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
		 if(ret){exception(ret);continue;}
		 // Read Encoders
		 ret = readEncoderL(&encLUp,&encLDown);
		 if(ret){exception(ret);continue;}
		 ret = readEncoderR(&encRUp,&encRDown);
		 if(ret){exception(ret);continue;}
		 // read Panel
		 readModbus();
		 /**************************************************************************
 		 * analyze section
 		 **************************************************************************/
		 switch (gRegisters[0x20]){
			 case 0x1: {// Init scenario{}
			 	if(!finished)break;
				ret = loadInitScenario(currentScenario);
				if(ret){exception(ret);continue;}
				sstep = 0;
				sstage = 0;
				todo = 1;
				currentPiston = 0;
				gRegisters[0x08] = 1;
				gRegisters[0x09] = 1;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
			 }break;
			 case 0x3: {// Start scenario{}
			 	if(!finished)break;
			 	ret = loadStartScenario(currentScenario);
				if(ret){
					exception(ret);
					continue;
				}
				sstep = 0;
				sstage = 0;
				todo = 1;
				currentPiston = 0;
				gRegisters[0x08] = 1;
				gRegisters[0x09] = 1;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
			 }break;
			 case 0x5: {// Main scenario{}
			 	if(!finished)break;
				ret = loadMainScenario(currentScenario);
				if(ret){
					exception(ret);
					continue;
				}
				sstep = 0;
				sstage = 0;
				todo = 1;
				currentPiston = 0;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
				gRegisters[0x0e] = 0;
				gRegisters[0x0f] = 0;
				iCircleTime = 0;
			 }break;
			 case 0x80: {// Stop Signal
				sstep = 0;
				sstage = 0;
				todo = 0;
				bDO = 0;
				finished = 1;
			 }break;
		 }
		 /**************************************************************************
 		 * program checks and locks section
 		 **************************************************************************/
		 if(todo&&!bDO){
			 if(currentScenario==0)continue;
			 switch(sstage){
				 case 0:{
				 	if(currentScenario[sstep].wait.type==3){
						int ict = 0;
						ict = TimerReadValue() - iCircleTime;
						iCircleTime = TimerReadValue();
						sstage=3;
						gRegisters[(currentPiston==0)?0x0c:0x0d]++;
	   				 	gRegisters[(currentPiston==0)?0x0e:0x0f] = ict;
						if(gRegisters[0x0d]>=gRegisters[0x08]){ // пока делаем одинаковое кол-во циклов для левой и правой
							finished = 1;
							todo = 0;
							gRegisters[0x20]=0;
						}
						// счетчик пробега
						gRegisters[(currentPiston==0)?0x10:0x11]++;
	   				 	gRegisters[(currentPiston==0)?0x12:0x13] += ict;
						currentPiston = (currentPiston==1)?0:1;
					}
					else{
						if(currentScenario[sstep].wait.type==1)iStepTime = TimerReadValue();
					 	do_data = currentScenario[sstep].command;
						bDO = 1;
						sstage=1;
						iTimeout = TimerReadValue()-10;
					}
					break;
				 }
				 case 1:{
					 int flag = 0;
					 switch(currentScenario[sstep].wait.type){
						case 0: // wait for special DI signals
							flag = (di_data == currentScenario[sstep].wait.value);
							break;
						case 1: // Timeout wait
							flag=( (TimerReadValue() - iStepTime)>=currentScenario[sstep].wait.value);
							break;
						case 2: // Encoder wait
							flag = (((currentPiston==0)&&(encLDown /*encLUp*/ >= currentScenario[sstep].wait.value)) || ((currentPiston==1)&&(encRDown /*encRUp*/ >= currentScenario[sstep].wait.value)));
							break;
					 }
					 if(flag){
						sstage=2;
						do_data = currentScenario[sstep].finish;
						bDO = 1; // flag to send command
					 }
					 else { // check timeout
						 if((TimerReadValue()-iTimeout) >= ((currentScenario[sstep].timer>0)?currentScenario[sstep].timer:TOTAL_TIMEOUT)){
							 //alarm
							 gRegisters[0x20] = 0x80;
						 }
					 }
					 break;
				 }
				 case 2:{
					 RefreshWDT();
					 sstage = 3;
				 }
			 }
			 if(sstage>2){
				 sstep++;
				 sstage = 0;
			 }
			 if(sstep>=sizeof(currentScenario)){ // сбрасываем счетчик для повтора команды
				 sstep = 0;
			 }
		 }
		 /**************************************************************************
 		 * command section
 		 **************************************************************************/
		 if(bDO){
			 ret = sendCommand(do_data);
			 if(ret)if(exception(ret))goto Return;
			 bDO = 0;
		 }
		 /**************************************************************************
 		 * external control section
 		 **************************************************************************/
		 // Self analyze
		 //iTime=TimerReadValue();
		 // for debug mode only
		 counter++;
		 //DelayMs(800);
	}
	Return:
		deinitInw();

	return;
}
