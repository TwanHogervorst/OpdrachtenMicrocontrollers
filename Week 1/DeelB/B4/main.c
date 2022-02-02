/*
 * main.c
 *
 * Created: 2/2/2022 1:57:53 PM
 *  Author: twanh
 */ 

#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8e6

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{
	
	// Set PortD as output
	DDRD = 0xFF;
	
	
    while(1)
    {
		
    }
}