/*
 * dmx.c
 *
 * Created: 31-3-2022 15:35:39
 *  Author: twanh
 */ 

#define F_CPU 10e6

#include <stdlib.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "utils.h"

#include "dmx.h"

// DMX Protocol Settings
#define DMX_BAUDRATE 250000
#define DMX_CHANNEL_COUNT 512

#define UART_BAUD_REG_VALUE (F_CPU / 8 / DMX_BAUDRATE - 1)

volatile static char dmxData[DMX_CHANNEL_COUNT + 1];
volatile static int dmxDataIndex = 0;

volatile static bool dmxIsSending = false;

static char presetData[DMX_CHANNEL_COUNT + 1];

#define uart_start_tx() UCSR0B |= BIT(TXEN0)
#define uart_stop_tx() UCSR0B &= ~BIT(TXEN0)

#define uart_enable_empty_tx_buffer_interrupt() UCSR0B |= BIT(UDRIE0)
#define uart_disable_empty_tx_buffer_interrupt() UCSR0B &= ~BIT(UDRIE0)

ISR(USART0_UDRE_vect) {
	
	if(!dmxIsSending)
	{
		return;
	}
	
	UDR0 = dmxData[dmxDataIndex++];
	
	if(dmxDataIndex > 512)
	{
		uart_disable_empty_tx_buffer_interrupt();
		uart_stop_tx();
		
		dmxDataIndex = 0;
		dmxIsSending = false;
	}
	
}

void dmx_init() {
	
	// Initialize UART
	unsigned int ubrr = UART_BAUD_REG_VALUE;
	UBRR0H = (unsigned char)(ubrr>>8); // baudrate register, hoge byte
	UBRR0L = (unsigned char)(ubrr); // baudrate register, lage byte
	
	UCSR0C = 0b00001110; // async, 8 data/2 stop/no parity
	UCSR0B = 0b00000000; // receiver & transmitter disable
	UCSR0A |= BIT(1); // Double speed
	
	// Enable interrupts
	sei();
	
	// Set data[0] to 0 (required for DMX)
	dmxData[0] = 0;
	presetData[0] = 0;
	
	// DMX Requires data pin to be high when idle
	DDRE |= BIT(1);
	PORTE |= BIT(1);
	
}

void dmx_set(int channel, char value) {
	dmxData[bound(channel, 1, DMX_CHANNEL_COUNT)] = value;
}

char dmx_get(int channel) {
	return dmxData[bound(channel, 1, DMX_CHANNEL_COUNT)];
}

void dmx_start_send() {
	
	// Stop if already sending
	if(dmxIsSending)
		return;
	
	// Send start signal (pull low for 100us)
	PORTE &= ~BIT(1);
	_delay_us(100);
	PORTE |= BIT(1);
	
	dmxIsSending = true;
	uart_start_tx();
	uart_enable_empty_tx_buffer_interrupt();
	
}

void dmx_preset_save(){
	volatile int length = sizeof(presetData) / sizeof(presetData[0]);
	for(int i = 0; i < length; i++){
		presetData[i] = dmxData[i];
	}
}

void dmx_preset_load(){
	int length = sizeof(dmxData) / sizeof(dmxData[0]);
	for(int i = 0; i < length; i++){
		dmxData[i] = presetData[i];
	}
}