/*
 * main.c
 *
 * Created: 2/2/2022 11:00:31 AM
 *  Author: twanh
 */ 

#include <xc.h>

int main(void)
{
	DDRD = 0xFF; // Set PORTD to output
	PORTD = 0xAA; // Turn some leds on
	
    while(1)
    {
		
    }
	
	return 0;
}