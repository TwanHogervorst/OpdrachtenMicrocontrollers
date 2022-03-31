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

#include "motor_fader.h"
#include "adc.h"
#include "utils.h"
#include "spi.h"
#include "serial_seven_segment.h"

int main(void)
{
	DDRA = 0x00;
	
	DDRE = 0xFF;
	DDRF = 0x00;
	
	spi_master_init();
	
	sss_display_driver_init();
	sss_write(0);
	
	adc_init_pull();
	adc_enable_irs();
	
	//Initialize fader 0 on adc-channel zero, positive pin PORTE.3, negative pin PORTE.4
	mfader_handle_t mainFader = mfader_init(0, 0, 3, 4);
	adc_start_conversion();
	
	mfader_init_pwm();
	
	char currPos = 0;
	char savePosition = 0;
	while(1) {
		currPos = mfader_get_position(mainFader);
		
		sss_write(mfader_get_raw_position(mainFader));
		
		if(PINA & BIT(6)) {
			savePosition = currPos;
		}
		if(PINA & BIT(7)) {
			mfader_set_position(mainFader, savePosition);
		}
		
		wait(50);
	}
}

