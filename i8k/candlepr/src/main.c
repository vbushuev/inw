//#include <icpdas.h>
//#include <8000e.h>
//#include <8084W.h>
//#include "candle_structs.h"
#include "funcs.h"
word gRegisters[128];

void (far *Reset_Program)(void)=0xFFFF0000L; //Program start address.
void main(void){
	int ret;
	// Timer value
	int counter=0,ioLength,io;
	//
	byte oldKey;
	// scenario control
	int sstep=0, sstage = 0, todo = 0, finished = 1,bDO = 0, stepsInCirce;
	// DI_DO modules reading buffer
	dword di_data=0,do_data=0,iTimeout=0,iCircleTime = 0,enc_data,do_add = 0,diff=0;
	long ulto,lenc = 0,renc=0;
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

	clearEncoder(0); // нижний поршень в верхнем положении левый(0)
	clearEncoder(1); // нижний поршень в верхнем положении правый(1)
	// Help messages to Console
	// inwPrint("Please command (q-quit,c-clear)=");
	//test_check();
	// endless circle for main thread
	gRegisters[0x20]=0x0;
	// encoder delta
	gRegisters[0x31]=0x32;
	for(;;){
	//while(counter<10){
		/**************************************************************************
		 * reading section
		 **************************************************************************/
		 // DI module reading
		 di_data = 0;
		 ret = readSignals(&di_data);
		 if(ret)exception(ret);
		 /*
		 diff = di_data&0x00000020;
		 if(diff==0x00000020)clearEncoder(0); // нижний поршень в верхнем положении левый(0)
		 diff = di_data&0x00000800;
		 if(diff==0x00000800)clearEncoder(1); // нижний поршень в верхнем положении правый(1)
		 diff = 0;s
		 */
		 // Read Encoders
		 ret = Encoder(0,&lenc);
		 if(ret!=0)exception(ret);
		 //ledn(0xe,enc_data);
		 ret = Encoder(1,&renc);
		 if(ret!=0)exception(ret);
		 // read Panel
		 //!!!!!   readModbusRTU();
		 // System keys
		 if(IsSystemKey()){
			 byte sKey = GetSystemKey();
			 ledn(17,sKey);
			 switch(sKey){
				 case SKEY_UP:
				 	clearEncoder(0);
				 	clearEncoder(1);
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
		 // Hydro sensor
		 gRegisters[0x2d] = ((di_data&0x8000)==0x8000)?0:1;
		 /*
		 if(((gRegisters[0x2c])<<20)!=do_add){
			 do_add=gRegisters[0x2c];
			 do_add<<=20;
			 bDO=1;
		 }*/
		 switch (gRegisters[0x20]){
			 case 0x1: {// Init scenario{}
			 	sStep scenarioW[]={
					{0x00000004,{0,0x00000004},0x00000000,0},//00
					{0x00000800,{0,0x00000100},0x00000000,0},//01
					{0x00000040,{0,0x00000040},0x00000000,0},//02
					{0x00010000,{0,0x00002000},0x00000000,0},//03
					//{0x00000010,{0,0x00000020},0x00000000,0},//04
					//{0x00002000,{0,0x00000800},0x00000000,0},//05
					{0x00000020,{1,0x00000000/*rtv.Lh*/},0x00000000,0},//04
					{0x00004000,{1,0x00000000/*rtv.Rh*/},0x00000000,0},//05
					{H_01,{0,H_15},0x00000000,0},//06
					{0x00000000,{5,0x00000000},0x00000000,0},//07
		 	 	};
		      	scenarioW[4].wait.value =1500;
		      	scenarioW[5].wait.value =1500;
				if(compare_bit(di_data,H_01)==1){
					scenarioW[6].command = H_02;
				}
				stepsInCirce = 8;
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
					{0x00000010,{0,0x00000020},0x00000000,0},//00
					{0x00000020,{2,0x00000000/*rtv.Lh*/},0x00000000,0},//00
					{0x00000001,{0,0x00000001},0x00000000,0},//01
					{0x00000000,{5,0x00000000},0x00000000,0} //02 finish
				};
				scenarioW[1].wait.value = gRegisters[0x03];
		        //scenarioW[7].wait.value = gRegisters[0x05];
				stepsInCirce = 4;
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
					{0x00000002,{0,0x00000002},0x00000000,0},//00
				  	{0x00000008,{0,0x00000008},0x00000000,0},//01
				  	{0x00000410,{2,0x00000000/*rtv.LC*/},0x00000000,0},//02
					{0x00000008,{0,0x00000010},0x00000000,0},//03
					{0x00000004,{0,0x00000004},0x00000000,0},//04
					{0x00000080,{0,0x00000080},0x00000000,0},//05
				  	{0x00000410,{0,0x00000020},0x00000000,0},//06
				  	{0x00000100,{1,0x000021a6/*rtv.ttaker*/},0x00000100,0},//07
				  	{0x00000120,{2,0x00002186/*rtv.Lh*/},0x00000100,0},//8
				  	{0x00000300,{1,0x00002186/*rtv.tV10*/},0x00000100,0},//9
				  	{0x00000140,{0,0x00000040},0x00000000,0},//10
				  	{0x00000001,{0,0x00000001},0x00000000,0},//11

				  	{0x00000000,{3,0x00000000},0x00000000,0},  //12    Произвести фиксацию замеров для левого

				  	{0x00001000,{0,0x00000200},0x00000000,0},//13
				  	{0x00082000,{2,0x00000000/*rtv.RC*/},0x00000000,0},//14
				  	{0x00001000,{0,0x00000400},0x00000000,0},//15
				  	{0x00000800,{0,0x00000100},0x00000000,0},//16
				  	{0x00008000,{0,0x00001000},0x00000000,0},//17
				  	{0x00082000,{0,0x00000800},0x00000000,0},//18
				  	{0x00020000,{1,0x00000000/*rtv.ttaker*/},0x00020000,0},//19
				  	{0x00024000,{2,0x00000000/*rtv.Rh*/},0x00020000,0},//20
				  	{0x00060000,{1,0x00000000/*rtv.tV19*/},0x00020000,0},//21
				  	{0x00030000,{0,0x00002000},0x00000000,0},//22

				  	{0x00000000,{4,0x00000000},0x00000000,0},  //23    Произвести фиксацию замеров для левого
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

			 case 0x07: // V1 -> A1
			 case 0x08: //V1 -> A15
		     case 0x09: //V2  -> A2
		     case 0x0a: // V2 ->A15
		     case 0x0b: // V5 ->A5
		     case 0x0c: // V6 ->LC
		     case 0x0d: // V6 ->LH
		     case 0x0e: // V12 ->A9
		     case 0x0f: // V13 ->A10
		     case 0x10: // V13 ->A11
		     case 0x11: // V14 ->A12
		     case 0x12: // V15 ->RC
		     case 0x13: // V15 ->RH

			 case 0x14: // V7 ->A7
			 case 0x15: // V8 -> A8
			 case 0x16: // V17 ->A14
			 case 0x17: // V16 ->A13

			 case 0x18: // V3 -> A3
			 case 0x19: // V4 ->A4
			 case 0x1a: // V4 ->A5

			 case 0x20: // V9
			 case 0x21: // V18
			 case 0x22: // V10
			 case 0x23: // V19
			 {
				 if(finished==1){
					 sStep scenarioW[] = {
						 {0x00000001,{0,0x00000001},0x00000000,0},
				 		 {0x00000000,{5,0x00000000},0x00000000,0},  //23    Произвести фиксацию замеров для левого
					 };
					 gRegisters[0x30] = 0;
					 gRegisters[0x08] = 1;

					 gRegisters[0x0c] = 0;
					 gRegisters[0x0d] = 1;
					 gRegisters[0x0e] = 0;
					 gRegisters[0x0f] = 0;
					 switch (gRegisters[0x20]){
					 	case 0x07: {sStep sCommandStep = {0x00000001,{0,0x00000001},0x00000000,0};scenarioW[0]=sCommandStep;break;}
						case 0x08: {sStep sCommandStep = {0x00000001,{0,0x00004000},0x00000000,0};scenarioW[0]=sCommandStep;break;}
						case 0x09: {sStep sCommandStep = {0x00000002,{0,0x00000002},0x00000000,0};scenarioW[0]=sCommandStep;break;}
						case 0x0a: {sStep sCommandStep = {0x00000002,{0,0x00004000},0x00000000,0};scenarioW[0]=sCommandStep;break;}
						case 0x0b: {sStep sCommandStep = {0x00000010,{0,0x00000020},0x00000000,0};scenarioW[0]=sCommandStep;break;}
						case 0x0c: {sStep sCommandStep = {0x00000010,{2,0x00000010/*rtv.LC*/},0x00000000,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x02];break;}
						case 0x0d: {sStep sCommandStep = {0x00000020,{2,0x00000010/*rtv.LC*/},0x00000000,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x03];break;}

						case 0x0e: {sStep sCommandStep = {0x00000800,{0,0x00000100},0x00000000,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;break;}
						case 0x0f: {sStep sCommandStep = {0x00001000,{0,0x00000200},0x00000000,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;break;}
						case 0x10: {sStep sCommandStep = {0x00001000,{0,0x00000400},0x00000000,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;break;}
						case 0x11: {sStep sCommandStep = {0x00002000,{0,0x00000800},0x00000000,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;break;}
						case 0x12: {sStep sCommandStep = {0x00002000,{2,0x00000000/*rtv.RC*/},0x00000000,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;scenarioW[0].wait.value = gRegisters[0x04];break;}
						case 0x13: {sStep sCommandStep = {0x00004000,{2,0x00000000/*rtv.Rh*/},0x00000000,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;scenarioW[0].wait.value = gRegisters[0x05];break;}

						case 0x14: {sStep sCommandStep = {0x00000040,{0,0x00000040},0x00000000,0};scenarioW[0]=sCommandStep;break;}
						case 0x15: {sStep sCommandStep = {0x00000080,{0,0x00000080},0x00000000,0};scenarioW[0]=sCommandStep;break;}
						case 0x16: {sStep sCommandStep = {0x00010000,{0,0x00002000},0x00000000,0};scenarioW[0]=sCommandStep;break;}
						case 0x17: {sStep sCommandStep = {0x00008000,{0,0x00001000},0x00000000,0};scenarioW[0]=sCommandStep;break;}

						case 0x18: {sStep sCommandStep = {0x00000004,{0,0x00000004},0x00000000,0};scenarioW[0]=sCommandStep;break;}
						case 0x19: {sStep sCommandStep = {0x00000008,{0,0x00000008},0x00000000,0};scenarioW[0]=sCommandStep;break;}
						case 0x1a: {sStep sCommandStep = {0x00000008,{0,0x00000010},0x00000000,0};scenarioW[0]=sCommandStep;break;}


						case 0x20: {sStep sCommandStep = {0x00000100,{1,0x00000000/*rtv.ttaker*/},0x00000000,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x0b]*1000;break;}
						case 0x21: {sStep sCommandStep = {0x00020000,{1,0x00000000/*rtv.ttaker*/},0x00000000,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x0b]*1000;break;}
						case 0x22: {sStep sCommandStep = {0x00000200,{1,0x00000000/*rtv.tV10*/},0x00000000,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x00]*1000;break;}
						case 0x23: {sStep sCommandStep = {0x00040000,{1,0x00000000/*rtv.tV19*/},0x00000000,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x01]*1000;break;}

				 	}
					if(!finished)break;
					 stepsInCirce = 2;
					 finished = 0;
					 currentScenario = scenarioW;
					 sstep = 0;
					 sstage = 0;
					 todo = 1;

					 iCircleTime = 0;
				 }
				 break;
			 }
			 case 0x1b: {// V5 ->A6 V6->LH
				 sStep scenarioW[]={
					{0x00000010,{0,0x00000020},0x00000000,0},//00
					{0x00000020,{2,0x00000000/*rtv.Lh*/},0x00000000,0},//00
					{0x00000000,{5,0x00000000},0x00000000,0} //02 finish
				};
				scenarioW[1].wait.value = gRegisters[0x03];
			    //scenarioW[7].wait.value = gRegisters[0x05];
				stepsInCirce = 3;
				if(!finished)break;
				finished = 0;
				currentScenario = scenarioW;
				//ret = loadStartScenario(currentScenario);if(ret){exception(ret);continue;}
				sstep = 0;
				sstage = 0;
				todo = 1;
				gRegisters[0x30] = 0;
				gRegisters[0x08] = 0;
				gRegisters[0x09] = 0;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
				 break;
			 }
			 case 0x1c: {// V14 ->A12 V15->RH
				sStep scenarioW[]={
					{0x00002000,{0,0x00000800},0x00000000,0},//00
					{0x00004000,{2,0x00000000/*rtv.Rh*/},0x00000000,0},//01
					{0x00000000,{5,0x00000000},0x00000000,0} //02 finish
				};
				scenarioW[1].wait.value = gRegisters[0x05];
				stepsInCirce = 3;
			    //scenarioW[7].wait.value = gRegisters[0x05];
				if(!finished)break;

				finished = 0;
				currentScenario = scenarioW;
				//ret = loadStartScenario(currentScenario);if(ret){exception(ret);continue;}
				sstep = 0;
				sstage = 0;
				todo = 1;
				gRegisters[0x30] = 1;
				gRegisters[0x08] = 0;
				gRegisters[0x09] = 0;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
				break;
			 }
 			 case 0x1d:{ // setup left
				 sStep scenarioW[] = {
 					{H_03,{0,H_03},H_00,0},//00
 					{H_05,{0,H_06},H_00,0},//01
 					{H_06,{2,H_00},H_00,0},//02 LH
 					{H_01,{0,H_01},H_00,0},//03
 				  	{H_00,{1,1000},H_00,0},//04 timer
 				  	{H_02,{0,H_15},H_00,0},//05
 					{H_04,{0,H_04},H_00,0},//06
 					{H_05,{2,H_00},H_00,0},//07 LC
 					{H_04,{0,H_05},H_00,0},//08
 					{H_03,{0,H_03},H_00,0},//09
					{H_05,{0,H_06},H_00,0},//10
 					{H_00,{5,H_00},H_00,0},  //11 Finish
 			  	};
				scenarioW[2].wait.value = gRegisters[0x03];
				scenarioW[4].wait.value = gRegisters[0x0b]*1000;
 				scenarioW[7].wait.value = gRegisters[0x02];
				stepsInCirce = 12;
				if(!finished)break;
				finished = 0;
 				currentScenario = scenarioW;
 				sstep = 0;
 				sstage = 0;
 				todo = 1;
				gRegisters[0x30] = 0;
				gRegisters[0x08] = 1;
				gRegisters[0x09] = 1;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
 				iCircleTime = 0;
				break;
			 }
			 case 0x1e:{ // setup right
				 sStep scenarioW[] = {
 					{H_12,{0,H_09},H_00,0},//00
 					{H_14,{0,H_12},H_00,0},//01
 					{H_15,{2,H_00},H_00,0},//02 LH
 					{H_02,{0,H_02},H_00,0},//03
 				  	{H_00,{1,1000},H_00,0},//04 timer
 				  	{H_01,{0,H_15},H_00,0},//05
 					{H_13,{0,H_10},H_00,0},//06
 					{H_14,{2,H_00},H_00,0},//07 LC
 					{H_13,{0,H_11},H_00,0},//08
 					{H_12,{0,H_09},H_00,0},//09
					{H_14,{0,H_12},H_00,0},//10
 					{H_00,{5,H_00},H_00,0},  //11 Finish
 			  	};
 				scenarioW[2].wait.value = gRegisters[0x05];
 				scenarioW[7].wait.value = gRegisters[0x04];
 				stepsInCirce = 12;
 				if(!finished)break;
 				finished = 0;
 				currentScenario = scenarioW;
 				sstep = 0;
 				sstage = 0;
 				todo = 1;
 				gRegisters[0x30] = 1;
 				gRegisters[0x0c] = 0;
 				gRegisters[0x0d] = 0;
 				gRegisters[0x0e] = 0;
 				gRegisters[0x0f] = 0;
 				iCircleTime = 0;
				break;
			 }
			 case 0x7f: {// manual mode
				 di_data = gRegisters[0x21];
				 do_data = gRegisters[0x22];
				 //gRegisters[0x20] = oldCommand;
				 break;
			 }
			 case 0x80: {// Stop Signal
				currentScenario = 0;
				sstep = 0;
				sstage = 0;
				todo = 0;
				bDO = 1;
				do_data = 0;
				finished = 1;
				gRegisters[0x20] = 0;
			 }break;
			 default:
			 	di_data = gRegisters[0x21];
			 	do_data = gRegisters[0x22];bDO = 1;
			 break;
		 }
		 /**************************************************************************
 		 * program checks and locks section
 		 **************************************************************************/

		 if(gRegisters[0x30]==0)ledn(0xe,lenc); else ledn(0xe,renc);
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
						if(currentScenario[sstep].wait.type==2){
							diff = currentScenario[sstep].command&0x00000020;
							if(diff == 0x00000020){
								clearEncoder(0); // нижний поршень в верхнем положении левый(0)
							}
							else {
								diff = currentScenario[sstep].command&0x00004000;
								if (diff == 0x00004000){
									clearEncoder(1); // нижний поршень в верхнем положении левый(0)
								}
							}
						}
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
					 gRegisters[0x32]=currentScenario[sstep].wait.value;
					 gRegisters[0x33]=currentScenario[sstep].wait.type;
					 switch(currentScenario[sstep].wait.type){
						 case 0: // wait for special DI signals
 							//flag = (di_data == currentScenario[sstep].wait.value)?1:0;
							diff = di_data&currentScenario[sstep].wait.value;
 							flag = (diff == currentScenario[sstep].wait.value)?1:0;

							diff = di_data&0x00000020;
							if(diff==0x00000020){
								clearEncoder(0); // нижний поршень в верхнем положении левый(0)
								//DelayMs(80);
							}
							diff = di_data&0x00000800;
							if(diff==0x00000800){
								clearEncoder(1); // нижний поршень в верхнем положении правый(1)
								//DelayMs(80);
							}
							break;
						 case 1: // Timeout wait
							flag = ( (TimerReadValue()-iTimeout)>= currentScenario[sstep].wait.value) ? 1 : 0;
							break;
						 case 2: {// Encoder wait
							 long lwaitval = currentScenario[sstep].wait.value;
							 if(gRegisters[0x30]==0){ //left
								 diff = do_data&0x00000010;
								 if(diff == 0x00000010) flag = (lwaitval >= lenc)?1:0;
								 else flag = (lwaitval <= lenc)?1:0;
							 }
							 else { //right
								 diff = do_data&0x00002000;
								 if(diff == 0x00002000) flag = (lwaitval >= renc)?1:0;
								 else flag = (lwaitval<=renc)?1:0;
							 }

							break;
						}
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
					 DelayMs(8);
					 sstage = 3;
				 }
			 }
			 if(sstage==3){
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
			 //do_data+=do_add;
			 if( (gRegisters[0x20] == 0x05) && (sstep==0) && (sstage==1) && (!compare_bit(di_data,0x00002144))){
				 exception(ERROR_NOT_INITIATED);
				 gRegisters[0x20] = 0x80;
			 }
			 if(!ret)ret = check (di_data,do_data,currentScenario[sstep].wait);
			 if(ret){
				 exception(ret);
				 gRegisters[0x20] = 0x80;
			 }
			 else {
				 ret = sendCommand(do_data);
				 if(ret)exception(ret);
			 }
			 bDO = 0;
		 }
		 /**************************************************************************
 		 * external control section
 		 **************************************************************************/
		 // read Panel
		 if(sstage!=1){
			 readModbusRTU();
		 }
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
        return;
	//DelayMs(1000);
}
