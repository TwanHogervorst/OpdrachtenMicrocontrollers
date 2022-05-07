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

#define sss_write(value) sss_write_offset(value, 0)
#define sss_write_upper(value) sss_write_offset(value, 4)

void sss_display_driver_init();

void sss_display_on();
void sss_display_off();

void sss_write_offset(int value, int offset);

#endif /* SERIAL_SEVEN_SEGMENT_H_ */