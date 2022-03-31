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

#define FADER_PORT PORTE

#define FADER_COUNT 2
#define FADER_MAX_ERROR 4

#define FADER_DEBOUNCE_DEADSPOT FADER_MAX_ERROR * 2

#define FADER_BREAK_DISTANCE 40

typedef enum { DOWN, UP } mfader_direction_t;

typedef struct mfader_conf mfader_conf_t;
struct mfader_conf {
	int faderIndex;
	char adcChannel;
	char positivePin;
	char negativePin;
	mfader_comp_reg compRegister;
	short currentPos;
	short previousPos;
	bool updatePos;
	mfader_direction_t updateDirection;
	short targetPos;
};

static mfader_handle_t faderList[] = {
	NULL,
	NULL
};
static int faderListIsrIndex = 0;

static void mfader_set_speed(mfader_handle_t fader, char speed);

static void mfader_stop_set_pos(mfader_handle_t fader);

static void mfader_toggle_pins(char onPin, char offPin);


#define mfader_move_back(fader) mfader_toggle_pins((fader)->negativePin, (fader)->positivePin)
#define mfader_move_forward(fader) mfader_toggle_pins((fader)->positivePin, (fader)->negativePin)


ISR(ADC_vect) {
	
	// Load new position
	if(faderList[faderListIsrIndex] != NULL) {
		mfader_handle_t fader = faderList[faderListIsrIndex];
		fader->previousPos = fader->currentPos;
		fader->currentPos = adc_reads();
		
		if(fader->updatePos) {
			if(
				/* UP */ (fader->updateDirection && fader->currentPos >= fader->targetPos && fader->currentPos <= (fader->targetPos + FADER_MAX_ERROR))
				|| /* DOWN */ (!fader->updateDirection && fader->currentPos <= fader->targetPos && fader->currentPos >= (fader->targetPos - FADER_MAX_ERROR))
			) {
					
				mfader_stop_set_pos(fader);
				
				// Reset debouncer
				fader->previousPos = 2048;
			}
			else if(fader->currentPos > fader->targetPos) {
				mfader_move_back(fader);
			}
			else if(fader->currentPos < fader->targetPos) {
				mfader_move_forward(fader);
			}
			
			if(abs(fader->currentPos - fader->targetPos) <= FADER_BREAK_DISTANCE) {
				mfader_set_speed(fader, 100);
			}
			else {
				mfader_set_speed(fader, 190);
			}
		}
		
		// Debounce
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
	TCCR3A = 0b00101001;
	
	// Prescaler clk/256, fast PWM
	TCCR3B = 0b00001100;
}

mfader_handle_t mfader_init(int faderIndex, char adcChannel, int positivePin, int negativePin, mfader_comp_reg compRegister) {
	mfader_handle_t result = NULL;
	
	result = (mfader_handle_t) malloc(sizeof(mfader_conf_t));
	if(!result) {
		return NULL;
	}
	
	result->faderIndex = faderIndex;
	result->adcChannel = adcChannel;
	result->positivePin = positivePin;
	result->negativePin = negativePin;
	result->compRegister = compRegister;
	
	// Enable compare register
	//switch(result->compRegister) {
		//case COMPA:
			//TCCR3A |= BIT(7);
			//break;
		//case COMPB:
			//TCCR3A |= BIT(5);
			//break;
		//case COMPC:
			//TCCR3A |= BIT(3);
			//break;
	//}
	
	adc_disable_irs();
	
	adc_channel_select(result->adcChannel);
	result->currentPos = adc_pulls();
	
	adc_enable_irs();
	
	result->previousPos = result->currentPos;
	
	mfader_set_position(result, 0);
	
	faderList[faderIndex] = result;
	return result;
}

void mfader_destroy(mfader_handle_t* fader) {
	if(fader && *fader) {
		faderList[(*fader)->faderIndex] = NULL;
		
		free(*fader);
		*fader = NULL;
	}
}

short mfader_get_raw_position(mfader_handle_t fader) {
	if(fader == NULL)
		return 0;
		
	return fader->currentPos;
}

char mfader_get_position(mfader_handle_t fader) {
	if(fader == NULL) 
		return 0;
		
	char result = 0;
	
	// The fader has a ramp-up at the end
	// To make the result lineair, we need to devide in different sections
	//   to scale differently for each section
	
	if(fader->currentPos < 30) {
		// Do nothing, return 0
	}
	else if(fader->currentPos <= 657) {
		result = (char) map(fader->currentPos, 30, 657, 0, 192);
	}
	else if(fader->currentPos <= 970) {
		result = (char) map(fader->currentPos, 658, 970, 193, 255);
	}
	else {
		result = 255;
	}
	
	return result;
}

void mfader_set_position(mfader_handle_t fader, char pos) {
	if(fader == NULL) {
		return;
	}
	
	fader->updatePos = true;
	
	fader->targetPos = 0;
	
	if(pos <= 192) {
		fader->targetPos  = (short) map(pos, 0, 192, 30, 657);
	}
	else {
		fader->targetPos = (short) map(pos, 193, 255, 658, 970);
	}
	
	fader->updateDirection = fader->targetPos > fader->currentPos;
}

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

static void mfader_stop_set_pos(mfader_handle_t fader) {
	FADER_PORT &= ~(BIT(fader->positivePin) | BIT(fader->negativePin));
	fader->updatePos = false;
}

static void mfader_toggle_pins(char onPin, char offPin) {
	char portValue = FADER_PORT;
	
	if(portValue & BIT(offPin)) {
		portValue &= ~BIT(offPin);
	}
	
	portValue |= BIT(onPin);
	
	FADER_PORT = portValue;
}