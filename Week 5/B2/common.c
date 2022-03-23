/*
 * common.c
 *
 * Created: 23-3-2022 14:44:46
 *  Author: twanh
 */ 

#define F_CPU 8e6
#include <util/delay.h>

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