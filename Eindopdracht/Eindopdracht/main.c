/*
 * Eindopdracht.c
 *
 * Created: 29-3-2022 12:39:46
 * Author : twanh
 */ 

#define F_CPU 10e6
#define SSS_SLAVE_PORT 0

#include <stdlib.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "motor_fader.h"
#include "adc.h"
#include "utils.h"
#include "spi.h"
#include "serial_seven_segment.h"
#include "dmx.h"

int main(void)
{
	DDRA = 0x00;
	
	DDRE = 0xFF;
	DDRF = 0x00;
	
	adc_init_pull();
	adc_enable_irs();
	
	dmx_init();
	
	mfader_init_pwm();
	
	//Initialize fader 0 on adc-channel zero, positive pin PORTE.2, negative pin PORTE.3
	mfader_handle_t mainFader = mfader_init(0, 0, 2, 3, COMPB);
	
	//Initialize fader 1 on adc-channel zero, positive pin PORTE.2, negative pin PORTE.3
	mfader_handle_t secondFader = mfader_init(1, 1, 6, 7, COMPC);
	
	adc_start_conversion();
	
    while (1) 
    {
		dmx_set(2, mfader_get_position(mainFader));
		dmx_set(3, mfader_get_position(secondFader));
		
		dmx_start_send();
		
		wait(50);
    }
	
	mfader_destroy(&mainFader);
	mfader_destroy(&secondFader);
}

