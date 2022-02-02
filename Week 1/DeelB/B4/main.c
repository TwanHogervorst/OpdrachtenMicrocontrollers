/*
 * main.c
 *
 * Created: 2/2/2022 1:57:53 PM
 *  Author: twanh
 */

 
#define F_CPU 8e6

#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}

int main(void)
{
	
	// Set PortD as output
	DDRD = 0xFF;
	PORTD = 0x01;
	
	int ledCount = 0;
	
    while(1)
    {
		/*PORTD = 1 << ledCount;
		ledCount = (ledCount + 1) % 8;*/
		
		if(PORTD == 0x80)
			PORTD = 1;
		else
			PORTD = PORTD << 1;
		
		wait(50);
		
    }
}