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

double bound(double value, double min, double max) {
	if(value > max)
		value = max;
	else if(value < min)
		value = min;
		
	return value;
}

double map(double value, double start1, double stop1, double start2, double stop2) {
	 return (value - start1) / (stop1 - start1) * (stop2 - start2) + start2;
}