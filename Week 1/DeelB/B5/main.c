/*
 * main.c
 *
 * Created: 2/2/2022 2:46:22 PM
 *  Author: twanh
 */ 

#define F_CPU 8e6

#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>

typedef struct {
	unsigned char port;
	unsigned char data;
	unsigned int delay;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[] = {
	{ 0x00, 0x01, 100 }, { 0x01, 0x01, 100 }, { 0x02, 0x01, 100 }, { 0x03, 0x01, 100 },
	{ 0x03, 0x03, 500 }, { 0x03, 0x07, 500 }, { 0x03, 0x0F, 500 }, { 0x03, 0x1F, 500 }, { 0x03, 0x3F, 500 }, { 0x03, 0x7F, 500 }, { 0x03, 0xFF, 500 },
	{ 0x02, 0x81, 100 }, { 0x01, 0x81, 100 }, { 0x00, 0x81, 100 },
	{ 0x00, 0xC1, 500 }, { 0x00, 0xE1, 500 }, { 0x00, 0xF1, 500 }, { 0x00, 0xF9, 500 }, { 0x00, 0xFD, 500 }, { 0x00, 0xFF, 500 }, { 0x00, 0xFF, 500 },
	{ 0x00, 0xFF, 2000 }
};

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}

int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	
	int patternLength = sizeof pattern / sizeof pattern[0];
	
    while(1)
    {
		PORTA = 0x00;
		PORTB = 0x00;
		PORTC = 0x00;
		PORTD = 0x00;
		
		for (int i = 0; i < patternLength; i++) {
			PATTERN_STRUCT pFrame = pattern[i];
			
			switch (pFrame.port)
			{
				case 0x00:
					PORTA = pFrame.data;
					break;
				case 0x01:
					PORTB = pFrame.data;
					break;
				case 0x02:
					PORTC = pFrame.data;
					break;
				case 0x03:
					PORTD = pFrame.data;
					break;
				default:
					break;
			}
			
			wait(pFrame.delay);
		}
    }
}