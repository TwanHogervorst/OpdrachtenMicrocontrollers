/*
 * adc.c
 *
 * Created: 29-3-2022 13:02:33
 *  Author: twanh
 */ 

#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "utils.h"

#include "adc.h"

#define ADC_START_CONVERSION BIT(6)

void adc_convert_wait();

void adc_init_pull() {
	// Enable ADC, Clock Prescaler = / 64
	ADCSRA = 0b10000110;
	
	// Use vcc reference, left adjust 
	ADMUX = 0b01100000;
}

void adc_init_free() {
	// Enable ADC, Start conversion, Freerun mode, Clock Prescaler = / 64
	ADCSRA = 0b11100110;
	
	// Use vcc reference, left adjust
	ADMUX = 0b01100000;
}

void adc_enable_irs() {
	ADCSRA |= BIT(3);
	sei();
}

void adc_disable_irs() {
	ADCSRA &= ~BIT(3);
}

void adc_channel_select(unsigned char channelPin) {
	// reset current selection
	ADMUX &= 0b11111000;
	
	// select new channel
	ADMUX |= channelPin & 0x07;
}

char adc_readc() {
	return ADCH;
}

short adc_reads() {
	return ((ADCL & 0b11000000) >> 6) | (ADCH << 2);
}

void adc_start_conversion() {
	ADCSRA |= ADC_START_CONVERSION;
}

bool adc_is_converting() {
	return ADCSRA & ADC_START_CONVERSION;
}

char adc_pullc() {
	adc_start_conversion();
	
	adc_convert_wait();
	
	return adc_readc();
}

short adc_pulls() {
	short result = 0;
	
	adc_start_conversion();
	
	adc_convert_wait();
	
	result = ((ADCL & 0b11000000) >> 6) | (ADCH << 2);
	
	return result;
}

void adc_convert_wait() {
	
	// Wait for conversion to finish => ADCSRA == 0bx0xxxxxx
	//     => ADCSRA & ADC_START_CONVERSION
	while (adc_is_converting()) ;
	
}