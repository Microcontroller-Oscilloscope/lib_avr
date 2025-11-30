/*
	board_avr_timer.h - timer configuration for avr
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

/**
 * Arduino functionality that uses hardware timers
 * 
 * wiring.c
 * 		millis()
 * 			Uses one timer
 * wiring_analog.c
 * 		PWM/AnalogWrite
 * 			Uses all timers if all PWM pins used
 * Tone.cpp
 * 		Tone generation
 * 			Uses one timer
 */

#include <osc_common/common_timer.h>

#if defined(__AVR_ATmega328P__)
	#include "board_atmega328p_timer.h"
#endif