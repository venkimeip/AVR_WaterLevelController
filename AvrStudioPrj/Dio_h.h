//filename : #include "Dio_h.h"

#ifndef _DIO_H
#define _DIO_H
#include <stdlib.h>
#include <avr/io.h>
#define GLCD_STD_DELAY 1

#define GLCD_PORT PORTA

#define GLCD_RS_MASK 0x04U 
#define GLCD_RW_MASK 0x08U
#define GLCD_EN_MASK 0x10U
#define GLCD_CS1_MASK 0x20U
#define GLCD_CS2_MASK 0x40U
#define GLCD_RST_MASK 0x80U

#define GLCD_RS_HIGH()	{GLCD_PORT |=GLCD_RS_MASK;} 
#define GLCD_RS_LOW()	{GLCD_PORT &= ~GLCD_RS_MASK;} 

#define GLCD_RW_HIGH()	{GLCD_PORT |=GLCD_RW_MASK;} 
#define GLCD_RW_LOW()	{GLCD_PORT &= ~GLCD_RW_MASK;} 

#define GLCD_EN_HIGH()	{GLCD_PORT |=GLCD_EN_MASK;} 
#define GLCD_EN_LOW()	{GLCD_PORT &= ~GLCD_EN_MASK;} 

#define GLCD_CS1_HIGH()	{GLCD_PORT |=GLCD_CS1_MASK;} 
#define GLCD_CS1_LOW()	{GLCD_PORT &= ~GLCD_CS1_MASK;} 

#define GLCD_CS2_HIGH()	{GLCD_PORT |=GLCD_CS2_MASK;} 
#define GLCD_CS2_LOW()	{GLCD_PORT &= ~GLCD_CS2_MASK;} 

#define GLCD_RST_HIGH()	{GLCD_PORT |=GLCD_RST_MASK;} 
#define GLCD_RST_LOW()	{GLCD_PORT &= ~GLCD_RST_MASK;} 



extern void GLCD_init(void);
extern void GLCD_delay(unsigned int val);
extern void GLCD_Command(unsigned char cmd);
extern void GLCD_Data(unsigned char data);
extern void GLCD_On(void);

extern uint16_t ADC_Read_CH(uint8_t channelNo);
extern void ADC_init(void);
#endif
