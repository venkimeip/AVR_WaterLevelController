
/*
 * Atmega16_TI_RF.c
 *
 * Created: 18-10-2014 AM 11:35:01
 *  Author: Home
 */ 
#define SPI_Data_Byte 0x55
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
#define MODBUS_SLAVE_ID 1
#define MB_RX_ENABLE() { PORTB &=0x7F;}//PORTB.7 ->Modbus directoin selector
#define MB_TX_ENABLE() { PORTB |=0x80;}
//#include "Spi.h" 
#include "Uart.h" 
#include "Switch_h.h"
#define ADC_CH_TH 500 // Threshold detection counts

void ADC_ReadDisp_All(void);
uint8_t ADC_Debounce_ChNo[8];
uint8_t ADC_Debounce_State;
uint8_t Sec_Counter='0';
uint16_t Adc_result_u16=0;
uint8_t a=0,i,j,k,l;
uint16_t Received_crc_Val=0;
uint16_t Calculated_crc_Val=0;
uint16_t MB_Requested_Length=0;
uint8_t Modbus_TxBuff[20];

int main(void)
{
	


	switch_init();
	DDRC=0x3F;
	PORTC=0x55;
	DDRB|=0x80;//PB.7 -> Modbus direction control : Output
MB_RX_ENABLE();

#if 1
	Uart_init(12);//9600 @ 1MHz Internal clock
	/*
	baudrate selection
	9600 - > 12
	4800-> 25
	*/
	

	InitTimer0();

	ADC_init();
//	RTC_init();
	
// initialize the LCD controller as determined by the defines (LCD instructions)
    lcd_init_4d();                                  // initialize the LCD display for a 4-bit interface

// display the first line of information
    //lcd_write_string_4d("Hello World");
Uart_TxChar('A');
Uart_TxChar('V');
Uart_TxChar('R');
#endif
init_SWTimer();
_delay_ms(500);
Start_Timer(TIMER_0,1000);
Start_Timer(TIMER_2,10);
Start_Timer(TIMER_3,5000);
Start_Timer(TIMER_4,100);
sei(); // Enable global interrupt
	    while(1)
    {
		
		
	if(GetTimer_State(TIMER_2)==TIMER_EXPIRED)
	{
	
	switch_service();
	Start_Timer(TIMER_2,10);
	
	}	
		
			
		//lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne | 15);
		//_delay_ms(1);
		//lcd_write_character_4d(Rx_Char);
		
#if 1		
if (GetTimer_State(TIMER_0)==TIMER_EXPIRED)
		{	Sec_Counter++;
			if (Sec_Counter>'9')
			{
			Sec_Counter='0';
			}
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne | 15);
			lcd_write_character_4d(Sec_Counter);//Ref1msCounter);
			Start_Timer(TIMER_0,1000);

			if (GetTimer_State(TIMER_4)==TIMER_RUNNING)
			{
				lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne | 0);
				lcd_write_string_4d("ONLINE ");
			}
			else
			{

				lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne | 0);
				lcd_write_string_4d("OFFLINE");
			}




		}

#endif
ADC_ReadDisp_All();
//process modbus frame


