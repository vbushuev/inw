//#include <icpdas.h>
//#include <8000e.h>
//#include <8084W.h>
//#include "candle_structs.h"
#include "funcs.h"
unsigned int gRegisters[128];
int todo = 0, finished = 1;
//dword iCircleTime = 0;
void (far *Reset_Program)(void)=0xFFFF0000L; //Program start address.

void main(void){
	int ret;
	// Timer value
	//int counter=0,ioLength,io;
	//
	// scenario control
	int sstep=0, stepsInCirce;
	sStep currentStep;
	// DI_DO modules reading buffer
	dword di_data=0;//,iTimeout=0;

	sStep initScenario[]={
		{H_03,{0,H_03},H_00,0},//00
		{H_12,{0,H_09},H_00,0},//01
		{H_07,{0,H_07},H_00,0},//02
		{H_17,{0,H_14},H_00,0},//03
		//{H_05,{0,H_06},H_00,0},//04
		//{H_14,{0,H_12},H_00,0},//05
		{H_06,{1,1500/*rtv.Lh*/},H_00,0},//04
		{H_15,{1,1500/*rtv.Rh*/},H_00,0},//05
		{H_01,{0,H_15},H_00,0},//06
		{H_00,{5,H_00},H_00,0}//07
	};
	sStep startScenario[]={
		{H_05,{0,H_06},H_00,0},//00
		{H_06,{2,H_00/*rtv.Lh*/},H_00,0},//01
		{H_01,{0,H_01},H_00,0},//02
		{H_00,{1,1500},H_00,0},//03
		{H_02,{0,H_15},H_00,0},//04
		{H_00,{5,H_00},H_00,0} //05 finish
	};
	sStep lencScenario[]={
	   {H_05,{0,H_06},H_00,0},//00
	   {H_06,{2,H_00/*rtv.Lh*/},H_00,0},//00
	   {H_00,{5,H_00},H_00,0} //02 finish
	};
	sStep rencScenario[]={
		{H_14,{0,H_12},H_00,0},//00
		{H_15,{2,H_00/*rtv.Rh*/},H_00,0},//01
		{H_00,{5,H_00},H_00,0} //02 finish
	};
	sStep bonusScenario[]={
	   {H_04,{0,H_04},H_00,0},//00
	   {H_05|H_11,{2,H_00/*rtv.LC*/},H_00,0},//01
	   {H_04,{0,H_05},H_00,0},//02
	   {H_03,{0,H_03},H_00,0},//03
	   {H_05,{0,H_06},H_00,0},//04
	   {H_08,{0,H_08},H_00,0},//05
	   {H_09,{1,200/*rtv.start*/},H_09,0},//06
	   {H_06|H_09,{2,H_00/*rtv.Lh*/},H_09,0},//07
	   {H_09|H_10,{1,H_00/*rtv.tV10*/},H_09,0},//08
	   {H_07|H_09,{0,H_07},H_00,0},//09

	   {H_00,{5,H_00},H_00,0} //10
	};
	sStep mainScenario[]={
		{H_04,{0,H_04},H_00,0},//00
		{H_05|H_11,{2,H_00/*rtv.LC*/},H_00,0},//01
		{H_04,{0,H_05},H_00,0},//02
		{H_03,{0,H_03},H_00,0},//03
		//{H_05|H_11,{0,H_06},H_00,0},//04
		{H_05,{0,H_06},H_00,0},//04
		{H_08,{0,H_08},H_00,0},//05
		{H_09|H_02,{0,H_02/*rtv.ttaker*/},H_09,0},//06
		{H_09,{1,200/*rtv.start*/},H_09,0},//07
		{H_06|H_09,{2,H_00/*rtv.Lh*/},H_09,0},//08
		{H_09|H_10,{1,H_00/*rtv.tV10*/},H_09,0},//09
		{H_07|H_09,{0,H_07},H_00,0},//10
		{H_01,{0,H_15},H_00,0},//11

		{H_00,{3,H_00},H_00,0},  //12    Произвести фиксацию замеров для левого

		{H_13,{0,H_10},H_00,0},//13
		{H_14|H_20,{2,H_00/*rtv.RC*/},H_00,0},//14
		{H_13,{0,H_11},H_00,0},//15
		{H_12,{0,H_09},H_00,0},//16
		//{H_14|H_20,{0,H_12},H_00,0},//16
		{H_14,{0,H_12},H_00,0},//17
		{H_16,{0,H_13},H_00,0},//18
		{H_18|H_01,{0,H_01/*rtv.ttaker*/},H_18,0},//19
		{H_18,{1,200/*rtv.start*/},H_18,0},//20
		{H_15|H_18,{2,H_00/*rtv.Rh*/},H_18,0},//21
		{H_18|H_19,{1,H_00/*rtv.tV19*/},H_18,0},//22
		{H_17|H_18,{0,H_14},H_00,0},//23
		{H_02,{0,H_15},H_00,0},//24
		{H_00,{4,H_00},H_00,0}  //25    Произвести фиксацию замеров для левого
	};

	sStep setupRightScenario[] = {
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
	   {H_00,{5,H_00},H_00,0}  //11 Finish
	};
	sStep setupLeftScenario[] = {
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
	   {H_05|H_08,{0,H_08},H_00,0},//10
	   {H_09,{1,400},H_09,0},//11
	   {H_06|H_09,{2,H_00},H_09,0},//12 LH
	   {H_10|H_09,{1,H_00},H_09,0},//13 cutter
	   {H_07|H_09,{0,H_07},H_00,0},//14 move outs
	   {H_00,{5,H_00},H_00,0}  //15 Finish
	};
	sStep mainScenarioMulti[]={
		{H_04|H_02,{0,H_04|H_02},H_00,0},//00
		{H_05|H_11,{2,H_00/*rtv.LC*/},H_00,0},//01
		{H_04,{0,H_05},H_00,0},//02
		{H_03,{0,H_03},H_00,0},//03
		{H_05|H_08,{0,H_06|H_08},H_00,0},//04
		{H_09|H_01,{0,H_15},H_09,0},//05
		{H_06|H_09,{2,H_00/*rtv.Lh*/},H_09,0},//06
		{H_09|H_10,{1,H_00/*rtv.tV10*/},H_09,0},//07
		{H_07|H_09,{0,H_07},H_00,0},//08

		{H_00,{3,H_00},H_00,0},  //09    Произвести фиксацию замеров для левого

		{H_13|H_01,{0,H_10|H_01},H_00,0},//10
		{H_14|H_20,{2,H_00/*rtv.RC*/},H_00,0},//11
		{H_13,{0,H_11},H_00,0},//12
		{H_12,{0,H_09},H_00,0},//13
		{H_14|H_16,{0,H_12|H_13},H_00,0},//14
		{H_18|H_02,{0,H_15/*rtv.ttaker*/},H_18,0},//15
		{H_15|H_18,{2,H_00/*rtv.Rh*/},H_18,0},//16
		{H_18|H_19,{1,H_00/*rtv.tV19*/},H_18,0},//17
		{H_17|H_18,{0,H_14},H_00,0},//18

		{H_00,{4,H_00},H_00,0}  //19    Произвести фиксацию замеров для левого
	};
	sStep testScenario[] = {
	   {H_01,{0,H_01},H_00,0},
	   {H_00,{5,H_00},H_00,0}  //23    Произвести фиксацию замеров для левого
	};
	// Console reading buffer;
	//unsigned char consoleTemp[10];
	// Standart I/O buffer
	//char ioTemp[256];
	// Settings
	//sRuntimeValues rtv;
	//sTotalValues tv;
	// scenarios
	sStep * currentScenario = NULL;
	// Start
	ret = initInw();
	if(ret)exception(ret);
	sendCommand(0);
	//clearEncoder(0); // нижний поршень в верхнем положении левый(0)
	//clearEncoder(1); // нижний поршень в верхнем положении правый(1)
	// Help messages to Console
	// inwPrint("Please command (q-quit,c-clear)=");
	//test_check();


	// do nothing on start
	gRegisters[0x20]=0x0;
	// encoder delta
	gRegisters[0x31]=0x05;
	// endless circle for main thread
	for(;;){
	//while(counter<10){
		/**************************************************************************
		 * reading section
		 **************************************************************************/
		 readModbusRTU();
		 // System keys
		 if(IsSystemKey()){
			 byte sKey = GetSystemKey();
			 //ledn(17,sKey);
			 switch(sKey){
				 case SKEY_UP:
				 	clearEncoder(0);
				 	clearEncoder(1);
				 	break;
				 case SKEY_DOWN:
					break;
                 case SKEY_SET:
				 	//if(sKey==SKEY_MODE){
						Reset_Program();
						goto Return;
					//}
				 	break;
				 	//break;
                 case SKEY_MODE:
			   	 	break;
			 }
		 }
		 //readSignalss
		 di_data = readSignals2();
		 /**************************************************************************
 		 * analyze section
 		 **************************************************************************/
		 // Hydro sensor
		 gRegisters[0x2d] = 1;//(di_data&0x8000)?0:1;
		 /*
		 if(((gRegisters[0x2c])<<20)!=do_add){
			 do_add=gRegisters[0x2c];
			 do_add<<=20;
		 }*/
		 if(finished){
		 	switch (gRegisters[0x20]){
			 case 0x1: {// Init scenario{}
			 	if(!finished)break;
				if((di_data&H_01)==1){
					initScenario[6].command = H_02;
				}
				currentScenario = initScenario;
				stepsInCirce = 8;
				sstep = 0;
				todo = 1;
				gRegisters[0x30] = 0;
				finished = 0;
				gRegisters[0x08] = 1;
				gRegisters[0x09] = 1;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
			 }break;
			 case 0x3: {// Start scenario{}
			 	if(!finished)break;
				startScenario[1].wait.value = gRegisters[0x03];
		        currentScenario = startScenario;
				stepsInCirce = 6;
				finished = 0;
				sstep = 0;
				todo = 1;
				gRegisters[0x30] = 0;
				gRegisters[0x08] = 1;
				gRegisters[0x09] = 1;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
			 }break;
			 case 0x5: {// Main scenario{}
				if(!finished)break;

				mainScenario[1].wait.value = gRegisters[0x03] - gRegisters[0x02];
				//mainScenario[6].wait.value = gRegisters[0x0b]; // ttaker
				mainScenario[7].wait.value = gRegisters[0x0a];
				mainScenario[8].wait.value = gRegisters[0x03];
				mainScenario[9].wait.value = gRegisters[0x00];// v10

				mainScenario[14].wait.value = gRegisters[0x05] - gRegisters[0x04];
				//mainScenario[19].wait.value = gRegisters[0x0b]; // ttaker
				mainScenario[20].wait.value = gRegisters[0x0a];
				mainScenario[21].wait.value = gRegisters[0x05];
				mainScenario[22].wait.value = gRegisters[0x01]; //V19
				currentScenario = mainScenario;
				stepsInCirce = 26;
				finished = 0;
				sstep = 0;
				todo = 1;
				gRegisters[0x30] = 0;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
				gRegisters[0x0e] = 0;
				gRegisters[0x0f] = 0;
				//iCircleTime = TimerReadValue();
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
			 case 0x23:{ // V19
				 if(finished==1){
					 gRegisters[0x30] = 0;
					 gRegisters[0x08] = 1;

					 gRegisters[0x0c] = 0;
					 gRegisters[0x0d] = 0;
					 gRegisters[0x0e] = 0;
					 gRegisters[0x0f] = 0;
					 switch (gRegisters[0x20]){
					 	case 0x07: {testScenario[0].command = H_01; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_01;break;}
						case 0x08: {testScenario[0].command = H_01; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_15;break;}
						case 0x09: {testScenario[0].command = H_02; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_02;break;}
						case 0x0a: {testScenario[0].command = H_02; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_15;break;}
						case 0x0b: {testScenario[0].command = H_05; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_06;break;}
						case 0x0c: {testScenario[0].command = H_05; testScenario[0].wait.type = 2;testScenario[0].wait.value = gRegisters[0x03] - gRegisters[0x02];break;}
						case 0x0d: {testScenario[0].command = H_06; testScenario[0].wait.type = 2;testScenario[0].wait.value = gRegisters[0x03];break;}

						case 0x0e: {testScenario[0].command = H_12; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_09;gRegisters[0x30]=1;break;}
						case 0x0f: {testScenario[0].command = H_13; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_10;gRegisters[0x30]=1;break;}
						case 0x10: {testScenario[0].command = H_13; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_11;gRegisters[0x30]=1;break;}
						case 0x11: {testScenario[0].command = H_14; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_12;gRegisters[0x30]=1;break;}
						case 0x12: {testScenario[0].command = H_14; testScenario[0].wait.type = 2;gRegisters[0x30]=1;testScenario[0].wait.value = gRegisters[0x05] - gRegisters[0x04];break;}
						case 0x13: {testScenario[0].command = H_15; testScenario[0].wait.type = 2;gRegisters[0x30]=1;testScenario[0].wait.value = gRegisters[0x05];break;}

						case 0x14: {testScenario[0].command = H_07; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_07;break;}
						case 0x15: {testScenario[0].command = H_08; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_08;break;}
						case 0x16: {testScenario[0].command = H_17; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_14;break;}
						case 0x17: {testScenario[0].command = H_16; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_13;break;}

						case 0x18: {testScenario[0].command = H_03; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_03;break;}
						case 0x19: {testScenario[0].command = H_04; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_04;break;}
						case 0x1a: {testScenario[0].command = H_04; testScenario[0].wait.type = 0; testScenario[0].wait.value = H_05;break;}


						case 0x20: {testScenario[0].command = H_09; testScenario[0].wait.type = 1;testScenario[0].wait.value = gRegisters[0x0b];break;}
						case 0x21: {testScenario[0].command = H_18; testScenario[0].wait.type = 1;testScenario[0].wait.value = gRegisters[0x0b];break;}
						case 0x22: {testScenario[0].command = H_10; testScenario[0].wait.type = 1;testScenario[0].wait.value = gRegisters[0x00];break;}
						case 0x23: {testScenario[0].command = H_19; testScenario[0].wait.type = 1;testScenario[0].wait.value = gRegisters[0x01];break;}

				 	}
					currentScenario = testScenario;
					stepsInCirce = 2;
					finished = 0;
					sstep = 0;
					todo = 1;
					//iCircleTime = 0;// TimerReadValue();
				 }
				 break;
			 }
			 case 0x1b: {// V5 ->A6 V6->LH
				 if(!finished)break;

				 lencScenario[1].wait.value = gRegisters[0x03];
			    //currentScenario[7].wait.value = gRegisters[0x05];
				currentScenario = lencScenario;
				 stepsInCirce = 3;
				 finished = 0;

				//ret = loadStartScenario(currentScenario);if(ret){exception(ret);continue;}
				sstep = 0;
				todo = 1;
				gRegisters[0x30] = 0;
				gRegisters[0x08] = 0;
				gRegisters[0x09] = 0;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
				 break;
			 }
			 case 0x1c: {// V14 ->A12 V15->RH
				 if(!finished)break;
				 rencScenario[1].wait.value = gRegisters[0x05];
				 currentScenario = rencScenario;
				 stepsInCirce = 3;
				 finished = 0;
				 sstep = 0;
				 todo = 1;
				gRegisters[0x30] = 1;
				gRegisters[0x08] = 0;
				gRegisters[0x09] = 0;
				gRegisters[0x0c] = 0;
				gRegisters[0x0d] = 0;
				break;
			 }
 			 case 0x1d:{ // setup left
				if(!finished)break;
				else{
					setupLeftScenario[2].wait.value = gRegisters[0x03];
					setupLeftScenario[4].wait.value = gRegisters[0x0b];
	 				setupLeftScenario[7].wait.value = gRegisters[0x03] - gRegisters[0x02];

					setupLeftScenario[11].wait.value = 100; // timer wait
					setupLeftScenario[12].wait.value = gRegisters[0x03]; // LH
					setupLeftScenario[13].wait.value = gRegisters[0x00]; // cutter
					currentScenario = setupLeftScenario;
					stepsInCirce = 16;
					finished = 0;
	 				sstep = 0;
	 				todo = 1;
					gRegisters[0x30] = 0;
					gRegisters[0x08] = 0;
					gRegisters[0x09] = 0;
					gRegisters[0x0c] = 0;
					gRegisters[0x0d] = 0;
	 				//iCircleTime = 0;
				}
				break;
			 }
			 case 0x1e:{ // setup right
				 if(!finished)break;
				 setupRightScenario[2].wait.value = gRegisters[0x05];
				 setupRightScenario[4].wait.value = gRegisters[0x0b];
 				 setupRightScenario[7].wait.value = gRegisters[0x05] - gRegisters[0x04];
				 currentScenario = setupRightScenario;
 				 stepsInCirce = 12;
 				 finished = 0;
 				 sstep = 0;
 				 todo = 1;
 				gRegisters[0x30] = 1;
 				gRegisters[0x0c] = 0;
 				gRegisters[0x0d] = 0;
 				gRegisters[0x0e] = 0;
 				gRegisters[0x0f] = 0;
 				//iCircleTime = 0;
				break;
			 }
			 case 0x1f:{ //test multi
				 if(finished==1){
					gRegisters[0x30] = 0;
					gRegisters[0x08] = 1;
					gRegisters[0x0c] = 0;
					gRegisters[0x0d] = 0;
					gRegisters[0x0e] = 0;
					gRegisters[0x0f] = 0;
					testScenario[0].command = H_05|H_08|H_02;
					testScenario[0].wait.type = 0;
					testScenario[0].wait.value = H_08;
					currentScenario = testScenario;
					stepsInCirce = 2;
					finished = 0;
					sstep = 0;
					todo = 1;
					//iCircleTime = 0;// TimerReadValue();
				}
				 break;
			 }
			 case 0x7f: {// manual mode
				 //di_data = gRegisters[0x21];
				 //gRegisters[0x20] = oldCommand;
				 break;
			 }
			 case 0x24:{
				 if(!finished)break;
	 		   	bonusScenario[1].wait.value = gRegisters[0x03] - gRegisters[0x02];
	 		    bonusScenario[7].wait.value = gRegisters[0x03];
	 		    bonusScenario[8].wait.value = gRegisters[0x00];// v10

	 		   currentScenario = bonusScenario;
	 		   stepsInCirce = 11;
	 		   finished = 0;
	 		   sstep = 0;
	 		   todo = 1;
	 		   gRegisters[0x30] = 0;
	 		   gRegisters[0x0c] = 0;
	 		   gRegisters[0x0d] = 0;
	 		   gRegisters[0x0e] = 0;
	 		   gRegisters[0x0f] = 0;
				 break;
			 }
			 case 0x25:{
				if(!finished)break;
				sStep mainScenarioMulti[]={
					{H_04|H_02,{0,H_04|H_02},H_00,0},//00
					{H_05|H_11,{2,H_00/*rtv.LC*/},H_00,0},//01
					{H_04,{0,H_05},H_00,0},//02
					{H_03,{0,H_03},H_00,0},//03
					{H_05|H_08,{0,H_06|H_08},H_00,0},//04
					{H_09|H_01,{0,H_15},H_09,0},//05
					{H_06|H_09,{2,H_00/*rtv.Lh*/},H_09,0},//06
					{H_09|H_10,{1,H_00/*rtv.tV10*/},H_09,0},//07
					{H_07|H_09,{0,H_07},H_00,0},//08

					{H_00,{3,H_00},H_00,0},  //09    Произвести фиксацию замеров для левого

					{H_13|H_01,{0,H_10|H_01},H_00,0},//10
					{H_14|H_20,{2,H_00/*rtv.RC*/},H_00,0},//11
					{H_13,{0,H_11},H_00,0},//12
					{H_12,{0,H_09},H_00,0},//13
					{H_14|H_16,{0,H_12|H_13},H_00,0},//14
					{H_18|H_02,{0,H_15/*rtv.ttaker*/},H_18,0},//15
					{H_15|H_18,{2,H_00/*rtv.Rh*/},H_18,0},//16
					{H_18|H_19,{1,H_00/*rtv.tV19*/},H_18,0},//17
					{H_17|H_18,{0,H_14},H_00,0},//18

					{H_00,{4,H_00},H_00,0}  //19    Произвести фиксацию замеров для левого
				};
	 		   	mainScenarioMulti[1].wait.value = gRegisters[0x03] - gRegisters[0x02];
	 		    mainScenarioMulti[6].wait.value = gRegisters[0x03];
	 		    mainScenarioMulti[7].wait.value = gRegisters[0x00];// v10


				mainScenarioMulti[11].wait.value = gRegisters[0x05] - gRegisters[0x04];
				mainScenarioMulti[16].wait.value = gRegisters[0x05];
				mainScenarioMulti[17].wait.value = gRegisters[0x01]; //V19

				currentScenario = mainScenarioMulti;
				stepsInCirce = 20;

	 		   finished = 0;
	 		   sstep = 0;
	 		   todo = 1;
	 		   gRegisters[0x30] = 0;
	 		   gRegisters[0x0c] = 0;
	 		   gRegisters[0x0d] = 0;
	 		   gRegisters[0x0e] = 0;
	 		   gRegisters[0x0f] = 0;
				 break;
			 }
		 }
	 	}
		if(gRegisters[0x20]==0x80) {// Stop Signal
			currentScenario = NULL;
			sstep = 0;
			todo = 0;
			finished = 1;
			gRegisters[0x20] = 0;
		}
		if(gRegisters[0x20]==0x81) {// Stop Signal
			currentScenario = NULL;
			sstep = 0;
			todo = 0;
			finished = 1;
			gRegisters[0x20] = 0x24;
		}
		 /**************************************************************************
 		 * program checks and locks section
 		 **************************************************************************/

		 //if(!gRegisters[0x1f])ledn(0x10,sstep);

		 if(todo && (di_data&H_16) ){
		 //if(todo){
			 if((currentScenario==0)||(currentScenario==NULL))continue;
			 //ledn(16,stepsInCirce);

			 currentStep = currentScenario[sstep];
			 ret = doCommand(currentStep);
			 if(ret)exception(ret);
			 ledn(0,sstep);
			 //Delay(50);

			 sstep++;
			 if(sstep >= stepsInCirce){ // сбрасываем счетчик для повтора команды
				 sstep = 0;
			 }
		 }

		 //if(currentScenario!=0)ledn(16,currentScenario[sstep].wait.type*256+sstep*16+sstage);
		 /**************************************************************************
 		 * command section
 		 **************************************************************************/

		 /**************************************************************************
 		 * external control section
 		 **************************************************************************/
		 //counter++;
		 //gRegisters[0x26] = (int)(iTimeout%65536);
		 // read Panel

		 //DelayMs(80);
		 RefreshWDT();

		//RefreshWDT();
		 //
	}
	Return:
		//ret = -ret;
		//ret+=sstep*16;
		//ledn(0xe,-ret);
		//Delay(1200);
		deinitInw();
    	return;
	//
}
