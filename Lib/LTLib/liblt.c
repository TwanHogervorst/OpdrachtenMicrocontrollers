/*
 * liblt.c
 *
 * Created: 9-2-2022 10:13:30
 *  Author: twanh
 */ 

#include "liblt.h"

void wait(int ms)
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}