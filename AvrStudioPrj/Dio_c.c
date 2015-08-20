#include "prj_common.h"
#include "Dio_h.h"
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
//#include "Spi.h" 
#include "Uart.h" 
//function prototypes

void GLCD_delay(unsigned int val);
void GLCD_init(void);
void GLCD_Command(unsigned char cmd);
void GLCD_Data(unsigned char data);
void GLCD_On(void);
uint16_t ADC_Read_CH(uint8_t channelNo);
void ADC_init(void);

void ADC_init(void)
{
//DDRA = 0x00;//All Inputs
//PORTA=0x00;
ADMUX =0;
ADCSRA = (1<<ADEN)|(1<<ADPS2);

}

uint16_t ADC_Read_CH(uint8_t channelNo)
{


ADMUX = (ADMUX & 0xF8)|channelNo; // clears the bottom 3 bits before ORing
 
  // start single convertion
  // write ’1' to ADSC
  ADCSRA |= (1<<ADSC);
 
  // wait for conversion to complete
  // ADSC becomes ’0' again
  // till then, run loop continuously
  while(ADCSRA & (1<<ADSC));
 
  return (ADC);


}

unsigned char GLCD_Status(void)
{


}



void GLCD_On(void)
{
GLCD_CS1_LOW();
GLCD_CS2_LOW()
GLCD_RW_LOW();
GLCD_RS_LOW();
GLCD_PORT = 0x3F;
GLCD_delay(GLCD_STD_DELAY);
GLCD_EN_HIGH();
GLCD_delay(GLCD_STD_DELAY);

GLCD_delay(GLCD_STD_DELAY);
GLCD_delay(GLCD_STD_DELAY);
GLCD_EN_LOW();
}


void GLCD_Command(unsigned char cmd)
{
//GLCD_EN_LOW();
//GLCD_delay(GLCD_STD_DELAY);
GLCD_RW_LOW();
GLCD_RS_LOW();
GLCD_PORT = cmd;
//GLCD_delay(GLCD_STD_DELAY);
GLCD_EN_HIGH();
//GLCD_delay(GLCD_STD_DELAY);

GLCD_delay(GLCD_STD_DELAY);
GLCD_delay(GLCD_STD_DELAY);
GLCD_EN_LOW();
GLCD_delay(GLCD_STD_DELAY);
//GLCD_delay(GLCD_STD_DELAY);
//GLCD_delay(GLCD_STD_DELAY);
GLCD_EN_HIGH();
GLCD_RS_HIGH();
GLCD_RW_HIGH();
//GLCD_CS1_LOW();
//GLCD_CS2_LOW();
GLCD_delay(GLCD_STD_DELAY);
//GLCD_delay(GLCD_STD_DELAY);
}

void GLCD_Data(unsigned char data)
{
//GLCD_EN_LOW();
//GLCD_delay(GLCD_STD_DELAY);
GLCD_RW_LOW();
GLCD_RS_HIGH();
GLCD_PORT = data;
//GLCD_delay(GLCD_STD_DELAY);
GLCD_EN_HIGH();
GLCD_delay(GLCD_STD_DELAY);

GLCD_delay(GLCD_STD_DELAY);
GLCD_delay(GLCD_STD_DELAY);
GLCD_EN_LOW();
GLCD_delay(GLCD_STD_DELAY);
//GLCD_delay(GLCD_STD_DELAY);
GLCD_EN_HIGH();
GLCD_RS_HIGH();
GLCD_RW_HIGH();
//GLCD_CS1_LOW();
//GLCD_CS2_LOW();
GLCD_delay(GLCD_STD_DELAY);
GLCD_delay(GLCD_STD_DELAY);
}



void GLCD_init(void)
{
unsigned char index=0;
//init ports
DDRA=0xFF;
DDRD=0xFC;
PORTA=0;
PORTD=0;

//init pin states
GLCD_RS_LOW();
GLCD_RW_LOW();
GLCD_EN_HIGH();
GLCD_CS1_HIGH();
GLCD_CS2_LOW();
GLCD_RST_HIGH();



//GLCD_Command(0x3F);
GLCD_delay(GLCD_STD_DELAY);
GLCD_Command(0x40);//set y axis page
GLCD_delay(GLCD_STD_DELAY);
GLCD_Command(0xB8 | 0);//x axis Page number 0-7
//GLCD_Data();
for (index=0;index<64;index++)
{	GLCD_Data(0x0F);
	//GLCD_delay(GLCD_STD_DELAY);	
 } 
//GLCD_CS1_HIGH();
//GLCD_CS2_HIGH();
}

void GLCD_delay(unsigned int val)
{
unsigned int i=0,j=0,k=0,l;

	for(k=0;k<val;k++)
	{
		for(i=0;i<1;i++)
		{
			for(j=0;j<50;j++)//50
			{
				l++;
			}
		}

	}

return;
}
