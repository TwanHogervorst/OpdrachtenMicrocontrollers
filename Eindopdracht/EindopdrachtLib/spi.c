/*
 * spi.c
 *
 * Created: 29-3-2022 14:40:32
 *  Author: twanh
 */ 

#include <avr/io.h>

#include "utils.h"

#include "spi.h"

#define DDR_SPI		DDRB					// spi Data direction register
#define PORT_SPI	PORTB					// spi Output register
#define SPI_SCK		1						// PB1: spi Pin System Clock
#define SPI_MOSI	2						// PB2: spi Pin MOSI
#define SPI_MISO	3						// PB3: spi Pin MISO
#define SPI_SS		0						// PB0: spi Pin Slave Select

static void write(unsigned char data);
static void slave_select(unsigned char chipNumber);
static void slave_deselect(unsigned char chipNumber);

void spi_master_init(void)
{
	DDR_SPI = 0xff;							// All pins output: MOSI, SCK, SS, SS_display as output
	DDR_SPI &= ~BIT(SPI_MISO);				// 	except: MISO input
	PORT_SPI |= BIT(SPI_SS);				// SS_ADC == 1: deselect slave
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);	// or: SPCR = 0b11010010;
	// Enable spi, MasterMode, Clock rate fck/64, bitrate=125kHz
	// Mode = 0: CPOL=0, CPPH=0;
}

void spi_write(unsigned char slavePin, unsigned char data) {
	slave_select(slavePin);
	write(data);
	slave_deselect(slavePin);
}

// Write a word = address byte + data byte from master to slave
void spi_write_word(unsigned char slavePin, unsigned char adress, unsigned char data)
{
	slave_select(slavePin);
	write(adress);
	write(data);
	slave_deselect(slavePin);
}

char spi_write_read(unsigned char slavePin, unsigned char data) {
	
	slave_select(slavePin);
	write(data);
	data = SPDR;
	slave_deselect(slavePin);
	
	return data;
}

// Write a byte from master to slave
static void write( unsigned char data )
{
	SPDR = data;							// Load byte to Data register --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); 			// Wait for transmission complete
}

// Select device on pinnumer PORTB
static void slave_select(unsigned char chipNumber)
{
	PORTB &= ~BIT(chipNumber);
}

// Deselect device on pinnumer PORTB
static void slave_deselect(unsigned char chipNumber)
{
	PORTB |= BIT(chipNumber);
}