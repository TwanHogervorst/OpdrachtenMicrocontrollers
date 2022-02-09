#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int button1Pressed = 0;
int button2Pressed = 0;
int counter = 0;

const unsigned char Numbers [16] = {
	// dPgfe dcba
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111, // 9
	0b01110111, // A
	0b01111100, // B
	0b00111001, // C
	0b01011110, // D
	0b01111001, // E
	0b01110001,  // D
};

/******************************************************************
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
Version :    	DMK, Initial code
*******************************************************************/
void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

void display(int digit)
{
	if(digit > -1 && digit < 16)
	{
		PORTB = Numbers[digit];	
	}
	else
	{
		PORTB = Numbers[14];
	}
}


ISR( INT4_vect ) {
	button1Pressed = (PINE & (1 << 4)) >> 4;
	
	if(button1Pressed && button2Pressed)
	{
		counter = 0;
		display(0);
	}
	else
	{
		counter--;
		display(counter);
	}
}

ISR( INT5_vect ) {
	button2Pressed = (PINE & (1 << 5)) >> 5;
	
	if(button1Pressed && button2Pressed)
	{
		counter = 0;
		display(0);
	}
	else
	{
		counter++;
		display(counter);
	}
}

/******************************************************************
short:			main() loop, entry point of executable
inputs:
outputs:
notes:
Version :    	DMK, Initial code
*******************************************************************/
int main( void ) {
	DDRB = 0b11111111;					// PORTB all output 
	DDRD = 0;
	
	EICRB |= 0x05;
	EIMSK |= 0X30;
	
	sei();
	
	display(0);
	
	while (1==1) {
		_delay_ms( 1 );
	}

	return 1;
}