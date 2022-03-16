/*
 * Week 4.c
 *
 * Created: 16-3-2022 11:58:02
 * Author : Luuk Verhagen
 */ 

#include <avr/io.h>
#define F_CPU 8e6

#include <util/delay.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}

void adcInit( void )
{
	//REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0
	ADMUX = 0b01100001; //result left adjusted, channel 1 selected, AVCC
	
	//ADEN ADSC ADFR ADIF ADIE ADPS2 ADPS1 ADPS0
	ADCSRA = 0b11100110;
}


int main(void)
{
	DDRF = 0x00;
	DDRA = 0xFF;
	DDRB = 0xFF;
	adcInit();
	
    while (1) 
    {
		PORTB = ADCL;
		PORTA = ADCH;
		wait(100);
    }
}

