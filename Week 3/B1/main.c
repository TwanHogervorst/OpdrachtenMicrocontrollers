/*
 * B1.c
 *
 * Created: 16-2-2022 10:45:35
 * Author : twanh
 */ 

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>

#include "utils.h"
#include "lcd.h"

char* numCharLookup = "0123456789ABCDEF";

int main(void)
{
		
	// Initialize registers
	DDRD &= ~0x80; // DDRD.7 as input, else is output
	DDRB = 0xFF; // DDRB all output
	
	// Initialize timer2 as counter, rising edge
	TCCR2 = 0b00000111;
	
	// Wait for LCD to initialize
	wait(100);
	
	lcd_init();
	lcd_display_text("Timer!");
	
	lcd_set_cursor(LCD_LINE_2);
	lcd_display_text("0x");
	
	char counterValue;
	char* numText = "00";
	while(1) {
		// Update counterValue
		// Update PORTB for debug reasons
		counterValue = PORTB = TCNT2;
		
		// Get hexadecimal string
		numText[0] = numCharLookup[(counterValue >> 4) & 0x0F];
		numText[1] = numCharLookup[counterValue & 0x0F];
		
		// Show on lcd
		lcd_set_cursor(LCD_LINE_2 + 2);
		lcd_display_text(numText);
		
		wait(50);
	}
}
