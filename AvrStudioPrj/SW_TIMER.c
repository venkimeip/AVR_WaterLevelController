#include "prj_common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "SW_TIMER.h"
#include "Timer0.h"

volatile unsigned int TimerStart[SW_TIMER_COUNT];
volatile unsigned char TimerState[SW_TIMER_COUNT];
volatile unsigned int TimerTimeOutVal[SW_TIMER_COUNT];
void init_SWTimer(void);
void Start_Timer(unsigned char Timer_ID , unsigned int TimeoutVal);
unsigned char GetTimer_State(unsigned char TimerID);
void Stop_Timer(unsigned char TimerID);

void init_SWTimer(void)
{
unsigned char counter=0;

	for (counter=0;counter <SW_TIMER_COUNT ; counter++)
	{
	TimerStart[counter]=0;
	TimerState[counter] =TIMER_STOPPED;
	TimerTimeOutVal[counter]=0;
	
	}


}

//---------------------------------------------------
void Start_Timer(unsigned char Timer_ID , unsigned int TimeoutVal)
{
TimerStart[Timer_ID] = SWTimer_Ref1msCounter;
TimerState[Timer_ID] =TIMER_RUNNING;
TimerTimeOutVal[Timer_ID] =TimeoutVal;
}

//---------------------------------------------------
unsigned char GetTimer_State(unsigned char TimerID)
{
if ( TimerState[TimerID] == TIMER_RUNNING )
    {
		if( (SWTimer_Ref1msCounter -TimerStart[TimerID]) > TimerTimeOutVal[TimerID] )
		TimerState[TimerID] =TIMER_EXPIRED;
	}
return TimerState[TimerID];
}
//---------------------------------------------------
void Stop_Timer(unsigned char TimerID) 
{
TimerState[TimerID] =TIMER_STOPPED;
}
