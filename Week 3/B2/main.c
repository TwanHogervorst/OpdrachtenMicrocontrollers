/*
 * B2.c
 *
 * Created: 16-2-2022 11:08:30
 * Author : Luuk Verhagen
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BIT(x) (1 << x)

int TimerPreset = -313;
int TimerCompare = -313+117;

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

ISR( TIMER2_OVF_vect ) {
	TCNT2 = TimerPreset;	// Preset value
	PORTD |= BIT(7);
}

ISR( TIMER2_COMP_vect ){
	PORTD &= ~BIT(7);
}

// Initialize timer2
void timer2Init( void ) {
	TCNT2 = TimerPreset;	// Preset value of counter 2
	OCR2 = TimerCompare;
	TIMSK |= BIT(6);		// T2 overflow interrupt enable
	TIMSK |= BIT(7);		// T2 overflow interrupt enable
	sei();				// turn_on intr all
	TCCR2 = 0b00010101;		
}



int main(void)
{
	DDRD &= BIT(7);
	
	timer2Init();
	
	PORTD |= BIT(7);
	
    /* Replace with your application code */
    while (1) 
    {
		wait(10);
    }
}

