/*
 * utils.c
 *
 * Created: 29-3-2022 12:46:04
 *  Author: twanh
 */ 

#define F_CPU 10e6
#include <util/delay.h>

#include "utils.h"

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}