/*
	board_avr_delay.c - delay configuration for avr boards
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

#include <osc_common/common_thread.h>
#include <hardware_timer.h>

#if defined(ARDUINO) && !defined(OVERRIDE_ARDUINO_TIMER)
	#include <Arduino.h>
#endif

#if !defined(ARDUINO) || defined(OVERRIDE_ARDUINO_TIMER)

	//#define __DELAY_BACKWARD_COMPATIBLE__
	#include <util/delay.h>

	#define TIME_UNIT_CONVERT 1000

	volatile uint32_t delayCount = 0U;

	void RUN_IN_RAM(delayCounter) delayCounter(void *emptyParams) {
		delayCount++;
	}

#endif

void hardDelayMS(uint32_t delayAmount) {

	#if defined(ARDUINO) && !defined(OVERRIDE_ARDUINO_TIMER)
		delay(delayAmount);
	#else
		hard_timer_enum_t timer = HARD_TIMER_INVALID;
		hard_timer_freq_t freq = 0;

		uint16_t leftover = 0;

		// delay s
		if (delayAmount >= TIME_UNIT_CONVERT) {
			leftover = delayAmount % TIME_UNIT_CONVERT;
			delayCount = 0;

			freq = 1;
			
			// try 1Hz
			if (!setHardTimer(&timer, &freq, &delayCounter, NULL, HARD_TIMER_PRIORITY_DEFAULT)) {
				freq = 100;
				cancelHardTimer(timer);
				timer = HARD_TIMER_INVALID;

				// try 100Hz
				if (!setHardTimer(&timer, &freq, &delayCounter, NULL, HARD_TIMER_PRIORITY_DEFAULT)) {
					cancelHardTimer(timer);
					return;
				}
			}

			while (delayCount < (delayAmount - leftover) * freq / TIME_UNIT_CONVERT) {}
			cancelHardTimer(timer);
		}

		// delay ms
		if (leftover > 0) {
			freq = TIME_UNIT_CONVERT;
			delayCount = 0;
			timer = HARD_TIMER_INVALID;

			if (!setHardTimer(&timer, &freq, &delayCounter, NULL, HARD_TIMER_PRIORITY_DEFAULT)) {
				cancelHardTimer(timer);
				return;
			}
			while (delayCount < leftover * freq / TIME_UNIT_CONVERT) {}
			cancelHardTimer(timer);
		}
	#endif
}

void hardDelayUS(uint32_t delayAmount) {

	#if defined(ARDUINO) && !defined(OVERRIDE_ARDUINO_TIMER)
		delayMicroseconds(delayAmount);
	#else
		// TODO: implement us delay

		//_delay_us(delayAmount);

		/*if (delayAmount >= TIME_UNIT_CONVERT) {
			hardDelayMS(delayAmount / TIME_UNIT_CONVERT);
			delayAmount = delayAmount % TIME_UNIT_CONVERT;
		}

		while (delayAmount >= 100) {
			_delay_us(100);
			delayAmount -= 100;
		}
		while (delayAmount >= 10) {
			_delay_us(10);
			delayAmount -= 10;
		}*/

		// with other overhead, this probably add extra time
		/*while (delayAmount >= 1) {
			_delay_us(1);
			delayAmount -= 1;
		}*/
	#endif
}