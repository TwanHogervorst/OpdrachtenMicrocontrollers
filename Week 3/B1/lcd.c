/*
 * lcd.c
 *
 * Created: 9-2-2022 12:42:11
 *  Author: twanh
 */ 

#include <avr/io.h>

#include "utils.h"
#include "lcd.h"

void lcd_command(unsigned char command);
void lcd_data(unsigned char data);
void lcd_write_nibble(unsigned char nibble);

void lcd_reset() {
	// Pull RST-Pin high
	PORTA |= 1 << LCD_RST_PIN;
	wait(100);
	
	// Pull RS-Pin low
	PORTA |= 1 << LCD_RS_PIN;
	wait(10);
}

void lcd_init() {
	DDRA = 0xFF;
	DDRC = 0xFF;
	
	PORTA = 0x00;
	PORTC = 0x00;
	
	// Wait for lcd to boot
	wait(100);
	
	// Function Set (use lcd_write_nibble, see documentation of method)
	//	=> DL: 0 (4-bit), N discard, F discard
	lcd_write_nibble(0x02);
	
	// Function Set Again, for two line mode
	//  => DL: 0 (4-bit), N 1 (2 lines), F 0 (default font, 5x8)
	lcd_command(0x28);
	
	// Clear display and wait
	lcd_command(0x01);
	wait(10);
	
	// Set cursor to start
	lcd_set_cursor(0);

	// Display on
	//  => D: 1 (on), C: 1 (cursor), B: 1 (blink)
	lcd_command(0x0F);
	
	// Entry mode set
	//  => I/D: 1 (increment), S: 0 (shift-right)
	lcd_command(0x06);
}

void lcd_display_text(char *str) {
	for(int i = 0; str[i]; i++) {
		lcd_data(str[i]);
	}
}

void lcd_set_cursor(int position) {
	// Set cursor to home and wait (see documentation)
	lcd_command(0x02);
	wait(5);
	
	while(position--) {
		lcd_command(0x14);
	}
}

void lcd_command(unsigned char command) {
	// Pull RS-Pin low
	PORTA &= ~(1 << LCD_RS_PIN);
	
	// Send upper nibble
	lcd_write_nibble(command >> 4);
	// Send lower nibble
	lcd_write_nibble(command);
}

void lcd_data(unsigned char data) {
	// Pull RS-Pin high
	PORTA |= 1 << LCD_RS_PIN;
	
	// Send upper nibble
	lcd_write_nibble(data >> 4);
	// Send lower nibble
	lcd_write_nibble(data);
}

/**
 * \brief Writes the lower part of the nibble param to the LCD-display
 *			(as the higher nibble of PORTC)
 * \param nibble
 * 
 * \return void
 */
void lcd_write_nibble(unsigned char nibble) {
	PORTC = (0x0F & nibble) << 4;
	
	PORTA |= (1 << LCD_E_PIN);
	wait(2);
	PORTA &= ~(1 << LCD_E_PIN);
	wait(2);
}
