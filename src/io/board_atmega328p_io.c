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

#include <osc_common.h>

uint8_t claimedPins[3] = {0, 0, 0};

struct pinData {
	volatile uint8_t* PORT; // rw port resistor (1: input pullup, 0: input)
	volatile uint8_t* DDR; // rw pin direction port (1: output, 0: input)
	volatile uint8_t* PIN; // r input value
};

const struct pinData PINS_B PROGMEM = {
	.PORT = &PORTB,
	.DDR = &DDRB,
	.PIN = &PINB,
};

const struct pinData PINS_C PROGMEM = {
	.PORT = &PORTC,
	.DDR = &DDRC,
	.PIN = &PINC,
};

const struct pinData PINS_D PROGMEM = {
	.PORT = &PORTD,
	.DDR = &DDRD,
	.PIN = &PIND,
};

//const struct pinData digitalPins[] PROGMEM = {
const struct pinData *const digitalPins[] PROGMEM = {
	&PINS_D, // port D
	&PINS_D,
	&PINS_D,
	&PINS_D,
	&PINS_D,
	&PINS_D,
	&PINS_D,
	&PINS_D,
	&PINS_B, // port B
	&PINS_B,
	&PINS_B,
	&PINS_B,
	&PINS_B,
	&PINS_B,
	&PINS_C, // port C
	&PINS_C,
	&PINS_C,
	&PINS_C,
	&PINS_C,
	&PINS_C,
};

const uint8_t pinMask[] PROGMEM = {
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

bool initBoard() {
	return true;
}

void hardPinMode(pin_t pin, enum pinModeState mode) {

	if (mode == PIN_MODE_INPUT) {
		*(digitalPins[pin] -> DDR) &= ~(pinMask[pin]); // sets input
	}
	else if (mode == PIN_MODE_OUTPUT) {
		*(digitalPins[pin] -> DDR) |= (pinMask[pin]); // sets output
	}
	else if (mode == PIN_MODE_INPUT_PULL_UP) {
		*(digitalPins[pin] -> DDR) &= ~(pinMask[pin]); // sets input
		*(digitalPins[pin] -> PORT) |= (pinMask[pin]); // sets pullup
	}
}

void hardDigitalWrite(pin_t pin, enum digitalState value) {
	
	if (value) {
		*(digitalPins[pin] -> PORT) |= (pinMask[pin]); // sets HIGH
	}
	else {
		*(digitalPins[pin] -> PORT) &= ~(pinMask[pin]); // sets LOW
	}
}