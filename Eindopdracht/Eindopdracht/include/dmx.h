/*
 * dmx.h
 *
 * Created: 31-3-2022 15:35:51
 *  Author: twanh
 */ 


#ifndef DMX_H_
#define DMX_H_

void dmx_init();

void dmx_set(int channel, char value);
char dmx_get(int channel);

void dmx_start_send();

void dmx_preset_save();
void dmx_preset_load();

#endif /* DMX_H_ */