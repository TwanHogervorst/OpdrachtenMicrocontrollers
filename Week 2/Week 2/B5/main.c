/*
 * main.c
 *
 * Created: 2/9/2022 12:38:49 PM
 *  Author: twanh
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "lcd.h"

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}

int main(void)
{
	
	wait(1000);
		
    init();
	
	display_text("Welcome!");
	
	char* alphabet = "abcdefghijklmnopqrstuvwxyz";
	int alphabetLength = 26; // strlen(alphabet);
	int i = -1;
	
	char textToDisplay[] = { 0x00, 0x00 };
	
	while(1) {
		i = (i + 1) % alphabetLength;
		textToDisplay[0] = alphabet[i];
		
		set_cursor(LCD_LINE_2);
		display_text(textToDisplay);
		wait(500);
	}
}