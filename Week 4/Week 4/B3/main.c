/*
 * Week 4.c
 *
 * Created: 16-3-2022 11:58:02
 * Author : Luuk Verhagen
 */ 

#include <avr/io.h>
#define F_CPU 8e6

#include <util/delay.h>

#define ADC_START_CONVERSION 0b01000000

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}

void adcInit()
{
	
	//REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0
	ADMUX = 0b01100011; //result left adjusted, channel 3 selected, AVCC
	
	//ADEN ADSC ADFR ADIF ADIE ADPS2 ADPS1 ADPS0
	ADCSRA = 0b10000110;
	
}

// 16 bit
//short adcPull(void) {
	//short result = 0;
	//
	//ADCSRA |= ADC_START_CONVERSION;
	//
	//while(ADCSRA & 0b01000000) {
		//wait(1);
	//}
	//
	//result = ((ADCL & 0b11000000) >> 6) | (ADCH << 2);
	//
	//return result;
//}

char adcPull(void) {
	ADCSRA |= ADC_START_CONVERSION;
	
	// Wait for conversion to finish => ADCSRA == 0bx0xxxxxx
	//     => ADCSRA & ADC_START_CONVERSION
	while(ADCSRA & ADC_START_CONVERSION) {
		wait(1);
	}
	
	return ADCH;
}


int main(void)
{
	DDRF = 0x00;
	DDRB = 0xFF;
	
	adcInit();
	
    while (1) 
    {
		PORTB = adcPull();
		
		wait(100);
    }
}

