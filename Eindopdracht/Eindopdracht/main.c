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

int mainChannel = 1;
int secondChannel = 2;

ISR( INT0_vect ) {
	mainChannel = loop(mainChannel + 1, 1, 512);
	
	if(mainChannel == secondChannel)
		mainChannel = loop(mainChannel + 1, 1, 512);
}

ISR( INT1_vect ) {
	mainChannel = loop(mainChannel - 1, 1, 512);
	
	if(mainChannel == secondChannel)
		mainChannel = loop(mainChannel - 1, 1, 512);
}

ISR( INT2_vect ) {
	secondChannel = loop(secondChannel + 1, 1, 512);
	
	if(secondChannel == mainChannel)
		secondChannel = loop(secondChannel + 1, 1, 512);
}

ISR( INT3_vect ) {
	secondChannel = loop(secondChannel - 1, 1, 512);
	
	if(secondChannel == mainChannel)
		secondChannel = loop(secondChannel - 1, 1, 512);
}

int main(void)
{
	DDRA = 0x00;
	
	DDRD = 0xF0;
	
	DDRE = 0xFF;
	DDRF = 0x00;
	
	EICRA = 0b10101010;
	EIMSK = 0x0F;
	
	spi_master_init();
	sss_display_driver_init();
	
	sss_write(0);
	
	adc_init_pull();
	adc_enable_irs();
	
	dmx_init();
	
	mfader_init_pwm();
	
	//Initialize fader 0 on adc-channel zero, positive pin PORTE.2, negative pin PORTE.3
	mfader_handle_t mainFader = mfader_init(0, 0, 2, 3, COMPB);
	
	//Initialize fader 1 on adc-channel zero, positive pin PORTE.2, negative pin PORTE.3
	mfader_handle_t secondFader = mfader_init(1, 1, 6, 7, COMPC);
	
	adc_start_conversion();
	
	int prevMainChannel = -1;
	int prevSecondChannel = -1;
    while (1) 
    {
		if(prevMainChannel != mainChannel) {
			mfader_set_position(mainFader, dmx_get(mainChannel));
			prevMainChannel = mainChannel;
		}
		
		if(prevSecondChannel != secondChannel) {
			mfader_set_position(secondFader, dmx_get(secondChannel));
			prevSecondChannel = secondChannel;
		}
		
		sss_write_upper(mainChannel);
		sss_write(secondChannel);
		
		if(!mfader_is_moving(mainFader))
			dmx_set(mainChannel, mfader_get_position(mainFader));
		
		if(!mfader_is_moving(secondFader))
			dmx_set(secondChannel, mfader_get_position(secondFader));
		
		dmx_start_send();
		
		wait(50);
    }
	
	mfader_destroy(&mainFader);
	mfader_destroy(&secondFader);
}

