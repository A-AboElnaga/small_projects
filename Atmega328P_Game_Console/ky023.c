#include "ky023.h"
#include "delay.h"
#include "max7219.h"
#include "spi.h"
#include "ky023.h"
#include "Engine.h"
#include "SpaceGame.h"
#include "Game.h"
#include "Vector2.h"

void ky023_init(KY023* ky023) {
	// Configure the switch pin as input
	DDRD &= ~(1 << ky023->switchPin);
	PORTD |= (1 << ky023->switchPin); // Enable internal pull-up resistor

	// Initialize the ADC
	ADMUX = (1 << REFS0); // Reference voltage AVcc with external capacitor at AREF pin
	ADMUX &=~(1<<ADLAR);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1<<ADPS0); // Enable ADC with prescaler = 128
}

uint16_t ky023_readXAxis(const KY023* ky023) {
	ADMUX = (ADMUX & 0xF0) | ky023->xPin; // Select the X-axis pin
	ADCSRA |= (1 << ADSC); // Start the conversion
	while (ADCSRA & (1 << ADSC)) {;} // Wait for the conversion to complete
	
	//tune the value
	if(ADC>=775) return 1;
	else if(ADC<=250) return 2;
	else if(ADC>250 || ADC<775)return 0;
	return 0; //
}

uint16_t ky023_readYAxis(const KY023* ky023) {
	ADMUX = (ADMUX & 0xF0) | ky023->yPin; // Select the Y-axis pin
	ADCSRA |= (1 << ADSC); // Start the conversion
	while (ADCSRA & (1 << ADSC)) {;} // Wait for the conversion to complete
		
	//tune the value
	if(ADC>=775) return 1;
	else if(ADC<=250) return 2;
	else if(ADC>250 || ADC<775)return 0;
	return 0; //
}

uint8_t ky023_readSwitch(const KY023* ky023) {
	return !(PIND & (1 << ky023->switchPin)); // Invert the value because the switch is active low
}
