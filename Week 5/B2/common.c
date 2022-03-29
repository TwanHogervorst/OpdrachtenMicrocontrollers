/*
 * common.c
 *
 * Created: 23-3-2022 14:44:46
 *  Author: twanh
 */ 

#define F_CPU 8e6
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"

// wait(): busy waiting for 'ms' millisecond
// used library: util/delay.h
void wait(int ms)
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms(1);
	}
}

// Splits the specified value to each tenth
// E.g. value=6942, tens=3 => { 6, 9, 4, 2 }
char* numberSplit(int value, char tens) {
	char* result = calloc(sizeof(char), tens + 1);
	if(!result)
		return NULL;
	
	result[0] = value / powi(10, tens);
	
	int curr;
	int res;
	for(int i = tens - 1; i >= 0; i--) {
		curr = tens - i;
		
		value = value - (result[curr - 1] * powi(10, curr - 1));
		res = value / powi(10, curr);
		result[curr] = res;
	}
	
	return result;
}

// Same as pow, but will work with integers
// Positive Y only
int powi(int x, unsigned int y) {
	int result = 1;
	
	for(int i = 0; i < y; i++) {
		result = result * x;
	}
	
	return result;
}