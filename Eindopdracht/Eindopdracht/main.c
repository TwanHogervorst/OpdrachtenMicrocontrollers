/*
 * Eindopdracht.c
 *
 * Created: 29-3-2022 12:39:46
 * Author : twanh
 */ 

#define F_CPU 10e6

#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"

#define UART0BAUD 250000 // Baud rate USART0
#define MYUBRR F_CPU/16/UART0BAUD - 1 // my USART Baud Rate Register
#define MYHSUBRR F_CPU/8/UART0BAUD - 1

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
	UCSR0B |= BIT(TXEN);
}

void uart_stop_tx()
{
	UCSR0B &= ~BIT(TXEN);
}

void uart_send_char(char ch)
{
	while (!(UCSR0A & BIT(UDRE0))) ;
	UDR0 = ch;
}

int main(void)
{
	DDRE |= BIT(1);
	
	usart0_init();
    /* Replace with your application code */
	int brightness = 0;
    while (1) 
    {
		PORTE &= ~BIT(1);
		_delay_us(100);
		//PORTE |= BIT(1);
		
		uart_start_tx();
		uart_send_char(0x00);
		
		uart_send_char(0);
		uart_send_char(brightness);
		uart_send_char(0);
		uart_send_char(0);
		
		uart_send_char(0);
		uart_send_char(255-brightness);
		uart_send_char(0);
		uart_send_char(0);
		
		for(int i = 0; i < 512/4-2; i++)
		{
			uart_send_char(255);
			uart_send_char(255);
			uart_send_char(0);
			uart_send_char(brightness);
		}
		
		//for(int i = 0; i < 512-4; i++)
		//{
			//uart_send_char(0);
		//}
		uart_stop_tx();
		brightness = (brightness+10 % 256);
		
		wait(250);
    }
}

