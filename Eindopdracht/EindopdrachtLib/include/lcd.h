/*
 * lcd.h
 *
 * Created: 29-3-2022 12:46:15
 *  Author: twanh
 */ 


#ifndef LCD_H_
#define LCD_H_

#define LCD_LINE_1 0
#define LCD_LINE_2 40

void lcd_reset();
void lcd_init();
void lcd_display_text(char *str);
void lcd_set_cursor(int position);

#endif /* LCD_H_ */