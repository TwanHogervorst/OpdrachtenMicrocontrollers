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

//On button D0 press increase the channel of the first fader 
ISR( INT0_vect ) {
	mainChannel = loop(mainChannel + 1, 1, 512);
	
	if(mainChannel == secondChannel)
		mainChannel = loop(mainChannel + 1, 1, 512);
}

//On button D1 press decrease the channel of the first fader 
ISR( INT1_vect ) {
	mainChannel = loop(mainChannel - 1, 1, 512);
	
	if(mainChannel == secondChannel)
		mainChannel = loop(mainChannel - 1, 1, 512);
}

//On button D2 press increase the channel of the second fader 
ISR( INT2_vect ) {
	secondChannel = loop(secondChannel + 1, 1, 512);
	
	if(secondChannel == mainChannel)
		secondChannel = loop(secondChannel + 1, 1, 512);
}

//On button D3 press decrease the channel of the second fader 
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
	
	EICRA = 0b10101010;		// Set Interrupts INT0...INT3 falling edge
	EIMSK = 0x0F;			// Enable Interrupts INT0...INT3
	
	spi_master_init();
	sss_display_driver_init();
	
	// Show 0 on 7 segment display
	sss_write(0);
	sss_write_upper(0);
	
	// Init adc as async pull mode
	adc_init_pull();
	adc_enable_irs();
	
	dmx_init();
	
	// Initialize the Timer for pwm signal faders
	mfader_init_pwm();
	
	//Initialize fader 0 on adc-channel zero, positive pin PORTE.2, negative pin PORTE.3
	mfader_handle_t mainFader = mfader_init(0, 0, 2, 3, COMPB);
	
	//Initialize fader 1 on adc-channel zero, positive pin PORTE.2, negative pin PORTE.3
	mfader_handle_t secondFader = mfader_init(1, 1, 6, 7, COMPC);
	
	// Start an asynchronous adc conversion
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
		
		// Set the value of the dmx channel if preset button isn't pressed and fader isn't moving to target
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
		
		// Start async sending all dmx values to devices
		dmx_start_send();
		
		wait(50);
    }
	
	// Free resources of faders
	// Should never be run
	mfader_destroy(&mainFader);
	mfader_destroy(&secondFader);
}

