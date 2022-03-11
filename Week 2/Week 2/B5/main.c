/*
 * main.c
 *
 * Created: 2/9/2022 12:38:49 PM
 *  Author: twanh
 */ 

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>

#include "utils.h"
#include "lcd.h"

int main(void)
{
	
	lcd_reset();
	
	// Wait for the lcd to reset
	wait(100);
		
    lcd_init();
	
	lcd_display_text("Welcome!");
	
	char* alphabet = "abcdefghijklmnopqrstuvwxyz";
	int alphabetLength = 26; // strlen(alphabet);
	int i = -1;
	
	char textToDisplay[] = { 0x00, 0x00 };
	
	while(1) {
		i = (i + 1) % alphabetLength;
		textToDisplay[0] = alphabet[i];
		
		lcd_set_cursor(LCD_LINE_2);
		lcd_display_text(textToDisplay);
		wait(500);
	}
}