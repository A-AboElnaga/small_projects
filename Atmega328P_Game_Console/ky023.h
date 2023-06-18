#ifndef KY023_H
#define KY023_H

#include <avr/io.h>

typedef struct {
    uint8_t xPin;
    uint8_t yPin;
    uint8_t switchPin;
} KY023;

/**
 * @brief Initializes the KY-023 joystick module.
 * Configures the ADC and sets the switch pin as input with a pull-up resistor enabled.
 * @param ky023 Pointer to the KY023 struct representing the KY-023 joystick module.
 */
void ky023_init(KY023* ky023);

/**
 * @brief Reads the X-axis value from the KY-023 joystick module.
 * @param ky023 Pointer to the KY023 struct representing the KY-023 joystick module.
 * @return The converted X-axis value.
 */
uint16_t ky023_readXAxis(const KY023* ky023);

/**
 * @brief Reads the Y-axis value from the KY-023 joystick module.
 * @param ky023 Pointer to the KY023 struct representing the KY-023 joystick module.
 * @return The converted Y-axis value.
 */
uint16_t ky023_readYAxis(const KY023* ky023);

/**
 * @brief Reads the state of the switch from the KY-023 joystick module.
 * @param ky023 Pointer to the KY023 struct representing the KY-023 joystick module.
 * @return The state of the switch (1 if pressed, 0 if not pressed).
 */
uint8_t ky023_readSwitch(const KY023* ky023);

#endif
