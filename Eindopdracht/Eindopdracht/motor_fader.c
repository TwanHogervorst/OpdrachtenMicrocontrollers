/*
 * motor_fader.c
 *
 * Created: 30-3-2022 13:34:21
 *  Author: twanh
 */ 

#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc.h"
#include "utils.h"

#include "motor_fader.h"

// Port where the fader H-Bridge is connected
#define FADER_PORT PORTE

#define FADER_COUNT 2

// The maximum error of the fader position from the set value
#define FADER_MAX_ERROR 4

// If fader position is in deadspot, fader will be seen as on position and movement will stop
#define FADER_DEBOUNCE_DEADSPOT FADER_MAX_ERROR * 2

// Start slowing down the fader when fader difference to target is smaller than break distance
#define FADER_BREAK_DISTANCE 40

// Every value beneath ZERO_THRESHOLD will be treated as zero value
#define FADER_POSITION_ZERO_THRESHOLD 60

// Valid movement directions of fader
typedef enum { DOWN, UP } mfader_direction_t;

// Struct containing all information of a fader
typedef struct mfader_conf mfader_conf_t;
struct mfader_conf {
	int faderIndex; // Index in fader list, fader id
	char adcChannel; // ADC Channel where the fader value can be read
	char positivePin; // The positive pin for the H-Bridge (motor up direction)
	char negativePin; // The negative pin for the H-Bridge (motor down direction)
	mfader_comp_reg compRegister; // Timer compare register for PWM
	short currentPos; // The current analog position of the fader
	short previousPos; // The previous analog position of the fader
	bool updatePos; // When true, fader is moving to target
	mfader_direction_t updateDirection; // The direction the fader is moving in
	short targetPos; // The target position to move to
};

// Private vars
static mfader_handle_t faderList[] = {
	NULL,
	NULL
};
static int faderListIsrIndex = 0;

// Private functions
static void mfader_set_speed(mfader_handle_t fader, char speed);

static void mfader_stop_set_pos(mfader_handle_t fader);

static void mfader_toggle_pins(char onPin, char offPin);

// Private defines
#define mfader_move_back(fader) mfader_toggle_pins((fader)->negativePin, (fader)->positivePin)
#define mfader_move_forward(fader) mfader_toggle_pins((fader)->positivePin, (fader)->negativePin)

// Interrupt routine for ADC conversion finish
ISR(ADC_vect) {
	
	if(faderList[faderListIsrIndex] != NULL) {
		// Read new position of current selected fader from ADC
		mfader_handle_t fader = faderList[faderListIsrIndex];
		fader->previousPos = fader->currentPos;
		fader->currentPos = adc_reads();
		
		// Check if fader is moving
		if(fader->updatePos) {
			if(
				/* UP */ (fader->updateDirection && fader->currentPos >= fader->targetPos && fader->currentPos <= (fader->targetPos + FADER_MAX_ERROR))
				|| /* DOWN */ (!fader->updateDirection && fader->currentPos <= fader->targetPos && fader->currentPos >= (fader->targetPos - FADER_MAX_ERROR))
			) {
				// Fader has reached target
				mfader_stop_set_pos(fader);
				
				// Reset debouncer by setting a value out of range, larger than deadspot
				fader->previousPos = 2048;
			}
			else if(fader->currentPos > fader->targetPos) {
				// Fader needs to move back to hit target
				mfader_move_back(fader);
			}
			else if(fader->currentPos < fader->targetPos) {
				// Fader needs to move forward to hit target
				mfader_move_forward(fader);
			}
			
			// Check if fader needs breaking
			if(abs(fader->currentPos - fader->targetPos) <= FADER_BREAK_DISTANCE) {
				mfader_set_speed(fader, 100); // Slow down fader
			}
			else {
				mfader_set_speed(fader, 190); // Move with normal speed
			}
		}
		
		// Debounce (because adc/fader isn't that precise)
		if(abs(fader->currentPos - fader->previousPos) <= (FADER_DEBOUNCE_DEADSPOT * (fader->currentPos / 500 + 1))) {
			fader->currentPos = fader->previousPos;
		}
	}
	
	// Update current fader
	faderListIsrIndex = faderListIsrIndex ? 0 : 1;
	
	// Select next fader adc
	if(faderList[faderListIsrIndex] != NULL)
		adc_channel_select(faderList[faderListIsrIndex]->adcChannel);
	
	// Start a new conversion
	adc_start_conversion();
	
}

