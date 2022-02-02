/*
 * main.c
 *
 * Created: 2/2/2022 2:31:28 PM
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
	DDRC = 0x00;
	DDRD = 0xFF;
	PORTD = 0x00;
	
	int sleep = 0x01;
	int buttonPressed = 0;
	int switched = 0;
	
    while(1)
    {
		PORTD ^= (0x01 << 7);
		
		if((!PINC & 0x01) && !buttonPressed)
		{
			buttonPressed = 1;
		}
		else if(!(!PINC & 0x01) && buttonPressed)
		{
			buttonPressed = 0;
			switched = 0;
		}
		
		if(buttonPressed && sleep == 1 && !switched)
		{
			sleep = 4;
			switched = 1;
		}
		else if(buttonPressed && sleep == 4 && !switched)
		{
			sleep = 1;
			switched = 1;
		}
		
        wait(1000/sleep);
    }
}