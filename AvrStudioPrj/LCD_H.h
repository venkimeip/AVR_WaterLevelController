#ifndef LCD_MODE4_H
#define LCD_MODE4_H
//filename :  #include "LCD_H.h"
// LCD interface (should agree with the diagram above)
//   make sure that the LCD RW pin is connected to GND
#define lcd_D7_port     PORTD                   // lcd D7 connection
#define lcd_D7_bit      7//PORTD7
#define lcd_D7_ddr      DDRD

#define lcd_D6_port     PORTD                   // lcd D6 connection
#define lcd_D6_bit      6//PORTD6
#define lcd_D6_ddr      DDRD

#define lcd_D5_port     PORTD                   // lcd D5 connection
#define lcd_D5_bit      5//PORTD5
#define lcd_D5_ddr      DDRD

#define lcd_D4_port     PORTD                   // lcd D4 connection
#define lcd_D4_bit      4//PORTD4
#define lcd_D4_ddr      DDRD

#define lcd_E_port      PORTD                   // lcd Enable pin
#define lcd_E_bit       3//PORTD3
#define lcd_E_ddr       DDRD

#define lcd_RS_port     PORTD                   // lcd Register Select pin
#define lcd_RS_bit      2//PORTD2
#define lcd_RS_ddr      DDRD

// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2
//#define   lcd_LineThree   0x14                  // start of line 3 (20x4)
//#define   lcd_lineFour    0x54                  // start of line 4 (20x4)
//#define   lcd_LineThree   0x10                  // start of line 3 (16x4)
//#define   lcd_lineFour    0x50                  // start of line 4 (16x4)

// LCD instructions
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position

extern void lcd_write_4(uint8_t);
extern void lcd_write_instruction_4d(uint8_t);
extern void lcd_write_character_4d(uint8_t);
extern void lcd_write_string_4d(uint8_t *);
extern void lcd_init_4d(void);
#endif
