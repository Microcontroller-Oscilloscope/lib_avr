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

//#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <osc_common/common_timer.h>

//#define MS_TO_US 1000
#define TIME_UNIT_CONVERT 1000

volatile uint32_t delayCount = 0U;

hard_timer_return_t RUN_IN_RAM(delayCounter) delayCounter(hard_timer_param_t emptyParams) {
	delayCount++;
	HARD_TIMER_END();
}

void hardDelayMS(uint32_t delayAmount) {
	hard_timer_t timer = HARD_TIMER_INVALID;
	freq_t freq = 0;

	uint16_t leftover = 0;

	// delay s
	if (delayAmount >= TIME_UNIT_CONVERT) {
		leftover = delayAmount % TIME_UNIT_CONVERT;
		delayCount = 0;

		freq = 1;
		
		// try 1Hz
		if (!setHardTimer(&timer, &freq, &delayCounter, DEFAULT_HARD_TIMER_PRIORITY)) {
			freq = 100;
			cancelHardTimer(timer);
			timer = HARD_TIMER_INVALID;

			// try 100Hz
			if (!setHardTimer(&timer, &freq, &delayCounter, DEFAULT_HARD_TIMER_PRIORITY)) {
				cancelHardTimer(timer);
				return;
			}
		}

		while (delayCount < (delayAmount - leftover) * freq / TIME_UNIT_CONVERT) {
			yield();
		}
		cancelHardTimer(timer);
	}

	// delay ms
	if (leftover > 0) {
		freq = TIME_UNIT_CONVERT;
		delayCount = 0;
		timer = HARD_TIMER_INVALID;

		if (!setHardTimer(&timer, &freq, &delayCounter, DEFAULT_HARD_TIMER_PRIORITY)) {
			cancelHardTimer(timer);
			return;
		}
		while (delayCount < leftover * freq / TIME_UNIT_CONVERT) {}
		cancelHardTimer(timer);
	}
}

void hardDelayUS(uint32_t delayAmount) {

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
}