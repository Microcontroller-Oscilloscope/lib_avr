/*
	board_atmega328p_pins.h - pin configuration for atmega328p
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
 *                     +-\/-+
 *      (RESET) PC6   1|    |28  PC5 (D19, A5, SCL0)
 *    (D0, RX0) PD0   2|    |27  PC4 (D18, A4, SDA0)
 *    (D1, TX0) PD1   3|    |26  PC3 (D17, A3)
 *         (D2) PD2   4|    |25  PC2 (D16, A2)
 *      (D3, ~) PD3   5|    |24  PC1 (D15, A1)
 *         (D4) PD4   6|    |23  PC0 (D14, A0)
 *              VCC   7|    |22  GND
 *              GND   8|    |21  AREF
 *      (XTAL1) PB6   9|    |20  AVCC
 *      (XTAL2) PB7  10|    |19  PB5 (D13, SCK0)
 *      (D5, ~) PD5  11|    |18  PB4 (D12, CIPO0)
 *      (D6, ~) PD6  12|    |17  PB3 (D11, ~, COPI0)
 *         (D7) PD7  13|    |16  PB2 (D10, ~, CS0)
 *         (D8) PB0  14|    |15  PB1 (D9, ~)
 *                     +----+
 */

#ifdef PLATFORMIO
	#include <pins_arduino.h>
#endif

// digital
#ifndef D0
	#define D0 0
#endif
#ifndef D1
	#define D1 1
#endif
#ifndef D2
	#define D2 2
#endif
#ifndef D3
	#define D3 3
#endif
#ifndef D4
	#define D4 4
#endif
#ifndef D5
	#define D5 5
#endif
#ifndef D6
	#define D6 6
#endif
#ifndef D7
	#define D7 7
#endif
#ifndef D8
	#define D8 8
#endif
#ifndef D9
	#define D9 9
#endif
#ifndef D10
	#define D10 10
#endif
#ifndef D11
	#define D11 11
#endif
#ifndef D12
	#define D12 12
#endif
#ifndef D13
	#define D13 13
#endif
#ifndef D14
	#define D14 14
#endif
#ifndef D15
	#define D15 15
#endif
#ifndef D16
	#define D16 16
#endif
#ifndef D17
	#define D17 17
#endif
#ifndef D18
	#define D18 18
#endif
#ifndef D19
	#define D19 19
#endif

// serial
#ifndef RX0
	#define RX0 D0
#endif
#ifndef TX0
	#define TX0 D1
#endif
#ifndef SERIAL_COUNT
	#define SERIAL_COUNT 1
#endif

// SPI
#ifndef CS0
	#define CS0 D10
#endif
#ifndef COPI0
	#define COPI0 D11
#endif
#ifndef CIPO0
	#define CIPO0 D12
#endif
#ifndef SCK0
	#define SCK0 D13
#endif
#ifndef SPI_COUNT
	#define SPI_COUNT 1
#endif

// I2C
#ifndef SDA0
	#define SDA0 D18
#endif
#ifndef SCL0
	#define SCL0 D19
#endif
#ifndef I2C_COUNT
	#define I2C_COUNT 1
#endif

// analog
#ifndef ADC0
	#define ADC0 D14
#endif
#ifndef ADC1
	#define ADC1 D15
#endif
#ifndef ADC2
	#define ADC2 D16
#endif
#ifndef ADC3
	#define ADC3 D17
#endif
#ifndef ADC4
	#define ADC4 D18
#endif
#ifndef ADC5
	#define ADC5 D19
#endif
#ifndef ADC_COUNT
	#define ADC_COUNT 6
#endif

// DAC
#ifndef DAC_COUNT
	#define DAC_COUNT 0
#endif

// PWM
#ifndef PWM0
	#define PWM0 D3
#endif
#ifndef PWM1
	#define PWM1 D5
#endif
#ifndef PWM2
	#define PWM2 D6
#endif
#ifndef PWM3
	#define PWM3 D9
#endif
#ifndef PWM4
	#define PWM4 D10
#endif
#ifndef PWM5
	#define PWM5 D11
#endif
#ifndef PWM_COUNT
	#define PWM_COUNT 6
#endif

// LED
#ifndef INTERNAL_LED
	#define INTERNAL_LED D13
#endif
#ifndef LED_COUNT
	#define LED_COUNT 1
#endif