#include "prj_common.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include<compat/deprecated.h>
#include<avr/eeprom.h>
#include "Timer0.h"
#include "Dio_h.h"
#include "LCD_H.h"
#include "SW_TIMER.h"
#include "Switch_h.h"
uint8_t Motor_Command=MOTOR_NO_COMMAND;
static uint8_t Switch_state=0;
static uint8_t Switch_Debounce_Counter=0;


void switch_init(void);
void Clear_SwitchStatus(void);
void switch_service(void);
uint8_t Get_Switch_Status(void);

void switch_init(void)
{

DDRB &= 0xFC;//PB0-Motor start,PB1-MOtor stop
PORTB= 0;
}

void Clear_SwitchStatus(void)
{
Motor_Command=0;
Switch_state=0;
Switch_Debounce_Counter=0;
}

uint8_t Get_Switch_Status(void)
{
return Motor_Command;

}
void switch_service(void)
{
uint8_t a;
static uint8_t SwitchOldstatus=0;
uint8_t SwitchNewState=0;
a=PINB;
		a &= 0x03;
		if(a)
		{
			SwitchNewState=a;
		}
		else
		{
			SwitchNewState=0;

		}


	if(SwitchOldstatus ==SwitchNewState)
	{
		if(SwitchNewState)
		{
			Switch_Debounce_Counter++;
			if(	Switch_Debounce_Counter >2)//20ms debounce time
			{
			
			Switch_state =SwitchNewState;
			Start_Timer(TIMER_3,5000);
			}
		}
		//else
		//Switch_Debounce_Counter=0;
	}
	else
	{
		Switch_Debounce_Counter=0;
		//Switch_state=0;
	}



switch(Switch_state)
{
case 1:Motor_Command =MOTOR_START;
lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne | 10);
lcd_write_string_4d("START");
break;
case 2:Motor_Command =MOTOR_STOP;
lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne | 10);
lcd_write_string_4d("STOP ");
break;
case 3://Motor_Command =MOTOR_NO_COMMAND;
default:Motor_Command =MOTOR_NO_COMMAND;

lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne | 10);
lcd_write_string_4d("     ");
break;

}
	
if (GetTimer_State(TIMER_3)==TIMER_EXPIRED)
{
	Clear_SwitchStatus();
	Stop_Timer(TIMER_3);
}

SwitchOldstatus = SwitchNewState;
}
