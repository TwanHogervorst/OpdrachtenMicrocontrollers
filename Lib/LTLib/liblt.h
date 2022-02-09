/*
 * liblt.h
 *
 * Created: 9-2-2022 10:13:45
 *  Author: twanh
 */ 


#ifndef LIBLT_H_
#define LIBLT_H_

#ifdef PF_SIMULATOR
	#define F_CPU 1e6
#else
	#define F_CPU 8e6
#endif

#include <xc.h>
#include <util/delay.h>


#endif /* LIBLT_H_ */