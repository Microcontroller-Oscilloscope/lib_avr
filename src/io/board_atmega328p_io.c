/*
	board_atmega328p_io.c - IO configuration for atmega328p
	Copyright (C) 2025 Camren Chraplak

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <osc_common/common_io.h>
#include <status/status.h>
#include "../pins/pins.h"

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t claimedPins[3] = {0, 0, 0};

const uint8_t bitMask[] PROGMEM = {
	TO_BIT(0), // port D
	TO_BIT(1),
	TO_BIT(2),
	TO_BIT(3),
	TO_BIT(4),
	TO_BIT(5),
	TO_BIT(6),
	TO_BIT(7),
	TO_BIT(0), // port B
	TO_BIT(1),
	TO_BIT(2),
	TO_BIT(3),
	TO_BIT(4),
	TO_BIT(5),
	TO_BIT(0), // port C
	TO_BIT(1),
	TO_BIT(2),
	TO_BIT(3),
	TO_BIT(4),
	TO_BIT(5),
};

#define GET_BIT_MASK(i) pgm_read_byte_near(bitMask + i)

#define OSC_PORTB 0
#define OSC_PORTC 1
#define OSC_PORTD 2

const uint8_t portIDMask[] PROGMEM = {
	OSC_PORTD, // port D
	OSC_PORTD,
	OSC_PORTD,
	OSC_PORTD,
	OSC_PORTD,
	OSC_PORTD,
	OSC_PORTD,
	OSC_PORTD,
	OSC_PORTB, // port B
	OSC_PORTB,
	OSC_PORTB,
	OSC_PORTB,
	OSC_PORTB,
	OSC_PORTB,
	OSC_PORTC, // port C
	OSC_PORTC,
	OSC_PORTC,
	OSC_PORTC,
	OSC_PORTC,
	OSC_PORTC,
};

#define GET_PORT_ID_MASK(i) pgm_read_byte_near(portIDMask + (i))

const uint16_t ddrMask[] PROGMEM = {
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

// rw pin direction port (1: output, 0: input)
#define GET_DDR_MASK(i) ((volatile uint8_t*)(pgm_read_word_near(ddrMask + (i))))

const uint16_t portMask[] PROGMEM = {
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

// rw port resistor (1: input pullup, 0: input)
#define GET_PORT_MASK(i) ((volatile uint8_t*)(pgm_read_word_near(portMask + (i))))

// r input value
#define GET_PIN_MASK(i)

bool initBoard() {
	return true;
}

void hardPinMode(pin_t pin, enum pinModeState mode) {

	uint8_t bitMasked = GET_BIT_MASK(pin);
	uint8_t pinPortID = GET_PORT_ID_MASK(pin);

	volatile uint8_t *pinDDR, *pinPORT;

	pinDDR = GET_DDR_MASK(pinPortID);
	pinPORT = GET_PORT_MASK(pinPortID);

	if (mode == PIN_MODE_INPUT) {
		cli();
		*pinDDR &= ~bitMasked;
		*pinPORT &= ~bitMasked;
		sei();
	}
	else if (mode == PIN_MODE_OUTPUT) {
		cli();
		*pinDDR |= bitMasked;
		sei();
	}
	else if (mode == PIN_MODE_INPUT_PULL_UP) {
		cli();
		*pinDDR &= ~bitMasked;
		*pinPORT |= bitMasked;
		sei();
	}
}

void hardDigitalWrite(pin_t pin, uint8_t value) {

	uint8_t bitMasked = GET_BIT_MASK(pin);
	uint8_t pinPortID = GET_PORT_ID_MASK(pin);
	volatile uint8_t* pinPORT = GET_PORT_MASK(pinPortID);
	
	if (value) {
		cli();
		*pinPORT |= bitMasked;
		sei();
	}
	else {
		cli();
		*pinPORT &= ~bitMasked;
		sei();
	}
}

bool getStatusPin(pin_t *pin, enum statusPin status) {
	if (status == STATUS_PIN_INTERNAL) {
		*pin = INTERNAL_LED;
		return true;
	}
	*pin = PIN_T_INVALID;
	return false;
}