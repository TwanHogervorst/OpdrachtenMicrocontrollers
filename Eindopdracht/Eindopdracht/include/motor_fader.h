/*
 * motor_fader.h
 *
 * Created: 30-3-2022 13:34:49
 *  Author: twanh
 */ 


#ifndef MOTOR_FADER_H_
#define MOTOR_FADER_H_

typedef struct mfader_conf* mfader_handle_t;

void mfader_init_pwm();

mfader_handle_t mfader_init(int faderIndex, char adcChannel, int positivePin, int negativePin);
void mfader_destroy(mfader_handle_t* fader);

short mfader_get_raw_position(mfader_handle_t fader);
char mfader_get_position(mfader_handle_t fader);

void mfader_set_position(mfader_handle_t fader, char pos);

#define mfader_set_speed(speed) OCR3C = speed;

#endif /* MOTOR_FADER_H_ */