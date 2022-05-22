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
volatile bool presetSaveButtonPressed = false;
volatile bool presetLoadButtonPressed = false;

//On button D1 (TODO?) press increase the channel of the first fader 
ISR( INT0_vect ) {
	mainChannel = loop(mainChannel + 1, 1, 512);
	
	if(mainChannel == secondChannel)
		mainChannel = loop(mainChannel + 1, 1, 512);
}

//On button D2 (TODO?) press decrease the channel of the first fader 
ISR( INT1_vect ) {
	mainChannel = loop(mainChannel - 1, 1, 512);
	
	if(mainChannel == secondChannel)
		mainChannel = loop(mainChannel - 1, 1, 512);
}

//On button D3 (TODO?) press increase the channel of the second fader 
ISR( INT2_vect ) {
	secondChannel = loop(secondChannel + 1, 1, 512);
	
	if(secondChannel == mainChannel)
		secondChannel = loop(secondChannel + 1, 1, 512);
}

//On button D4 (TODO?) press decrease the channel of the second fader 
ISR( INT3_vect ) {
	secondChannel = loop(secondChannel - 1, 1, 512);
	
	if(secondChannel == mainChannel)
		secondChannel = loop(secondChannel - 1, 1, 512);
}

int main(void)
{
	DDRD = 0x00;	//Initialize port D to input
	
	DDRE = 0xFF;	//Initialize port E to output
	DDRF = 0x00;	//Initialize port F to input
	
	EICRA = 0b10101010;		//TODO?
	EIMSK = 0x0F;			//TODO?
	
	spi_master_init();
	sss_display_driver_init();
	
	sss_write(0);	//Write to seven segment display
	
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
		//If main fader changed of channel
		if(prevMainChannel != mainChannel) {
			mfader_set_position(mainFader, dmx_get(mainChannel));
			prevMainChannel = mainChannel;
		}
		
		//If second fader changed of channel
		if(prevSecondChannel != secondChannel) {
			mfader_set_position(secondFader, dmx_get(secondChannel));
			prevSecondChannel = secondChannel;
		}
		
		//Write to seven segment display
		sss_write_upper(mainChannel);
		sss_write(secondChannel);
		
		//Load presets if preset button is pressed and fader isn't moving
		if(!mfader_is_moving(mainFader) && !presetLoadButtonPressed)
			dmx_set(mainChannel, mfader_get_position(mainFader));
		
		if(!mfader_is_moving(secondFader) && !presetLoadButtonPressed)
			dmx_set(secondChannel, mfader_get_position(secondFader));
			
		//Save presets if preset save button is pressed	
		if((PIND & (1 << 4)) && !presetSaveButtonPressed)
		{
			dmx_preset_save();
			presetSaveButtonPressed = true;
		}
		else if(presetSaveButtonPressed){
			presetSaveButtonPressed = false;
		}
		
		//Load presets if preset load button is pressed
		if((PIND & (1 << 5)) && !presetLoadButtonPressed)
		{
			dmx_preset_load();
			mfader_set_position(mainFader, dmx_get(mainChannel));
			mfader_set_position(secondFader, dmx_get(secondChannel));
			presetLoadButtonPressed = true;
		}else if(presetLoadButtonPressed){
			presetLoadButtonPressed = false;
		}
		
		dmx_start_send();
		
		wait(50);
    }
	
	mfader_destroy(&mainFader);
	mfader_destroy(&secondFader);
}

