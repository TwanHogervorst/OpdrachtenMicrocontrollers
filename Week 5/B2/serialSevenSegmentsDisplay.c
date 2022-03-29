/*
 * serialSevenSegmentsDisplay.c
 *
 * Created: 23-3-2022 14:43:50
 *  Author: twanh
 */ 

#include <stdbool.h>
#include <math.h>

#include "common.h"
#include "spi.h"

#include "serialSevenSegmentsDisplay.h"

void displayDriverInit()
{
	spi_writeWord(0x00, 0x00);
	
	// Register Decode Mode -> 0xFF (BCD mode for all digits)
	spi_writeWord(0x09, 0xFF);

	// Register Intensity -> Level 31/32 (max brightness)
	spi_writeWord(0x0A, 0x0F);

	// Register Scan-Limit -> 0x03 (Display digits 0..3)
	spi_writeWord(0x0B, 0x03);
	
	// Shutdown Register -> 1 (Display On/Normal Operation)
	spi_writeWord(0x0C, 0x01);
}

// Set display on ('normal operation')
void displayOn()
{
	spi_writeWord(0x0C, 0x01); // Shutdown Register -> 1 (Display On/Normal Operation)
}

// Set display off ('shut down')
void displayOff()
{
	spi_writeWord(0x0C, 0x00); // Shutdown Register -> 0 (Display Off)
}

// toont de waarde van value op het 4-digit display
void writeLedDisplay(int value) {
	bool isNegative = value < 0;
	
	value = abs(value);
	
	char thousends = 0;
	if(!isNegative) {
		char thousends = value / 1000;
		value = value - (thousends * 1000);
	}
	
	char hundreds = value / 100;
	
	value = value - (hundreds * 100);
	char tens = value / 10;
	
	char ones = value - (tens * 10);
	
	spi_writeWord(1, ones);
	spi_writeWord(2, tens);
	spi_writeWord(3, hundreds);
	
	if(isNegative)
		spi_writeWord(4, 0x0A /* '-' */);
	else 
		spi_writeWord(4, thousends);
}