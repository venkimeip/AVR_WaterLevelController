#ifndef _TIMER0_ISR
#define _TIMER0_ISR

extern volatile unsigned int Ref1msCounter;
extern volatile unsigned int SWTimer_Ref1msCounter;
void InitTimer0(void);
void RTC_init(void);
//ISR(TIMER0_OVF_vect);
ISR(TIMER2_OVF_vect);
#endif
