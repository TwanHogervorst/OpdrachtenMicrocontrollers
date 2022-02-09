/*
 * main.c
 *
 * Created: 2/9/2022 10:29:25 AM
 *  Author: twanh
 */ 

#define F_CPU 8e6

#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int ledCount = 0;

ISR( INT1_vect ) {
	// Move light forward
	ledCount = (ledCount + 1) % 8;
	PORTC = 1 << ledCount;
}

ISR( INT2_vect ) {
	// Move light backward
	ledCount = ledCount - 1;
	
	if(ledCount == -1) 
		ledCount = 7;
		
	PORTC = 1 << ledCount;
}

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{
	
	DDRC = 0xFF; // Set PORTC to output
	DDRD = 0x00; // Set PORTD to input
	
	// INT1 and INT2 falling edge
	EICRA |= 0x28;
	
	// Enable INT1 and INT2
	EIMSK |= 0x06;
	
	// Enable interrupt system
	sei();
	
	// Set first light on
	PORTC = 0x01;
	
    while(1)
	{
		// Do nothing
	}
}