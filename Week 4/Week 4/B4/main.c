/*
 *
 * Created: 16-3-2022 11:58:02
 * Author : Luuk Verhagen
 */ 

#include <avr/io.h>
#define F_CPU 8e6

#include "utils.h"
#include <util/delay.h>
#include "lcd.h"
#include <stdio.h>

void adcInit( void )
{
	//REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0
	ADMUX = 0b11000001; //result right adjusted, channel 1 selected, 
						//Internal 2.56V Voltage Reference
	
	//ADEN ADSC ADFR ADIF ADIE ADPS2 ADPS1 ADPS0
	ADCSRA = 0b11100110;
}

int main(void)
{
	DDRF = 0x00;
	DDRA = 0xFF;
	DDRB = 0xFF;
	adcInit();
	lcd_init();
	
	char s[50];
	volatile float temp;
	volatile int tempInt;
	volatile int tempDecimal;
	
    while (1) 
    {
		temp = (ADCL) / 2.56f;

		tempInt = (int)temp;
		tempDecimal = (int)((temp - tempInt) * 100);
		sprintf(s, "%d.%d", tempInt, tempDecimal);

		lcd_set_cursor(0);
		lcd_display_text(s);
		
		PORTB = ADCL;
		wait(1000);
    }
}