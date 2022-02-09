/*
 * main.c
 *
 * Created: 2/9/2022 11:22:10 AM
 *  Author: twanh
 */ 

#define F_CPU 8e6

#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>

typedef struct {
	unsigned char data;
	unsigned int delay;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[] = {
	//  .gfedcba
	{ 0b00000001, 500}, // Section: Start
	{ 0b00000010, 500},
	{ 0b00000100, 500},
	{ 0b00001000, 500},
	{ 0b00010000, 500},
	{ 0b00100000, 1000},
	{ 0b01000000, 250}, // Section: Blink
	{ 0b00000000, 250},
	{ 0b01000000, 250}, // repeat.
	{ 0b00000000, 250},
	{ 0b01000000, 250}, // rep.
	{ 0b00000000, 250},
	{ 0b01000000, 250}, // rep.
	{ 0b00000000, 250},
	{ 0b00000001, 100}, // Section: Loop
	{ 0b00000010, 100},
	{ 0b00000100, 100},
	{ 0b00001000, 100},
	{ 0b00010000, 100},
	{ 0b00100000, 100},
	{ 0b00000001, 100}, // rep.
	{ 0b00000010, 100},
	{ 0b00000100, 100},
	{ 0b00001000, 100},
	{ 0b00010000, 100},
	{ 0b00100000, 100},
	{ 0b00000001, 100}, // rep.
	{ 0b00000010, 100},
	{ 0b00000100, 100},
	{ 0b00001000, 100},
	{ 0b00010000, 100},
	{ 0b00100000, 100},
	{ 0b00000001, 100}, // Section: Snake
	{ 0b00000010, 100},
	{ 0b01000000, 100},
	{ 0b00010000, 100},
	{ 0b00001000, 100},
	{ 0b00000100, 100},
	{ 0b01000000, 100},
	{ 0b00100000, 100},
	{ 0b00000001, 100}, // rep.
	{ 0b00000010, 100},
	{ 0b01000000, 100},
	{ 0b00010000, 100},
	{ 0b00001000, 100},
	{ 0b00000100, 100},
	{ 0b01000000, 100},
	{ 0b00100000, 100},
	{ 0b00000001, 100}, // rep.
	{ 0b00000010, 100},
	{ 0b01000000, 100},
	{ 0b00010000, 100},
	{ 0b00001000, 100},
	{ 0b00000100, 100},
	{ 0b01000000, 100},
	{ 0b00100000, 100}
};

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}

int main(void)
{
	
	// 7-Segments LED is on PORTB, so set output
	DDRB = 0xFF;
	
	// Get patternLength
	int patternLength = sizeof pattern / sizeof pattern[0];
	
    while(1)
    {
		// Clear current frame
		PORTB = 0x00;
		
		// Loop over pattern
		for(int i = 0; i < patternLength; i++) {
			// Get current pattern frame
			PATTERN_STRUCT pFrame = pattern[i];
			
			// Set 7-Segments LED
			PORTB = pFrame.data;
			
			// Wait frame delay
			wait(pFrame.delay);
		}
    }
}