#include "funcs.h"
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
/*
 * Start scenario
 */
/*
 * Work scenario automate
 */
int loadMainScenario(sStep * sc){
     /*sStep scenarioW[]={
         {0x00000002,{0,0x00000002},0x00000000,0},  //00    убираем засыпку V2 -> A2
         {0x00000008,{0,0x00000008},0x00000000,0},  //01    верхний поршень прижимает V4 -> A4
         {0x00000410,{2,0},0x00000000,0},      //02    нижний поршень прижимает и отпускает фитиль(V11) V5->LS
         {0x00000008,{0,0x00000010},0x00000000,0},  //03    верхний дожимает V4 -> A5
         {0x00000004,{0,0x00000004},0x00000000,0},  //04    убираем вверхний V3->A3
         {0x00000080,{0,0x00000080},0x00000000,0},  //05    выдвигаем ухват V8->A8
         {0x00000410,{0,0x00000020},0x00000000,0},  //06    нижний поршень выдвигает и отпускает фитиль (V11) V5->A5
         {0x00000100,{1,0},0x00000100,0},  //07    зажимаем ухват и чуть ждем  V9
         {0x00000120,{2,0},0x00000100,0},      //08    опускаем нижний поршень V6->LH
         {0x00000300,{1,0},0x00000100,0},    //09    отрезаем свечу V10 на время tV10
         {0x00000140,{0,0x00000040},0x00000000,0},  //10    убираем ухват и отпускаем зажим V7->A7 V9=off
         {0x00000140,{0,0x00000040},0x00000000,0},  //11    ставим засыпаться левый цилиндр засыпку V1 -> A1

         {0x00000000,{3,0x00000000},0x00000000,0},  //12    Произвести фиксацию замеров для левого

         {0x00001000,{0,0x00000200},0x00000000,0},  //13    верхний поршень прижимает V13 -> A10
         {0x00082000,{2,0},0x00000000,0},      //14    нижний поршень прижимает и отпускает фитиль(V20) V14->RS
         {0x00001000,{0,0x00000400},0x00000000,0},  //15    верхний дожимает V13 -> A11
         {0x00000800,{0,0x00000100},0x00000000,0},  //16    убираем вверхний V12->A9
         {0x00008000,{0,0x00001000},0x00000000,0},  //17    выдвигаем ухват V16->A13
         {0x00002000,{0,0x00000800},0x00000000,0},  //18    нижний поршень выдвигает и отпускает фитиль (V20) V4->A12
         {0x00020000,{1,0},0x00020000,0},  //19    зажимаем ухват и чуть ждем  V18
         {0x00024000,{2,0},0x00020000,0},      //20    опускаем нижний поршень V15->RH
         {0x00060000,{1,0},0x00020000,0},    //21    отрезаем свечу V19 на время tV19
         {0x00030000,{0,0x00002000},0x00000000,0},  //22    убираем ухват и отпускаем зажим V17->A14 V18=off

         {0x00000000,{4,0x00000000},0x00000000,0}   //23    Произвести фиксацию замеров для левого
     };*/
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

		 {0x00000000,{4,0x00002145},0x00000000,0},  //12    Произвести фиксацию замеров для левого
	 };
     scenarioW[2].wait.value = gRegisters[0x02];
     scenarioW[7].wait.value = gRegisters[0x0b];
     scenarioW[8].wait.value = gRegisters[0x03];
     scenarioW[9].wait.value = gRegisters[0x00];
     scenarioW[14].wait.value = gRegisters[0x04];
     scenarioW[19].wait.value = gRegisters[0x0b];
     scenarioW[20].wait.value = gRegisters[0x05];
     scenarioW[21].wait.value = gRegisters[0x01];
     memcpy(sc,scenarioW,sizeof(scenarioW));
     return 0;
}
/*
 * Start scenario automate
 */
int loadStartScenario(sStep * sc){
	sStep scenarioW[]={
		{0x00000001,{0,0x00000001},0x00000000,0},//00
		{0x00000002,{0,0x00004000},0x00000000,0},//01
		{0x00000044,{0,0x00004044},0x00000000,0},//02
		{0x00000410,{0,0x00004064},0x00000000,0},//03
		{0x00000020,{2,/*rtv.Lh*/},0x00000000,0},//04
		{0x00000800,{0,0x00004144},0x00000000,0},//05
		{0x00012000,{0,0x00006944},0x00000000,0},//06
		{0x00014000,{2,/*rtv.Rh*/},0x00000000,0},//07
	 };
	 scenarioW[4].wait.value = gRegisters[0x03];
     scenarioW[7].wait.value = gRegisters[0x05];
	 memcpy(sc,scenarioW,sizeof(scenarioW)*sizeof(sStep));
     return 0;
 }
/*
 * Init scenario automate
 */
int loadInitScenario(sStep * sc){
	sStep scenarioW[]={
		{0x00000020,{2,0/*rtv.Lh*/},0x00000000,0},//00
		{0x00000001,{0,0x00002945},0x00000000,0},//01
		{0x00002000,{2,0x00002145/*rtv.Rh*/},0x00000000,0},//02
		{0x00000000,{1,0x00002145/*rtv.tstart*/},0x00000000,0},//03
	 };
	 scenarioW[0].wait.value = gRegisters[0x03];
     scenarioW[2].wait.value = gRegisters[0x05];
     scenarioW[3].wait.value = gRegisters[0x0a];
	 memcpy(sc,scenarioW,sizeof(scenarioW));
     return 0;
 }
