/*
 * delay.c
 *
 * Created: 5/20/2023 11:02:38 PM
 *  Author: A
 */ 
#include <avr/io.h>
#include "delay.h"

void delay_ms(unsigned int milliseconds) {
	unsigned int i, j;

	for (i = 0; i < milliseconds; i++) {
		for (j = 0; j < 1600; j++) {
			asm volatile("nop"); // Perform a no-operation instruction
		}
	}
}
