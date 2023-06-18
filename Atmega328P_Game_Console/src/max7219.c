#include "max7219.h"
#include <avr/io.h>
#include "spi.h" 
#include "delay.h"
#include "spi.h"
#include "ky023.h"
#include "Engine.h"
#include "SpaceGame.h"
#include "Game.h"
#include "Vector2.h"

// Pin definitions
#define MAX7219_CS_PORT   PORTB
#define MAX7219_CS_PIN    PORTB2

// Constants for MAX7219 registers
#define MAX7219_NOOP      0x00
#define MAX7219_DIGIT0    0x01
#define MAX7219_DIGIT1    0x02
#define MAX7219_DIGIT2    0x03
#define MAX7219_DIGIT3    0x04
#define MAX7219_DIGIT4    0x05
#define MAX7219_DIGIT5    0x06
#define MAX7219_DIGIT6    0x07
#define MAX7219_DIGIT7    0x08
#define MAX7219_DECODE    0x09
#define MAX7219_INTENSITY 0x0A
#define MAX7219_SCANLIMIT 0x0B
#define MAX7219_SHUTDOWN  0x0C
#define MAX7219_DISPLAYTEST 0x0F

void MAX7219_Init() {
	MAX7219_Send(MAX7219_SHUTDOWN, 0x01);      // Enable display (wakeup from shutdown)
	MAX7219_Send(MAX7219_SCANLIMIT, 0x07);     // Scan limit: 8 digits
	MAX7219_Send(MAX7219_DECODE, 0x00);        // No BCD decoding
	MAX7219_Send(MAX7219_DISPLAYTEST, 0x00);   // Display test off
	MAX7219_Send(MAX7219_INTENSITY, 0x0F);     // Maximum intensity
	MAX7219_Clear();                           // Clear display
}

void MAX7219_SetRow(uint8_t row, uint8_t data) {
	if (row >= 1 && row <= 8) {
		MAX7219_Send(row, data);
	}
}

void MAX7219_Clear() {
	for (uint8_t row = 1; row <= 8; row++) {
		MAX7219_SetRow(row, 0x00);
	}
}

void MAX7219_DisplayPattern(uint8_t pattern[8]) {
	for (uint8_t row = 1; row <= 8; row++) {
		MAX7219_SetRow(row, pattern[row - 1]);
	}
}

void MAX7219_Send(uint8_t address, uint8_t data) {
	MAX7219_CS_PORT &= ~(1 << MAX7219_CS_PIN);  // Pull CS low
	
	// Send address and data
	SPI_Transmit(address);
	SPI_Transmit(data);
	
	MAX7219_CS_PORT |= (1 << MAX7219_CS_PIN);  // Pull CS high
}
