/*
 * serial_seven_segment.h
 *
 * Created: 29-3-2022 15:03:35
 *  Author: twanh
 */ 


#ifndef SERIAL_SEVEN_SEGMENT_H_
#define SERIAL_SEVEN_SEGMENT_H_

#ifndef SSS_SLAVE_PORT
#define SSS_SLAVE_PORT 0
#endif

void sss_display_driver_init();

void sss_display_on();
void sss_display_off();

void sss_write(int value);

#endif /* SERIAL_SEVEN_SEGMENT_H_ */