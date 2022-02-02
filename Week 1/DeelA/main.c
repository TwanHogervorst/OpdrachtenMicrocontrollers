/*
 * main.c
 *
 * Created: 2/2/2022 11:00:31 AM
 *  Author: twanh
 */ 

#include <xc.h>

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>


/******************************************************************/
void wait( int ms )
/* 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
*******************************************************************/
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{
	DDRD = 0xFF; // Set PORTD to output
	PORTD = 0xFF; // Turn some leds on
	
    while(1)
    {
		PORTD ^= 0xFF;
		wait(300);
    }
	
	return 0;
}