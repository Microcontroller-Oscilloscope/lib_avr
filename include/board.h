/*
	board.h - configuration flags for AVR boards
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

#ifndef BOARD_UNO_H
#define BOARD_UNO_H

#ifdef ARDUINO_AVR_UNO
	#define UNOR3
	#define BOARD_FOUND
#endif

#ifdef UNOR3

	#include "board_uno_pins.h"

	#ifndef NUM_IO_PINS
		#define NUM_IO_PINS 20 // number pins available to controller
	#endif

	/****************************
	 * Board Overrides
	****************************/

	#ifndef NVM_SIZE
		#define NVM_SIZE 1024 // size in bytes of NVM
	#endif

	#ifndef STATUS_LED_PIN
		#define STATUS_LED_PIN INTERNAL_LED // pin for status LED
	#endif

	/****************************
	 * Test Timer Config
	****************************/

	#define TEST_FAST_FREQ 140000 // target frequency
	#define TEST_DELAY_RUNNER // enables testDelayRunner function

#endif
#endif