if (GetTimer_State(TIMER_1)==TIMER_EXPIRED)	
{
	Received_crc_Val = (Mbus_RxBuff[7] <<8) | Mbus_RxBuff[6];
	Calculated_crc_Val= get_crc16(Mbus_RxBuff,6);
	MB_Requested_Length = (Mbus_RxBuff[4] <<8) | Mbus_RxBuff[5];
	if((Mbus_RxBuff[0]==MODBUS_SLAVE_ID) && (Received_crc_Val ==Calculated_crc_Val)&&(MB_Requested_Length==2))
	
	{	
		
		
		Modbus_TxBuff[0]=MODBUS_SLAVE_ID;
		Modbus_TxBuff[1]=Mbus_RxBuff[1];
		Modbus_TxBuff[2]=4;//Length
		Modbus_TxBuff[3]=0;//Data high1
		Modbus_TxBuff[4]=ADC_Debounce_State;//data low1
		Modbus_TxBuff[5]=0;//data high 2
		Modbus_TxBuff[6]=Get_Switch_Status();
		
		Calculated_crc_Val= get_crc16(Modbus_TxBuff,7);
		Modbus_TxBuff[7]=Calculated_crc_Val;
		Modbus_TxBuff[8]=Calculated_crc_Val>>8;
		
		MB_TX_ENABLE();
		_delay_ms(1);
		for(i=0;i<9;i++)
		{
		Uart_TxChar(Modbus_TxBuff[i]);
		}
		_delay_ms(1);
		MB_RX_ENABLE();

		Start_Timer(TIMER_4,3000);
	}
	else
	{
		//Uart_TxChar(0xFF);//Error
	}
	Mbus_RxBuff_Counter=0;
Stop_Timer(TIMER_1);
} 






    }
}

//---------------------------------------------
void ADC_ReadDisp_All(void)
{
//ADC CH0
		

		Adc_result_u16 = ADC_Read_CH(0);
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo | 13);
		i=Adc_result_u16/100;
		lcd_write_character_4d(i | 0x30);//Disp First char
		
		i=Adc_result_u16 % 100;
		j=i/10;
		lcd_write_character_4d(j | 0x30);//Disp First char
		j=i%10;
		lcd_write_character_4d(j | 0x30);//Disp First char

		
		if(Adc_result_u16 > ADC_CH_TH)
		{	
			 ADC_Debounce_ChNo[0]++;
			if(ADC_Debounce_ChNo[0] > 10)
			{
				ADC_Debounce_State |= 0x01;
				ADC_Debounce_ChNo[0]=0;
			}
			else
			{
			//wait
			}
		 //PORTB=k;
		}
		else
		{
			ADC_Debounce_ChNo[0]=0;
			ADC_Debounce_State &= ~0x01;
		}
		
		
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo | 0);
		_delay_ms(1);
		if(ADC_Debounce_State & 0x01)
		lcd_write_character_4d('*');
		else
		lcd_write_character_4d(' ');
		
		
//ADC CH1

	Adc_result_u16 = ADC_Read_CH(1);
		if(Adc_result_u16 > ADC_CH_TH)
		{	
			 ADC_Debounce_ChNo[1]++;
			if(ADC_Debounce_ChNo[1] > 10)
			{
				ADC_Debounce_State |= 0x02;
				ADC_Debounce_ChNo[1]=0;
			}
			else
			{
			//wait
			}
		 //PORTB=k;
		}
		else
		{
			ADC_Debounce_ChNo[1]=0;
			ADC_Debounce_State &= ~0x02;
		}
		
		
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo | 1);
		_delay_ms(1);
		if(ADC_Debounce_State & 0x02)
		lcd_write_character_4d('*');
		else
		lcd_write_character_4d(' ');
		
//-------------------------------------------------------------
//ADC CH2

	Adc_result_u16 = ADC_Read_CH(2);
		if(Adc_result_u16 > ADC_CH_TH)
		{	
			 ADC_Debounce_ChNo[2]++;
			if(ADC_Debounce_ChNo[2] > 10)
			{
				ADC_Debounce_State |= 0x04;
				ADC_Debounce_ChNo[2]=0;
			}
			else
			{
			//wait
			}
		 //PORTB=k;
		}
		else
		{
			ADC_Debounce_ChNo[2]=0;
			ADC_Debounce_State &= ~0x04;
		}
		
		
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo | 2);
		_delay_ms(1);
		if(ADC_Debounce_State & 0x04)
		lcd_write_character_4d('*');
		else
		lcd_write_character_4d(' ');		
