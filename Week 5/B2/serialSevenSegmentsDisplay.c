/*
 * serialSevenSegmentsDisplay.c
 *
 * Created: 23-3-2022 14:43:50
 *  Author: twanh
 */ 

#include "spi.h"

#include "serialSevenSegmentsDisplay.h"

void displayDriverInit()
{
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