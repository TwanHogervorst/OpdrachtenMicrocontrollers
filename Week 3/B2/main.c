/*
 * B2.c
 *
 * Created: 16-2-2022 11:08:30
 * Author : Luuk Verhagen
 */ 

#define F_CPU 10e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BIT(x) (1 << x)

int isOn = 1;

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

ISR( TIMER2_COMP_vect ){
	OCR2 = isOn ? 244 : 146; // 25ms : 15ms
	PORTD = (!isOn) << 7;
	isOn = !isOn;
}

// Initialize timer2
void timer2Init( void ) {
	OCR2 = 146;
	TIMSK |= BIT(7);		// T2 compare interrupt enable
	sei();				// turn_on intr all
	TCCR2 = 0b00001101;		
}

int main(void)
{
	DDRD |= BIT(7);
	timer2Init();
	PORTD |= BIT(7);
    while (1) 
    {
		wait(10);
    }
}

