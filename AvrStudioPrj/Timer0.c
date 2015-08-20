#include "prj_common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Timer0.h"
volatile unsigned int Ref1msCounter=0;
volatile unsigned int SWTimer_Ref1msCounter=0;
void InitTimer0(void)

{
	//init timer0
	TCCR0= 2;//0x03;
	TCNT0=131;//83;
	OCR0=0;
	TIMSK=1;
	
	
	
}


ISR (TIMER0_OVF_vect) // 1ms ISR
{
	//Ref1msCounter++;
	Ref1msCounter++;
	SWTimer_Ref1msCounter++;
	TCNT0=131;//83; // reload timer value
	
}


void RTC_init(void)
{
	uint8_t sreg;/* Save status register and disable global interrupts */
	sreg=SREG;
	cli();	/* 0. Oscillator might take as long as one second to stabilize. */
	_delay_ms(1000);	/* 1. Disable the Timer/Counter2 interrupts by clearing OCIE2 and TOIE2. */
	TIMSK &= ~((1<<OCIE2)|(1<<TOIE2));	/* 2. Select clock source by setting AS2 as appropriate. */
	ASSR = (1<<AS2);	/* 3. Write new values to TCNT2, OCR2, and TCCR2. */
	TCNT2 =215;	// clear TCNT2A	// select precaler: 32.768 kHz / 128 = 1 sec between each overflow:
	TCCR2 = (1<<CS20);	/* 4. To switch to asynchronous operation: Wait for TCN2UB, OCR2UB, and
	TCR2UB. */
	while( (ASSR & (1<<TCN2UB)) | (ASSR & (1<<OCR2UB)) |
		(ASSR & (1<<TCR2UB)) ); 
	
	/* 5. Clear the Timer/Counter2 Interrupt Flags. */
	TIFR |= ((1<<OCF2)|(1<<TOV2));
	
	/* 6. enable Timer2 Overflow interrupt */
	TIMSK |= (1<<TOIE2);
    
	/* initial values */


	/* restore status-register */
	SREG=sreg;
	//Ref1msCounter='1';
}

ISR(TIMER2_OVF_vect)//interrupt every 1ms
{
TCNT2 =215;
//Ref1msCounter++;
//SWTimer_Ref1msCounter++;

}
