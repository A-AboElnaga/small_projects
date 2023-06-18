/*
 * spi.c
 *
 * Created: 5/20/2023 1:17:24 AM
 *  Author: A
 */ 

#include "spi.h"
#include <avr/io.h>

#define DIN_PIN PORTB3
#define CS_PIN PORTB2
#define CLK_PIN PORTB5

void SPI_Init() {
	// Set MOSI(DIN), SCK(CLK), and SS/CS pins as outputs
	DDRB |= (1 << CS_PIN) | (1 << DIN_PIN) | (1 << CLK_PIN);
	
	// Enable SPI, set as master, and clock rate prescaler fck/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_Transmit(uint8_t data) {
	// Start transmission
	SPDR = data;
	
	// Wait for transmission complete
	while (!(SPSR & (1 << SPIF)))
	;
}
