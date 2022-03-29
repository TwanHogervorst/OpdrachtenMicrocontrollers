/*
 * B2.c
 *
 * Created: 23-3-2022 14:40:47
 * Author : twanh
 */ 

#include <avr/io.h>

#include "common.h"
#include "spi.h"
#include "serialSevenSegmentsDisplay.h"

int main(void)
{
    // inilialize
    DDRB=0x01;					  	// Set PB0 pin as output for display select
    spi_masterInit();              	// Initialize spi module
    displayDriverInit();            // Initialize display chip

    // clear display (all zero's)
    for (char i = 1; i <= 4; i++)
    {
	    spi_writeWord(i, 0); // Set digit to 0
    }
    wait(1000);

    //// write 4-digit data
    //for (char i = 1; i <= 4; i++)
    //{
	    //spi_writeWord(i, i); // Write number to show to digit
	    //
	    //wait(1000);
    //}
    //wait(1000);
	
	//writeLedDisplay(6942);
	writeLedDisplay(-420);

    return (1);
}

