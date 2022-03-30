/*
 * adc.h
 *
 * Created: 29-3-2022 13:02:42
 *  Author: twanh
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdbool.h>

void adc_init_pull();
void adc_init_free();

void adc_enable_irs();
void adc_disable_irs();

void adc_channel_select(unsigned char channelPin);

char adc_readc();
short adc_reads();

void adc_start_conversion();
bool adc_is_converting();

char adc_pullc();
short adc_pulls();



#endif /* ADC_H_ */