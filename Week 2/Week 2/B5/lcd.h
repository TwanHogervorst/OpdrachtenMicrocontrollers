/*
 * lcd.h
 *
 * Created: 9-2-2022 12:42:27
 *  Author: twanh
 */ 


#ifndef LCD_H_
#define LCD_H_

#define LCD_E_PIN 6
#define LCD_RS_PIN 4
//#define LCD_RST_PIN 7

#define LCD_LINE_1 0
#define LCD_LINE_2 40

#ifdef LCD_LED_ON
	#define LCD_CMD_DELAY 10
#else
	#define LCD_CMD_DELAY 1
#endif

void init();
void display_text(char *str);
void set_cursor(int position);

#endif /* LCD_H_ */