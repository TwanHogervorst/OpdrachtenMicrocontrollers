/*
 * utils.h
 *
 * Created: 29-3-2022 12:46:26
 *  Author: twanh
 */ 


#ifndef UTILS_H_
#define UTILS_H_

#define BIT(x) (1 << x)

void wait(int ms);

int bound(int value, int min, int max);

double map(double value, double start1, double stop1, double start2, double stop2);

#define bound_map(value, start1, stop1, start2, stop2) bound(map(value, start1, stop1, start2, stop2), start2, stop2)

#endif /* UTILS_H_ */