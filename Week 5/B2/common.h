/*
 * common.h
 *
 * Created: 23-3-2022 14:44:53
 *  Author: twanh
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#define BIT(x)		( 1<<x )

void wait(int ms);

char* numberSplit(int value, char tens);

int powi(int x, unsigned int y);

#endif /* COMMON_H_ */