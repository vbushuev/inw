//#include <icpdas.h>
//#include <8000e.h>
//#include <8084W.h>
//#include "candle_structs.h"
#include "funcs.h"
int gRegisters[128];

void (far *Reset_Program)(void)=0xFFFF0000L; //Program start address.
void main(void){
	int ret;
	// Timer value
	int counter=0,ioLength,io, oldCommand;
	//
	byte oldKey;
	// scenario control
	int sstep=0, sstage = 0, todo = 0, finished = 1,bDO = 0, stepsInCirce;
	// DI_DO modules reading buffer
	dword di_data=0,do_data=0,iTimeout=0,iCircleTime = 0,enc_data;
	long encLUp,encLDown,encRUp,encRDown,ulto;
	// Console reading buffer;
	unsigned char consoleTemp[10];
	// Standart I/O buffer
	char ioTemp[256];
	// Settings
	//sRuntimeValues rtv;
	//sTotalValues tv;
	// scenarios
	sStep far *currentScenario = 0;
	// Start
	ret = initInw();
	if(ret)exception(ret);

	// Help messages to Console
	// inwPrint("Please command (q-quit,c-clear)=");
	//test_check();
	// endless circle for main thread
	gRegisters[0x20]=0x0;
	for(;;){
	//while(counter<10){
		/**************************************************************************
		 * reading section
		 **************************************************************************/
		 // DI module reading
		 ret = readSignals(&di_data);
		 if(ret)exception(ret);
		 // Read Encoders
		 ret = Encoder(0,&enc_data);
		 if(ret!=0)exception(ret);
		 ret = Encoder(1,&enc_data);
		 if(ret!=0)exception(ret);
		 // read Panel
		 oldCommand = gRegisters[0x20];
		 readModbusRTU();
		 // System keys
		 if(IsSystemKey()){
			 byte sKey = GetSystemKey();
			 ledn(17,sKey);
			 switch(sKey){
				 case SKEY_UP:
				 	break;
				 case SKEY_DOWN:
					break;
                 case SKEY_SET:
				 	if(oldKey==SKEY_MODE){
						Reset_Program();
					}
				 	goto Return;
				 	//break;
                 case SKEY_MODE:
			   	 	break;
			 }
			 oldKey = sKey;
		 }
		 /**************************************************************************
 		 * analyze section
 		 **************************************************************************/
		 switch (gRegisters[0x20]){
			 case 0x1: {// Init scenario{}
			 	sStep scenarioW[]={
		 			{0x00000020,{2,0/*rtv.Lh*/},0x00000000,0},//00
		 			{0x00000001,{0,0x00002945},0x00000000,0},//01
		 			{0x00002000,{2,0x00002145/*rtv.Rh*/},0x00000000,0},//02
		 			{0x00000000,{1,0x00002145/*rtv.tstart*/},0x00000000,0},//03
					{0x00000000,{5,0x00000000},0x00000000,0} //04 finish
		 	 	};
		 	 	scenarioW[0].wait.value = gRegisters[0x03];
		      	scenarioW[2].wait.value = gRegisters[0x05];
		      	scenarioW[3].wait.value = gRegisters[0x0a]*1000;
				stepsInCirce = 5;
				if(!finished)break;
				currentScenario = scenarioW;
				sstep = 0;
				sstage = 0;
				todo = 1;
				gRegisters[0x30] = 0;
				finished = 0;
				gRegisters[0x08] = 1;
				gRegisters[0x09] = 1;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
			 }break;
			 case 0x3: {// Start scenario{}
			 	sStep scenarioW[]={
					{0x00000001,{0,0x00000001},0x00000000,0},//00
					{0x00000002,{0,0x00004000},0x00000000,0},//01
					{0x00000044,{0,0x00004044},0x00000000,0},//02
					{0x00000410,{0,0x00004064},0x00000000,0},//03
					{0x00000020,{2,0/*rtv.Lh*/},0x00000000,0},//04
					{0x00000800,{0,0x00004144},0x00000000,0},//05
					{0x00012000,{0,0x00006944},0x00000000,0},//06
					{0x00014000,{2,0/*rtv.Rh*/},0x00000000,0},//07
					{0x00000000,{5,0x00000000},0x00000000,0} //08 finish
				};
				scenarioW[4].wait.value = gRegisters[0x03];
		        scenarioW[7].wait.value = gRegisters[0x05];
				stepsInCirce = 9;
			 	if(!finished)break;
				finished = 0;
				currentScenario = scenarioW;
			 	//ret = loadStartScenario(currentScenario);if(ret){exception(ret);continue;}
				sstep = 0;
				sstage = 0;
				todo = 1;
				gRegisters[0x30] = 0;
				gRegisters[0x08] = 1;
				gRegisters[0x09] = 1;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
			 }break;
			 case 0x5: {// Main scenario{}
			 	sStep scenarioW[] = {
					{0x00000002,{0,0x00002146},0x00000000,0},//00
				  	{0x00000008,{0,0x0000214a},0x00000000,0},//01
				  	{0x00000410,{2,0x0000214a/*rtv.LC*/},0x00000000,0},//02
					{0x00000008,{0,0x00002152},0x00000000,0},//03
					{0x00000004,{0,0x00002146},0x00000000,0},//04
					{0x00000080,{0,0x00002186},0x00000000,0},//05
				  	{0x00000410,{0,0x000021a6},0x00000000,0},//06
				  	{0x00000100,{1,0x000021a6/*rtv.ttaker*/},0x00000100,0},//07
				  	{0x00000120,{2,0x00002186/*rtv.Lh*/},0x00000100,0},//08
				  	{0x00000300,{1,0x00002186/*rtv.tV10*/},0x00000100,0},//09
				  	{0x00000140,{0,0x00002146},0x00000000,0},//10
				  	{0x00000001,{0,0x00002145},0x00000000,0},//11

				  	{0x00000000,{3,0x00002145},0x00000000,0},  //12    Произвести фиксацию замеров для левого

				  	{0x00001000,{0,0x00002245},0x00000000,0},//13
				  	{0x00082000,{2,0x00002245/*rtv.RC*/},0x00000000,0},//14
				  	{0x00001000,{0,0x00002445},0x00000000,0},//15
				  	{0x00000800,{0,0x00002145},0x00000000,0},//16
				  	{0x00008000,{0,0x00001145},0x00000000,0},//17
				  	{0x00082000,{0,0x00001945},0x00000000,0},//18
				  	{0x00020000,{1,0x00001945/*rtv.ttaker*/},0x00020000,0},//19
				  	{0x00024000,{2,0x00001145/*rtv.Rh*/},0x00020000,0},//20
				  	{0x00060000,{1,0x00001145/*rtv.tV19*/},0x00020000,0},//21
				  	{0x00030000,{0,0x00002145},0x00000000,0},//22

				  	{0x00000000,{4,0x00002145},0x00000000,0},  //23    Произвести фиксацию замеров для левого
			  	};
				scenarioW[2].wait.value = gRegisters[0x02];
				scenarioW[7].wait.value = gRegisters[0x0b]*1000; // ttaker
				scenarioW[8].wait.value = gRegisters[0x03];
				scenarioW[9].wait.value = gRegisters[0x00]*1000;// v10
				scenarioW[14].wait.value = gRegisters[0x04];
				scenarioW[19].wait.value = gRegisters[0x0b]*1000; // ttaker
				scenarioW[20].wait.value = gRegisters[0x05];
				scenarioW[21].wait.value = gRegisters[0x01]*1000; //V19
				stepsInCirce = 24;
				if(!finished)break;
				finished = 0;
				currentScenario = scenarioW;
				sstep = 0;
				sstage = 0;
				todo = 1;
				gRegisters[0x30] = 0;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
				gRegisters[0x0e] = 0;
				gRegisters[0x0f] = 0;
				iCircleTime = 0;
			 }break;
			 case 0x06:{
				 di_data = gRegisters[0x21];
				 encLUp = gRegisters[0x23];
				 encRUp = gRegisters[0x24];
 				 if(finished==1){
					 sStep scenarioW[] = {
	 					{0x00000002,{1,0x00000fa0},0x00000000,0},//00 wait for 4000 seconds
	 					{0x00000003,{0,0x00000003},0x00000000,0},//01 wait for DI = 0x0003
	 					{0x00000004,{0,0x00000004},0x00000000,0},//02 wait for DI = 0x0004
	 					{0x00000005,{2,0x00000100},0x00000000,0},//03 wait for Encoder L = 0x100
						{0x00000000,{3,0x00000000},0x00000000,0},//04 finish
						{0x00000006,{2,0x00000200},0x00000000,0},//05 wait for Encoder R = 0x200
	 				  	{0x00000000,{4,0x00000000},0x00000000,0},//06 finish
	 				  	{0x00000000,{5,0x00000000},0x00000000,0} //07 finish
	 			  	 };
					 stepsInCirce = 8;
					 finished = 0;
					 currentScenario = scenarioW;
					 sstep = 0;
	 				 sstage = 0;
	 				 todo = 1;
	 				 gRegisters[0x30] = 0;
	 				 gRegisters[0x0c] = 0;
	 				 gRegisters[0x0d] = 0;
	 				 gRegisters[0x0e] = 0;
	 				 gRegisters[0x0f] = 0;
	 				 iCircleTime = 0;
				 }
				 break;
			 }
			 case 0x7f: {// manual mode
				 di_data = gRegisters[0x21];
				 do_data = gRegisters[0x22];
				 encLUp = gRegisters[0x23];
				 encRUp = gRegisters[0x24];
				 //gRegisters[0x20] = oldCommand;
				 break;
			 }
			 case 0x80: {// Stop Signal
				sstep = 0;
				sstage = 0;
				todo = 0;
				bDO = 0;
				finished = 1;
				gRegisters[0x20] = 0;
			 }break;
			 default:
			 	di_data = gRegisters[0x21];
			 	do_data = gRegisters[0x22];bDO = 1;
			 	encLUp = gRegisters[0x23];
			 	encRUp = gRegisters[0x24];
			 break;
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
						gRegisters[0x0c]++;
	   				 	gRegisters[0x0e] = ict;
						// счетчик пробега
						gRegisters[0x10]++;
	   				 	gRegisters[0x12] += ict;
						gRegisters[0x30] = 1;
					}
					else if(currentScenario[sstep].wait.type==4){
						int ict = 0;
						ict = TimerReadValue() - iCircleTime;
						iCircleTime = TimerReadValue();
						sstage=3;
						gRegisters[0x0d]++;
	   				 	gRegisters[0x0f] = ict;
						if(gRegisters[0x0d]>=gRegisters[0x08]){ // пока делаем одинаковое кол-во циклов для левой и правой
							finished = 1;
							todo = 0;
							gRegisters[0x20]=0;
						}
						// счетчик пробега
						gRegisters[0x11]++;
	   				 	gRegisters[0x13] += ict;
						gRegisters[0x30] = 0;
					}
					else if(currentScenario[sstep].wait.type==5){
						finished = 1;
						todo = 0;
						gRegisters[0x20]=0;
					}
					else{
						//if(currentScenario[sstep].wait.type==1)iStepTime = TimerReadValue();
					 	do_data = currentScenario[sstep].command;
						bDO = 1;
						sstage=1;
						//TimerResetValue();
						iTimeout = TimerReadValue();
					}
					break;
				 }
				 case 1:{
					 byte flag = 0;
					 switch(currentScenario[sstep].wait.type){
						 case 0: // wait for special DI signals
 							flag = (di_data == currentScenario[sstep].wait.value)?1:0;
							if(di_data&0x00000020)clearEncoder(0); // нижний поршень в верхнем положении левый(0)
							if(di_data&0x00000800)clearEncoder(1); // нижний поршень в верхнем положении правый(1)
 							break;
						 case 1: // Timeout wait
							flag = ( (TimerReadValue()-iTimeout)>= currentScenario[sstep].wait.value) ? 1 : 0;
							break;
						 case 2: // Encoder wait
							flag = (
								(
									(gRegisters[0x30]==0)&&(gRegisters[0x23] >= currentScenario[sstep].wait.value)
								)
								||
								(
									(gRegisters[0x30]==1)&&(gRegisters[0x24] >= currentScenario[sstep].wait.value)
								)
							)?1:0;
							break;
					 }
					 if(flag==1){
						sstage=2;
						do_data = currentScenario[sstep].finish;
						bDO = 1; // flag to send command
					 }
					 else { // check timeout
						 //if((TimerReadValue()-iTimeout) >= ((currentScenario[sstep].timer>0)?currentScenario[sstep].timer:(gRegisters[0x25]>0?gRegisters[0x25]:TOTAL_TIMEOUT))){
						 if((TimerReadValue()-iTimeout) >= gRegisters[0x25]*1000){
							 //alarm
							 gRegisters[0x20] = 0x80;
							 exception(ERROR_ALARM_DELAY);
						 }
					 }
					 break;
				 }
				 case 2:{
					 //RefreshWDT();
					 sstage = 3;
				 }
			 }
			 if(sstage>2){
				 sstep++;
				 sstage = 0;
			 }
			 //debug
			 if(sstep>=stepsInCirce){ // сбрасываем счетчик для повтора команды
				 sstep = 0;
			 }
		 }
		 //ledn(17,(TimerReadValue()-iTimeout));
		 //if(currentScenario!=0)ledn(16,currentScenario[sstep].wait.type*256+sstep*16+sstage);
		 /**************************************************************************
 		 * command section
 		 **************************************************************************/
		 if(bDO){
			 ret = sendCommand(do_data);
			 if(ret)exception(ret);
			 bDO = 0;
		 }
		 /**************************************************************************
 		 * external control section
 		 **************************************************************************/
		 gRegisters[0x21] = (unsigned int)di_data;
		 gRegisters[0x22] = (unsigned int)do_data;
		 gRegisters[0x23] = (unsigned int)encLUp%65536;
		 gRegisters[0x24] = (unsigned int)encRUp%65536;
		 // Self analyze
		 //iTime=TimerReadValue();
		 // for debug mode only
		 //gRegisters[0x25] = (int)(TimerReadValue()%65536);
		 gRegisters[0x26] = (int)(iTimeout%65536);
		 counter++;
		 //DelayMs(800);
		 showError();
	}
	Return:
		deinitInw();
	DelayMs(1000);
	return;
}
