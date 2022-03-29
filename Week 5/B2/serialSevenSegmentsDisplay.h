/*
 * serialSevenSegmentsDisplay.h
 *
 * Created: 23-3-2022 14:43:58
 *  Author: twanh
 */ 


#ifndef SERIALSEVENSEGMENTSDISPLAY_H_
#define SERIALSEVENSEGMENTSDISPLAY_H_

void displayDriverInit();

void displayOn();
void displayOff();

void writeLedDisplay(int value);

#endif /* SERIALSEVENSEGMENTSDISPLAY_H_ */