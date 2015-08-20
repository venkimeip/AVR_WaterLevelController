#include "prj_common.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include<compat/deprecated.h>
#include<avr/eeprom.h>
#include "Uart.h" 
#include "LCD_H.h"
#include "SW_TIMER.h"
#define poly 0xa001
volatile unsigned char Rx_Char='R';

volatile uint8_t Mbus_RxBuff[Mbus_RxBuff_SIZE];
volatile uint8_t Mbus_RxBuff_Counter=0;

void Uart_init(unsigned int ubrr_value)
{

	DDRD =1;
	  		 //Set Baud rate
  		 UBRRL = ubrr_value;
  		 UBRRH = (ubrr_value>>8);
  		 /*Set Frame Format
  		 >> Asynchronous mode
  		 >> No Parity
  		 >> 1 StopBit
  		 >> char size 8
  		 */
  		UCSRA= (1<<U2X);
		 UCSRC=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
  		 //Enable The receiver and transmitter
  		 UCSRB=(1<<RXEN)|(1<<TXEN)|(1 << RXCIE);
		// 	UCSR1B |= ; // Enable the USART Recieve Complete interrupt 
	
	
}


void Uart_TxChar( unsigned char data)
{
	//Wait untill the transmitter is ready

   		while(!(UCSRA & (1<<UDRE)));
   		//Now write the data to USART buffer
   		UDR=data;
}

unsigned char Uart_RcChar(void)
{
	while(!(UCSRA & (1<<RXC)));
   

   		//Now USART has got data from host
   		//and is available is buffer

  		 return UDR;
}


//SIGNAL (SIG_UART_RECV) 
ISR(USART_RXC_vect)
{ // USART RX interrupt
	unsigned char c;
	//Rx_Char=UDR;
	c = UDR;
	Rx_Char=c;
	//lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne | 0);
	//lcd_write_character_4d(UDR+1);
//Uart_TxChar(c+1);
//Uart_TxChar('1');
//reti();
if(Mbus_RxBuff_Counter <Mbus_RxBuff_SIZE)
{ 
	Mbus_RxBuff[Mbus_RxBuff_Counter++]=c;
}

Start_Timer(TIMER_1,MODBUS_FRAME_TIMEOUT_PERIOD);
}		

//crc
uint16_t get_crc16(uint8_t *buff,uint8_t len)
{
uint16_t creg;
uint8_t t1,t2;
creg=0xffff;//initial value
for(t1=0;t1<len;t1++)
{
	creg^=buff[t1];
	for(t2=1;t2<9;t2++)
	{
		if((creg%2))// if lsb==1
		{
			creg>>=1;
			creg^=poly;
		}
		else
		{
			creg>>=1;
			//creg>>=1;
		}
	} 
}

return creg;

}
