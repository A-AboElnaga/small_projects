#ifndef MAX7219_H
#define MAX7219_H

#include <stdint.h>

void MAX7219_Init();
void MAX7219_SetRow(uint8_t row, uint8_t data);
void MAX7219_Clear();
void MAX7219_DisplayPattern(uint8_t pattern[8]);
void MAX7219_Send(uint8_t address, uint8_t data);

#endif
