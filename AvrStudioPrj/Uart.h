#ifndef _UART_H
#define _UART_H

//#define
#define Mbus_RxBuff_SIZE 10

extern volatile uint8_t Mbus_RxBuff[Mbus_RxBuff_SIZE];
extern volatile uint8_t Mbus_RxBuff_Counter;

extern volatile unsigned char Rx_Char;
void Uart_init(unsigned int ubrr_value);
void Uart_TxChar( unsigned char data);
unsigned char Uart_RcChar(void);
extern uint16_t get_crc16(uint8_t *buff,uint8_t len);
//ISR(USART_RXC_vect);
#endif
