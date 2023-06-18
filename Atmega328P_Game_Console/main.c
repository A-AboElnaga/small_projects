
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
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

const bool showStartupSequence = true;
const float deltaTime= 100;
int activeGameIndex = 0;
unsigned long timeOld;


KY023 ky023;
Engine engine={
	.buttonPin = BUTTON_PIN,
	.xPin = X_PIN,
	.yPin = Y_PIN,
	.dinPin = DIN_PIN,
	.csPin = CS_PIN,
	.clkPin = CLK_PIN,
	.deltaTime = 0.0,
	.time = 0,
	.inputX = 0.0,
	.inputY = 0.0,
	.buttonState = false
	};
SpaceGame game;


void handleGames(float deltaTime);
void startupSequence();
void setup();
void loop();

int main(void){
	setup();
	while(1){
		loop();	
	}
}


void setup() {
	SPI_Init();
	MAX7219_Init();	
	ky023.switchPin=BUTTON_PIN;
	ky023.xPin=X_PIN;
	ky023.yPin=Y_PIN;
	ky023_init(&ky023);
	SpaceGame_Init(&game);
	Engine_Init(&engine);
	
	if (showStartupSequence) {
		startupSequence(&engine);
	}
}


void loop() {
	Engine_ClearScreen(&engine);
	SpaceGame_UpdateLoop(&game,&engine,&ky023);
	Engine_UpdateLoop(&engine,&ky023,deltaTime);
	Engine_DrawToDisplay(&engine);
	delay_ms(250);
}



void startupSequence(Engine* engine) {
	uint8_t x = 0b10101010;
	for (int y = 0; y < 8; y++) {
		engine->rowsDisplayA[y]=(0xFF ^x);
		x=~x;
	}
	Engine_DrawToDisplay(engine);
	delay_ms(300);
	Engine_ClearScreen(engine);
	delay_ms(300);
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (x <= y) {
				Engine_SetPixel(engine,x, y);
			}
			else break;
		}
	}
	Engine_DrawToDisplay(engine);
	delay_ms(300);
	
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
				Engine_SetPixel(engine,x, y);
		}
	}
	Engine_DrawToDisplay(engine);
	delay_ms(700);

	for (int i = 0; i < 8; i++) {
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				if ((x + (8 - y) <= i || 8 - x + y <= i)) {
					Engine_SetPixelToValue(engine,x, y, false);
				}
			}
		}
		Engine_DrawToDisplay(engine);
		delay_ms(50);
	}
	delay_ms(30);
}
