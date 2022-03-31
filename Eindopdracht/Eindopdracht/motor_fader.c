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
#define FADER_MAX_ERROR 2

typedef enum { POSITIVE, NEGATIVE } mfader_direction_t;

typedef struct mfader_conf mfader_conf_t;
struct mfader_conf {
	int faderIndex;
	char adcChannel;
	char positivePin;
	char negativePin;
	short currentPos;
	bool updatePos;
	short targetPos;
};

static mfader_handle_t faderList[] = {
	NULL,
	NULL
};
static int faderListIsrIndex = 0;


static void mfader_stop_set_pos(mfader_handle_t fader);

static void mfader_toggle_pins(char onPin, char offPin);


#define mfader_move_back(fader) mfader_toggle_pins((fader)->negativePin, (fader)->positivePin)
#define mfader_move_forward(fader) mfader_toggle_pins((fader)->positivePin, (fader)->negativePin)


ISR(ADC_vect) {
	
	// Load new position
	if(faderList[faderListIsrIndex] != NULL) {
		mfader_handle_t fader = faderList[faderListIsrIndex];
		fader->currentPos = adc_reads();
		
		if(fader->updatePos) {
			if(fader->currentPos >= (fader->targetPos - FADER_MAX_ERROR) && fader->currentPos <= (fader->targetPos + FADER_MAX_ERROR)) {
				mfader_stop_set_pos(fader);
			}
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

mfader_handle_t mfader_init(int faderIndex, char adcChannel, int positivePin, int negativePin) {
	mfader_handle_t result = NULL;
	
	result = (mfader_handle_t) malloc(sizeof(mfader_conf_t));
	if(!result) {
		return NULL;
	}
	
	result->faderIndex = faderIndex;
	result->adcChannel = adcChannel;
	result->positivePin = positivePin;
	result->negativePin = negativePin;
	
	adc_disable_irs();
	
	adc_channel_select(result->adcChannel);
	result->currentPos = adc_pulls();
	
	adc_enable_irs();
	
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

char mfader_get_position(mfader_handle_t fader) {
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
}

void mfader_update() {
	
	for (int i = 0; i < FADER_COUNT; i++) {
		mfader_handle_t fader = faderList[i];
		
		if(fader == NULL)
			continue;
		
		// Check if position is being set/updated
		if(fader->updatePos) {
			
			if(fader->currentPos >= (fader->targetPos - FADER_MAX_ERROR) && fader->currentPos <= (fader->targetPos + FADER_MAX_ERROR)) {
				mfader_stop_set_pos(fader);
			}
			else if(fader->currentPos > fader->targetPos) {
				mfader_move_back(fader);
			}
			else if(fader->currentPos < fader->targetPos) {
				mfader_move_forward(fader);
			}
			
		}
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