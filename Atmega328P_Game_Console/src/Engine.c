#include <stdint.h>
#include <avr/io.h>
#include "delay.h"
#include "max7219.h"
#include "spi.h"
#include "ky023.h"
#include "Engine.h"
#include "SpaceGame.h"
#include "Game.h"
#include "Vector2.h"


//Fire Switch on PORTD4
#define BUTTON_PIN PORTD4
//Joystick analog on PORTC
#define X_PIN PORTC0
#define Y_PIN PORTC1

//LED MATRIX on PORTB
#define DIN_PIN PORTB3
#define CS_PIN PORTB2
#define CLK_PIN PORTB5


void Engine_Init(Engine* engine) {
	DDRD &= ~(1 << BUTTON_PIN);  // Set BUTTON_PIN as input
	
	Engine_ClearScreen(engine);
	engine->time = 0;
}

void Engine_UpdateLoop(Engine* engine,const KY023* ky023, float deltaTime) {
	engine->deltaTime = deltaTime;
	engine->time += deltaTime;

	// Button input:
	engine->buttonState = (PIND & (1 << BUTTON_PIN)) >> BUTTON_PIN;
	//engine->buttonDown = engine->buttonState == 1;
	//engine->buttonDownThisFrame = engine->buttonDown && engine->buttonState != engine->buttonStateOld;
	//engine->buttonUpThisFrame = engine->buttonState == 0 && engine->buttonStateOld == 1;
	/*
	if (engine->buttonDownThisFrame) {
		engine->buttonDownDuration = 0;
	}
	if (engine->buttonDown) {
		engine->buttonDownDuration += deltaTime;
	}
	*/
	
	// Get analog stick input:
	//const float inputThreshold = 0.1;
	engine->inputX = ky023_readXAxis(ky023);
	engine->inputY = ky023_readYAxis(ky023);
	
	/*
	// Print input values (assuming you have a serial interface configured)
	printf("X: %.2f Y: %.2f\n", engine->inputX, engine->inputY);

	if (abs(engine->inputX) < inputThreshold) {
		engine->inputX = 0;
	}
	if (abs(engine->inputY) < inputThreshold) {
		engine->inputY = 0;
	}
	*/
}


void Engine_ClearScreen(Engine* engine) {
	for (int i = 0; i < 8; i++) {
		engine->rowsDisplayA[i] = 0;
		engine->rowsDisplayB[i] = 0;
	}
	Engine_DrawToDisplay(engine);
}

void Engine_DrawToDisplay(Engine* engine) {
	MAX7219_DisplayPattern(engine->rowsDisplayA);
}


void Engine_SetPixel(Engine* engine, int x, int y) {
	Engine_SetPixelToValue(engine, x, y, 1);
}

void Engine_SetPixelToValue(Engine* engine, int x, int y, bool on) {
	if (x >= 8 || x < 0 || y >= 8 || y < 0) {
		return;
	}

	if (on) {
		engine->rowsDisplayA[x] |= 1 << y;
		} else {
		engine->rowsDisplayA[x] &= ~(1 << y);
	}
	return;
}


