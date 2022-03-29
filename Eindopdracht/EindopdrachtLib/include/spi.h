/*
 * spi.h
 *
 * Created: 29-3-2022 14:40:38
 *  Author: twanh
 */ 


#ifndef SPI_H_
#define SPI_H_

#define spi_read(slavePin) spi_write_read(slavePin, 0x00)

void spi_master_init(void);

void spi_write(unsigned char slavePin, unsigned char data);
void spi_write_word(unsigned char slavePin, unsigned char adress, unsigned char data);
char spi_write_read(unsigned char slavePin, unsigned char data);



#endif /* SPI_H_ */