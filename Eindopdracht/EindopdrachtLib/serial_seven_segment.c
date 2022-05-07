/*
 * serial_seven_segment.c
 *
 * Created: 29-3-2022 15:00:33
 *  Author: twanh
 */ 

#include <stdbool.h>
#include <stdlib.h>

#include "spi.h"
#include "utils.h"

#include "serial_seven_segment.h"

void sss_display_driver_init() {
	
	// Reset display spi buffer
	spi_write_word(SSS_SLAVE_PORT, 0x00, 0x00);
	
	// Register Decode Mode -> 0xFF (BCD mode for all digits)
	spi_write_word(SSS_SLAVE_PORT, 0x09, 0xFF);

	// Register Intensity -> Level 31/32 (max brightness)
	spi_write_word(SSS_SLAVE_PORT, 0x0A, 0x0F);

	// Register Scan-Limit -> 0x03 (Display digits 0..3)
	spi_write_word(SSS_SLAVE_PORT, 0x0B, 0x07);
	
	// Shutdown Register -> 1 (Display On/Normal Operation)
	spi_write_word(SSS_SLAVE_PORT, 0x0C, 0x01);
	
	// Show 0 on all digits
	sss_write(0);
	sss_write_upper(0);
}

void sss_display_on() {
	spi_write_word(SSS_SLAVE_PORT, 0x0C, 0x01); // Shutdown Register -> 1 (Display On/Normal Operation)
}

void sss_display_off() {
	spi_write_word(SSS_SLAVE_PORT, 0x0C, 0x00); // Shutdown Register -> 0 (Display Off)
}

void sss_write_offset(int value, int offset) {
	
	char isNegative = value < 0;
	offset = bound(offset, 0, 4);
	
	value = abs(value);
	
	char thousends = 0;
	if(!isNegative) {
		thousends = value / 1000;
		value = value - (thousends * 1000);
	}
	
	char hundreds = value / 100;
	
	value = value - (hundreds * 100);
	char tens = value / 10;
	
	char ones = value - (tens * 10);
	
	spi_write_word(SSS_SLAVE_PORT, offset + 1, ones);
	spi_write_word(SSS_SLAVE_PORT, offset + 2, tens);
	spi_write_word(SSS_SLAVE_PORT, offset + 3, hundreds);
	
	if (isNegative) 
	{
		spi_write_word(SSS_SLAVE_PORT, offset + 4, 0x0A /* '-' */);
	}
	else {
		spi_write_word(SSS_SLAVE_PORT, offset + 4, thousends);
	}
}
