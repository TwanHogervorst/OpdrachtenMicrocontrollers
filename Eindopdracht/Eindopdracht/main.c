/*
 * Eindopdracht.c
 *
 * Created: 29-3-2022 12:39:46
 * Author : twanh
 */ 

#define F_CPU 10e6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#define UART0BAUD 250000 // Baud rate USART0
#define MYUBRR F_CPU/16/UART0BAUD - 1 // my USART Baud Rate Register
#define MYHSUBRR F_CPU/8/UART0BAUD - 1

volatile static char data[513];
//volatile unsigned char* data;
volatile int data_index = 0;
volatile int is_sending = 0;

void usart0_init( void ) // initialize USART0 - receive/transmit
{
	unsigned int ubrr = MYHSUBRR;
	UBRR0H = (unsigned char)(ubrr>>8); // baudrate register, hoge byte
	UBRR0L = (unsigned char)(ubrr); // baudrate register, lage byte
	UCSR0C = 0b00001110; // a sync, 8 data/2 stop/no parity
	UCSR0B = 0b00000000; // receiver & transmitter disable
	UCSR0A |= BIT(1);
}

void uart_start_tx()
{
	UCSR0B |= BIT(TXEN0);
}

void uart_stop_tx()
{
	UCSR0B &= ~BIT(TXEN0);
}

void uart_enable_empty_tx_buffer_interrupt()
{
	UCSR0B |= BIT(UDRIE0);
}

void uart_disable_empty_tx_buffer_interrupt()
{
	UCSR0B &= ~BIT(UDRIE0);
}

void uart_send_char(char ch)
{
	while (!(UCSR0A & BIT(UDRE0))) ;
	UDR0 = ch;
}

ISR(USART0_UDRE_vect)
{
	if(!is_sending)
	{
		return;
	}
	
	UDR0 = data[data_index++];
	
	if(data_index > 512)
	{
		uart_disable_empty_tx_buffer_interrupt();
		uart_stop_tx();
		data_index = 0;
		is_sending = 0;
	}
}

int main(void)
{
	DDRE |= BIT(1);
	
	usart0_init();
	sei();
    /* Replace with your application code */
	volatile unsigned int brightness = 0;
	data[1] = brightness;
    while (1) 
    {
		PORTE &= ~BIT(1);
		_delay_us(100);
		PORTE |= BIT(1);
		
		data[0] = 0;
		data[1] = brightness;
		data[2] = 0;
		
		is_sending = 1;
		uart_start_tx();
		uart_enable_empty_tx_buffer_interrupt();
		while(is_sending)
		{
			wait(1);
		}
		is_sending = 0;
		brightness = (brightness+10 % 256);
		
		wait(100);
    }
}

