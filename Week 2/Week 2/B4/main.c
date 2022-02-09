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

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}

int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code 
    }
}