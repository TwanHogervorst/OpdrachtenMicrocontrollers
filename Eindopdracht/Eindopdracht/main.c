/*
 * Eindopdracht.c
 *
 * Created: 29-3-2022 12:39:46
 * Author : twanh
 */ 

#define F_CPU 10e6
#define SSS_SLAVE_PORT 0

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
	
	char positions[] = {
		0,
		64,
		128,
		192,
		255,
		98,
		160
	};
	
	int positionsLength = sizeof(positions) / sizeof(positions[0]);
	int i = 0;
	
	int msCount = 0;
	while(1) {
		mfader_update();
		
		sss_write(mfader_get_position(mainFader));
		
		msCount++;
		if(msCount > 1000) {
			msCount = 0;
			mfader_set_position(mainFader, positions[i]);
			i++;
			
			if(i >= positionsLength) {
				i = 0;
			}
		}
		
		wait(1);
	}
}

