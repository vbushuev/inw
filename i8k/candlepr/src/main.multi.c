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
		 diff = di_data&H_06;
		 if(diff==H_06)clearEncoder(0); // нижний поршень в верхнем положении левый(0)
		 diff = di_data&H_12;
		 if(diff==H_12)clearEncoder(1); // нижний поршень в верхнем положении правый(1)
		 diff = 0;s
		 */
		 // Read Encoders
		 ret = Encoder(0,&lenc);
		 if(ret!=0)exception(ret);
		 //ledn(0xe,enc_data);
		 ret = Encoder(1,&renc);
		 if(ret!=0)exception(ret);
		 // read Panel
		 readModbusRTU();
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
					{H_03,{0,H_03},H_00,0},//00
					{H_12,{0,H_09},H_00,0},//01
					{H_07,{0,H_07},H_00,0},//02
					{H_17,{0,H_14},H_00,0},//03
					//{H_05,{0,H_06},H_00,0},//04
					//{H_14,{0,H_12},H_00,0},//05
					{H_06,{1,H_00/*rtv.Lh*/},H_00,0},//04
					{H_15,{1,H_00/*rtv.Rh*/},H_00,0},//05
					{H_01,{0,H_15},H_00,0},//06
					{H_00,{5,H_00},H_00,0},//07
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
					{H_05,{0,H_06},H_00,0},//00
					{H_06,{2,H_00/*rtv.Lh*/},H_00,0},//00
					{H_01,{0,H_01},H_00,0},//01
					{H_00,{5,H_00},H_00,0} //02 finish
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
					{H_02,{0,H_02},H_00,0},//00
				  	{H_04,{0,H_04},H_00,0},//01
				  	{0x00000410,{2,H_00/*rtv.LC*/},H_00,0},//02
					{H_04,{0,H_05},H_00,0},//03
					{H_03,{0,H_03},H_00,0},//04
					{H_08,{0,H_08},H_00,0},//05
				  	{0x00000410,{0,H_06},H_00,0},//06
				  	{H_09,{1,0x000021a6/*rtv.ttaker*/},H_09,0},//07
				  	{0x00000120,{2,0x00002186/*rtv.Lh*/},H_09,0},//8
				  	{0x00000300,{1,0x00002186/*rtv.tV10*/},H_09,0},//9
				  	{0x00000140,{0,H_07},H_00,0},//10
				  	{H_01,{0,H_01},H_00,0},//11

				  	{H_00,{3,H_00},H_00,0},  //12    Произвести фиксацию замеров для левого

				  	{H_13,{0,H_10},H_00,0},//13
				  	{0x00082000,{2,H_00/*rtv.RC*/},H_00,0},//14
				  	{H_13,{0,H_11},H_00,0},//15
				  	{H_12,{0,H_09},H_00,0},//16
				  	{H_16,{0,H_13},H_00,0},//17
				  	{0x00082000,{0,H_12},H_00,0},//18
				  	{H_18,{1,H_00/*rtv.ttaker*/},H_18,0},//19
				  	{0x00024000,{2,H_00/*rtv.Rh*/},H_18,0},//20
				  	{0x00060000,{1,H_00/*rtv.tV19*/},H_18,0},//21
				  	{0x00030000,{0,H_14},H_00,0},//22

				  	{H_00,{4,H_00},H_00,0},  //23    Произвести фиксацию замеров для левого
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
						 {H_01,{0,H_01},H_00,0},
				 		 {H_00,{5,H_00},H_00,0},  //23    Произвести фиксацию замеров для левого
					 };
					 gRegisters[0x30] = 0;
					 gRegisters[0x08] = 1;

					 gRegisters[0x0c] = 0;
					 gRegisters[0x0d] = 1;
					 gRegisters[0x0e] = 0;
					 gRegisters[0x0f] = 0;
					 switch (gRegisters[0x20]){
					 	case 0x07: {sStep sCommandStep = {H_01,{0,H_01},H_00,0};scenarioW[0]=sCommandStep;break;}
						case 0x08: {sStep sCommandStep = {H_01,{0,H_15},H_00,0};scenarioW[0]=sCommandStep;break;}
						case 0x09: {sStep sCommandStep = {H_02,{0,H_02},H_00,0};scenarioW[0]=sCommandStep;break;}
						case 0x0a: {sStep sCommandStep = {H_02,{0,H_15},H_00,0};scenarioW[0]=sCommandStep;break;}
						case 0x0b: {sStep sCommandStep = {H_05,{0,H_06},H_00,0};scenarioW[0]=sCommandStep;break;}
						case 0x0c: {sStep sCommandStep = {H_05,{2,H_05/*rtv.LC*/},H_00,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x02];break;}
						case 0x0d: {sStep sCommandStep = {H_06,{2,H_05/*rtv.LC*/},H_00,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x03];break;}

						case 0x0e: {sStep sCommandStep = {H_12,{0,H_09},H_00,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;break;}
						case 0x0f: {sStep sCommandStep = {H_13,{0,H_10},H_00,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;break;}
						case 0x10: {sStep sCommandStep = {H_13,{0,H_11},H_00,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;break;}
						case 0x11: {sStep sCommandStep = {H_14,{0,H_12},H_00,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;break;}
						case 0x12: {sStep sCommandStep = {H_14,{2,H_00/*rtv.RC*/},H_00,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;scenarioW[0].wait.value = gRegisters[0x04];break;}
						case 0x13: {sStep sCommandStep = {H_15,{2,H_00/*rtv.Rh*/},H_00,0};scenarioW[0]=sCommandStep;gRegisters[0x30]=1;scenarioW[0].wait.value = gRegisters[0x05];break;}

						case 0x14: {sStep sCommandStep = {H_07,{0,H_07},H_00,0};scenarioW[0]=sCommandStep;break;}
						case 0x15: {sStep sCommandStep = {H_08,{0,H_08},H_00,0};scenarioW[0]=sCommandStep;break;}
						case 0x16: {sStep sCommandStep = {H_17,{0,H_14},H_00,0};scenarioW[0]=sCommandStep;break;}
						case 0x17: {sStep sCommandStep = {H_16,{0,H_13},H_00,0};scenarioW[0]=sCommandStep;break;}

						case 0x18: {sStep sCommandStep = {H_03,{0,H_03},H_00,0};scenarioW[0]=sCommandStep;break;}
						case 0x19: {sStep sCommandStep = {H_04,{0,H_04},H_00,0};scenarioW[0]=sCommandStep;break;}
						case 0x1a: {sStep sCommandStep = {H_04,{0,H_05},H_00,0};scenarioW[0]=sCommandStep;break;}


						case 0x20: {sStep sCommandStep = {H_09,{1,H_00/*rtv.ttaker*/},H_00,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x0b]*1000;break;}
						case 0x21: {sStep sCommandStep = {H_18,{1,H_00/*rtv.ttaker*/},H_00,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x0b]*1000;break;}
						case 0x22: {sStep sCommandStep = {H_10,{1,H_00/*rtv.tV10*/},H_00,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x00]*1000;break;}
						case 0x23: {sStep sCommandStep = {H_19,{1,H_00/*rtv.tV19*/},H_00,0};scenarioW[0]=sCommandStep;scenarioW[0].wait.value = gRegisters[0x01]*1000;break;}

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
					{H_05,{0,H_06},H_00,0},//00
					{H_06,{2,H_00/*rtv.Lh*/},H_00,0},//00
					{H_00,{5,H_00},H_00,0} //02 finish
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
					{H_14,{0,H_12},H_00,0},//00
					{H_15,{2,H_00/*rtv.Rh*/},H_00,0},//01
					{H_00,{5,H_00},H_00,0} //02 finish
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
			 doCommand(currentScenario[sstep]);
			 sstep++;
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
				 gRegisters[0x34] = TimerReadValue() - gRegisters[0x34];
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