//-------------------------------------------------------------
//ADC CH3

	Adc_result_u16 = ADC_Read_CH(3);
		if(Adc_result_u16 > ADC_CH_TH)
		{	
			 ADC_Debounce_ChNo[3]++;
			if(ADC_Debounce_ChNo[3] > 10)
			{
				ADC_Debounce_State |= 0x08;
				ADC_Debounce_ChNo[3]=0;
			}
			else
			{
			//wait
			}
		 //PORTB=k;
		}
		else
		{
			ADC_Debounce_ChNo[3]=0;
			ADC_Debounce_State &= ~0x08;
		}
		
		
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo | 3);
		_delay_ms(1);
		if(ADC_Debounce_State & 0x08)
		lcd_write_character_4d('*');
		else
		lcd_write_character_4d(' ');
		//-------------------------------------------------------------
//ADC CH4

	Adc_result_u16 = ADC_Read_CH(4);
		if(Adc_result_u16 > ADC_CH_TH)
		{	
			 ADC_Debounce_ChNo[4]++;
			if(ADC_Debounce_ChNo[4] > 10)
			{
				ADC_Debounce_State |= 0x10;
				ADC_Debounce_ChNo[4]=0;
			}
			else
			{
			//wait
			}
		 //PORTB=k;
		}
		else
		{
			ADC_Debounce_ChNo[4]=0;
			ADC_Debounce_State &= ~0x10;
		}
		
		
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo | 4);
		_delay_ms(1);
		if(ADC_Debounce_State & 0x10)
		lcd_write_character_4d('*');
		else
		lcd_write_character_4d(' ');
//--------------------------------------------------------------

//ADC CH5

	Adc_result_u16 = ADC_Read_CH(5);
		if(Adc_result_u16 > ADC_CH_TH)
		{	
			 ADC_Debounce_ChNo[5]++;
			if(ADC_Debounce_ChNo[5] > 10)
			{
				ADC_Debounce_State |= 0x20;
				ADC_Debounce_ChNo[5]=0;
			}
			else
			{
			//wait
			}
		 //PORTB=k;
		}
		else
		{
			ADC_Debounce_ChNo[5]=0;
			ADC_Debounce_State &= ~0x20;
		}
		
		
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo | 5);
		_delay_ms(1);
		if(ADC_Debounce_State & 0x20)
		lcd_write_character_4d('*');
		else
		lcd_write_character_4d(' ');
		//--------------------------------------------------------------

//ADC CH6

	Adc_result_u16 = ADC_Read_CH(6);
		if(Adc_result_u16 > ADC_CH_TH)
		{	
			 ADC_Debounce_ChNo[6]++;
			if(ADC_Debounce_ChNo[6] > 10)
			{
				ADC_Debounce_State |= 0x40;
				ADC_Debounce_ChNo[6]=0;
			}
			else
			{
			//wait
			}
		 //PORTB=k;
		}
		else
		{
			ADC_Debounce_ChNo[6]=0;
			ADC_Debounce_State &= ~0x40;
		}
		
		
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo | 6);
		_delay_ms(1);
		if(ADC_Debounce_State & 0x40)
		lcd_write_character_4d('*');
		else
		lcd_write_character_4d(' ');
//--------------------------------------------------------------


//ADC CH7

	Adc_result_u16 = ADC_Read_CH(7);
		if(Adc_result_u16 > ADC_CH_TH)
		{	
			 ADC_Debounce_ChNo[7]++;
			if(ADC_Debounce_ChNo[7] > 10)
			{
				ADC_Debounce_State |= 0x80;
				ADC_Debounce_ChNo[7]=0;
			}
			else
			{
			//wait
			}
		 //PORTB=k;
		}
		else
		{
			ADC_Debounce_ChNo[7]=0;
			ADC_Debounce_State &= ~0x80;
		}
		
		
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo | 7);
		_delay_ms(1);
		if(ADC_Debounce_State & 0x80)
		lcd_write_character_4d('*');
		else
		lcd_write_character_4d(' ');
	//--------------------------------------------------------------						
		//While ends
}