void mfader_init_pwm() {
	// PWM-8bit
	TCCR3A = 0b00000001;
	
	// Prescaler clk/256, fast PWM
	TCCR3B = 0b00001100;
}

mfader_handle_t mfader_init(int faderIndex, char adcChannel, int positivePin, int negativePin, mfader_comp_reg compRegister) {
	mfader_handle_t result = NULL;
	
	// Allocate space for fader
	result = (mfader_handle_t) malloc(sizeof(mfader_conf_t));
	if(!result) {
		return NULL;
	}
	
	// Initialize fader values
	result->faderIndex = faderIndex;
	result->adcChannel = adcChannel;
	result->positivePin = positivePin;
	result->negativePin = negativePin;
	result->compRegister = compRegister;
	
	// Enable compare register
	switch(result->compRegister) {
		case COMPA:
			TCCR3A |= BIT(7);
			break;
		case COMPB:
			TCCR3A |= BIT(5);
			break;
		case COMPC:
			TCCR3A |= BIT(3);
			break;
	}
	
	// Read first position synchronously
	adc_disable_irs();
	
	adc_channel_select(result->adcChannel);
	result->currentPos = adc_pulls();
	
	adc_enable_irs();
	
	result->previousPos = result->currentPos;
	
	// Reset fader position to 0
	mfader_set_position(result, 0);
	
	faderList[faderIndex] = result;
	return result;
}

// Free all resources from fader
void mfader_destroy(mfader_handle_t* fader) {
	if(fader && *fader) {
		faderList[(*fader)->faderIndex] = NULL;
		
		mfader_stop_set_pos(*fader);
		
		free(*fader);
		*fader = NULL;
	}
}

// Gets the not mapped position from the fader (0-1024, not linear)
short mfader_get_raw_position(mfader_handle_t fader) {
	if(fader == NULL)
		return 0;
		
	return fader->currentPos;
}

// Gets the mapped position from the fader (0-255, linear)
char mfader_get_position(mfader_handle_t fader) {
	if(fader == NULL) 
		return 0;
		
	char result = 0;
	
	// The fader has a ramp-up at the end
	// To make the result linear, we need to divide in different sections
	//   to scale differently for each section
	
	if(fader->currentPos < FADER_POSITION_ZERO_THRESHOLD) {
		// Do nothing, return 0
	}
	else if(fader->currentPos <= 657) {
		result = (char) map(fader->currentPos, FADER_POSITION_ZERO_THRESHOLD, 657, 0, 192);
	}
	else if(fader->currentPos <= 970) {
		result = (char) map(fader->currentPos, 658, 970, 193, 255);
	}
	else {
		result = 255;
	}
	
	return result;
}

// Sets the mapped position of the fader (input: 0-255, linear)
void mfader_set_position(mfader_handle_t fader, char pos) {
	if(fader == NULL) {
		return;
	}
	
	fader->updatePos = true;
	
	fader->targetPos = 0;
	
	// The same with getting fader now for setting:
	// Because of the ramp-up, we need to divide in sections again
	
	if(pos <= 192) {
		fader->targetPos  = (short) map(pos, 0, 192, 30, 657);
	}
	else {
		fader->targetPos = (short) map(pos, 193, 255, 658, 970);
	}
	
	fader->updateDirection = fader->targetPos > fader->currentPos;
}

// True if the fader is moving to a target
bool mfader_is_moving(mfader_handle_t fader) {
	if(fader == NULL) 
		return false;
		
	return fader->updatePos;
}

// Sets the speed of the fader, by setting a compare register for PWM
static void mfader_set_speed(mfader_handle_t fader, char speed) {
	switch(fader->compRegister) {
		case COMPA:
			OCR3A = speed;
			break;
		case COMPB:
			OCR3B = speed;
			break;
		case COMPC:
			OCR3C = speed;
			break;
	}
}

// Stops the movement of the fader
static void mfader_stop_set_pos(mfader_handle_t fader) {
	FADER_PORT &= ~(BIT(fader->positivePin) | BIT(fader->negativePin));
	fader->updatePos = false;
}

// Toggles the specified pins. Used for setting the fader motor direction
static void mfader_toggle_pins(char onPin, char offPin) {
	char portValue = FADER_PORT;
	
	if(portValue & BIT(offPin)) {
		portValue &= ~BIT(offPin);
	}
	
	portValue |= BIT(onPin);
	
	FADER_PORT = portValue;
}