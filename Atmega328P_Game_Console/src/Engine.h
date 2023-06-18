#include <stdbool.h>
#include "ky023.h"
#ifndef ENGINE_H
#define ENGINE_H

//Fire Switch on PORTD4
#define BUTTON_PIN PORTD4
//Joystick analog on PORTC
#define X_PIN PORTC0
#define Y_PIN PORTC1

//LED MATRIX on PORTB
#define DIN_PIN PORTB3
#define CS_PIN PORTB2
#define CLK_PIN PORTB5

typedef struct {
	// Input pins:
	const int buttonPin;
	const int xPin;
	const int yPin;

	// Display pins:
	const int dinPin;
	const int csPin;
	const int clkPin;

	// Display values:
	//1st display
	unsigned char rowsDisplayA[8];
	//2nd display
	unsigned char rowsDisplayB[8];
	
	//Engine and Game Update Times:
	float deltaTime;
	unsigned long time;

	// Player input info:
	int8_t inputX;
	int8_t inputY;
	bool buttonState;
	
	//bool buttonDown;
	//bool buttonUpThisFrame;
	//bool buttonDownThisFrame;
	//float buttonDownDuration;
	
} Engine;

void Engine_Init(Engine* engine);
void Engine_DrawToDisplay(Engine* engine);
void Engine_UpdateLoop(Engine* engine,const KY023* ky023, float deltaTime);
void Engine_ClearScreen(Engine* engine);
void Engine_SetPixelToValue(Engine* engine, int x, int y, bool on);
void Engine_SetPixel(Engine* engine, int x, int y);

#endif /* ENGINE_H */
