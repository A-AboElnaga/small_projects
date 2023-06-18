/*
 * spi.h
 *
 * Created: 5/20/2023 1:16:58 AM
 *  Author: A
 */ 


#ifndef SPI_H_
#define SPI_H_


#include <stdint.h>

void SPI_Init();
void SPI_Transmit(uint8_t data);


#endif /* SPI_H_ */
