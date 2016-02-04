#include <icpdas.h>
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
int sendCommand(unsigned long command){
	DO_32(DOSLOT,command);
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
	InitLib();
	// check function
	//test_check();
	TimerOpen();
	ret = waitUntil(4L);
	if(!ret)Print("A4 occurs\r\n");
	else Print("Too long - no signal. Alarm on.\r\n");
	TimerClose();
}

