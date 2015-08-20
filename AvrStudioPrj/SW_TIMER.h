#ifndef _SW_TIMER
#define _SW_TIMER
#define SW_TIMER_COUNT 5

#define TIMER_0 0 // 1 Sec Periodic timer
#define TIMER_1 1 // Modbus timeout timer
#define TIMER_2 2 //10ms periodic timer
#define TIMER_3 3	//Switch state timeout timer
#define TIMER_4 4	//LCD Online indicator

#define TIMER_RUNNING 1
#define TIMER_STOPPED 2
#define TIMER_EXPIRED 3


extern volatile unsigned int TimerStart[SW_TIMER_COUNT];
extern volatile unsigned char TimerState[SW_TIMER_COUNT];
extern volatile unsigned int TimerTimeOutVal[SW_TIMER_COUNT];
extern void init_SWTimer(void);
extern void Start_Timer(unsigned char Timer_ID , unsigned int TimeoutVal);
extern unsigned char GetTimer_State(unsigned char TimerID);
extern void Stop_Timer(unsigned char TimerID);

#endif
