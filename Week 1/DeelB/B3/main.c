/*
 * main.c
 *
 * Created: 2/2/2022 12:30:56 PM
 *  Author: Luuk Verhagen
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
	DDRD = 0xFF;
	DDRC = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	
    while(1)
    {
		PORTD ^= (1 << 7) & ((!PINC & 0x01) << 7);
		wait(500);
		
		//if((PINC & 0x01) == 0x00)
		//{
			//PORTD = (PORTD & (1 << 7)) ^ (1 << 7);
			//wait(50);
		//}
		//else
		//{
			//PORTD = 0x00;
		//}
    }
}