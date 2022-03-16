/*
 * Week 4.c
 *
 * Created: 16-3-2022 11:58:02
 * Author : Luuk Verhagen
 */ 

#include <avr/io.h>
#define F_CPU 8e6

#include <util/delay.h>

#include <stdbool.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}

void adcInit(bool freeRunMode)
{
	//REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0
	ADMUX = 0b01100001; //result left adjusted, channel 1 selected, AVCC
	
	//ADEN ADSC ADFR ADIF ADIE ADPS2 ADPS1 ADPS0
	if(freeRunMode) {
		ADCSRA = 0b11100110;
	}
	else {
		ADCSRA = 0b10000110;
	}
	
}

short adcPull(void) {
	short result = 0;
	
	ADCSRA |= 0b01000000;
	
	while(ADCSRA & 0b01000000) {
		wait(1);
	}
	
	result = ((ADCL & 0b11000000) >> 6) | (ADCH << 2);
	
	return result;
}


int main(void)
{
	DDRF = 0x00;
	DDRA = 0xFF;
	DDRB = 0xFF;
	adcInit(false);
	
    while (1) 
    {
		// B2
		//PORTB = ADCL;
		//PORTA = ADCH;
		
		// B3
		PORTB = adcPull() & 0xFF;
		
		wait(100);
    }
}

