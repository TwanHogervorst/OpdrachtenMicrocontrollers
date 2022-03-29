/*
 * adc.c
 *
 * Created: 29-3-2022 13:02:33
 *  Author: twanh
 */ 

#include <avr/io.h>

#include "utils.h"

#include "adc.h"

#define ADC_START_CONVERSION BIT(6)

void adc_convert_wait();

char adc_readc() {
	return ADCH;
}

short adc_reads() {
	return ((ADCL & 0b11000000) >> 6) | (ADCH << 2);
}

char adc_pullc() {
	ADCSRA |= ADC_START_CONVERSION;
	
	adc_convert_wait();
	
	return adc_readc();
}

short adc_pulls() {
	short result = 0;
	
	ADCSRA |= ADC_START_CONVERSION;
	
	adc_convert_wait();
	
	result = ((ADCL & 0b11000000) >> 6) | (ADCH << 2);
	
	return result;
}

void adc_convert_wait() {
	
	// Wait for conversion to finish => ADCSRA == 0bx0xxxxxx
	//     => ADCSRA & ADC_START_CONVERSION
	while (ADCSRA & ADC_START_CONVERSION) ;
	
}