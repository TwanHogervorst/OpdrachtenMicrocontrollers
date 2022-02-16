/*
 * utils.c
 *
 * Created: 16-2-2022 10:56:58
 *  Author: twanh
 */ 

#define F_CPU 8e6

#include <util/delay.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}