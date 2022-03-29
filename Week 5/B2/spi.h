/*
 * spi.h
 *
 * Created: 23-3-2022 14:42:29
 *  Author: twanh
 */ 


#ifndef SPI_H_
#define SPI_H_

void spi_masterInit(void);

void spi_write( unsigned char data );
char spi_writeRead( unsigned char data );

void spi_slaveSelect(unsigned char chipNumber);
void spi_slaveDeSelect(unsigned char chipNumber);

void spi_writeWord(unsigned char adress, unsigned char data);

#endif /* SPI_H_ */