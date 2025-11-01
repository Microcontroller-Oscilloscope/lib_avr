/*
	board_uno_serial.c - serial configuration for Arduino Uno
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

#include <comm/hard_serial/hard_serial.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define UBRR_VAL ((F_CPU / 16) / BAUD_RATE - 1)

#if UBRR_VAL < 0
	#error BAUD_RATE too large (BAUD_RATE <= 1,000,000)
#elif (UBRR_VAL) > 4095
	#error BAUD_RATE too small (BAUD_RATE >= 245)
#endif

#if (F_CPU / 16) % BAUD_RATE != 0
	#warning BAUD_RATE will be slightly off set value. Consider setting a value divisible into 1,000,000. function: (1,000,000 / BAUD_RATE)
#endif

#define INT8_MAX_DIGITS 3 // max number of digits for int8
#define INT16_MAX_DIGITS 5 // max number of digits for int16
#define INT32_MAX_DIGITS 10 // max number of digits for int32

/**
 * Converts to n power of input datatypes
 * 
 * @param base base of exponent
 * @param power power of exponent
 * 
 * @return final value
 */
uint64_t toNPower(uint8_t base, uint8_t power) {
	uint64_t value = 1;
	for (uint8_t i = 0; i < power; i++) {
		value *= base;
	}
	return value;
}

/**
 * Sends byte of data to serial
 * 
 * @param value value to transmit
 */
void uartTransmit(uint8_t value) {
	while (!(UCSR0A & (1<<UDRE0))); // waits for empty transmit buffer
	cli();
	UDR0 = value;
	sei();
}

/**
 * Copies flash message to serial
 * 
 * @param message flash message to print
 * @param messageSize size of message to print
 */
void printMessage(memCharString *message, uint8_t messageSize) {
	for (uint8_t i = 0; i < messageSize; i++) {
		hardPrintChar((char)pgm_read_byte_near(message + i));
	}
}

void hardPrintBegin(uint32_t baud) {
	cli();
	// set baud rate
	UBRR0H = (uint8_t)(UBRR_VAL>>8);
	UBRR0L = (uint8_t)UBRR_VAL;
	// enable tx and rx
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	// frame format: 8 data, 2 stop
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
	sei();
}

void hardPrintMemCharArray(memCharString* value) {
	printMessage(value, strlen_P(value));
}

void hardPrintCharArray(const char* value) {
	uint8_t size = charArraySize(value);
	for (uint8_t i = 0; i < size - 1; i++) {
		uartTransmit(value[i]);
	}
}

void hardPrintBool(bool value) {
	uartTransmit(ASCII0 + value);
}

void hardPrintChar(char value) {
	uartTransmit(value);
}

void hardPrintInt8(int8_t value) {
	char buffer[INT8_MAX_DIGITS];
	sprintf(buffer, "%d", value);
	hardPrintCharArray(buffer);
}

void hardPrintUInt8(uint8_t value) {
	char buffer[INT8_MAX_DIGITS];
	sprintf(buffer, "%u", value);
	hardPrintCharArray(buffer);
}

void hardPrintInt16(int16_t value) {
	char buffer[INT16_MAX_DIGITS];
	sprintf(buffer, "%d", value);
	hardPrintCharArray(buffer);
}

void hardPrintUInt16(uint16_t value) {
	char buffer[INT16_MAX_DIGITS];
	sprintf(buffer, "%u", value);
	hardPrintCharArray(buffer);
}

void hardPrintInt32(int32_t value) {
	char buffer[INT32_MAX_DIGITS];
	sprintf(buffer, "%ld", value);
	hardPrintCharArray(buffer);
}

void hardPrintUInt32(uint32_t value) {
	char buffer[INT32_MAX_DIGITS];
	sprintf(buffer, "%lu", value);
	hardPrintCharArray(buffer);
}

void hardPrintInt64(int64_t value) {

	if (value & INT64_SIGN) {
		value *= -1;
		hardPrintChar('-');
	}

	hardPrintUInt64(value);
}

void hardPrintUInt64(uint64_t value) {

	// prints 0
	if (!value) {
		hardPrintChar('0');
		return;
	}

	int8_t digits = 0;

	{
		uint64_t temp = value;
		// gets count of digits
		while (temp > 0) {
			digits++;
			temp = temp/10U;
		}
	}

	// prints digits
	for (int8_t i = digits - 1; i >= 0; i--) {
		uint64_t power = toNPower((uint8_t)10, (uint8_t)i);
		uint64_t leftover = value % power;
		hardPrintUInt8((uint8_t)((value - leftover) / power));
		value -= value - leftover;
	}
}

void hardPrintFloat(float value) {}
void hardPrintFloatPlaces(float value, uint8_t places) {}
void hardPrintDouble(double value) {}
void hardPrintDoublePlaces(double value, uint8_t places) {}

void hardPrintMemCharArrayln(memCharString* value) {
	printMessage(value, strlen_P(value));
	uartTransmit('\n');
}

void hardPrintCharArrayln(const char* value) {
	uint8_t size = charArraySize(value);
	for (uint8_t i = 0; i < size - 1; i++) {
		uartTransmit(value[i]);
	}
	uartTransmit('\n');
}

void hardPrintBoolln(bool value) {
	uartTransmit(ASCII0 + value);
	uartTransmit('\n');
}

void hardPrintCharln(char value) {
	uartTransmit(value);
	uartTransmit('\n');
}

void hardPrintInt8ln(int8_t value) {
	char buffer[INT8_MAX_DIGITS];
	sprintf(buffer, "%d", value);
	hardPrintCharArrayln(buffer);
}

void hardPrintUInt8ln(uint8_t value) {
	char buffer[INT8_MAX_DIGITS];
	sprintf(buffer, "%u", value);
	hardPrintCharArrayln(buffer);
}

void hardPrintInt16ln(int16_t value) {
	char buffer[INT16_MAX_DIGITS];
	sprintf(buffer, "%d", value);
	hardPrintCharArrayln(buffer);
}

void hardPrintUInt16ln(uint16_t value) {
	char buffer[INT16_MAX_DIGITS];
	sprintf(buffer, "%u", value);
	hardPrintCharArrayln(buffer);
}

void hardPrintInt32ln(int32_t value) {
	char buffer[INT32_MAX_DIGITS];
	sprintf(buffer, "%ld", value);
	hardPrintCharArrayln(buffer);
}

void hardPrintUInt32ln(uint32_t value) {
	char buffer[INT32_MAX_DIGITS];
	sprintf(buffer, "%lu", value);
	hardPrintCharArrayln(buffer);
}

void hardPrintInt64ln(int64_t value) {
	hardPrintInt64(value);
	uartTransmit('\n');
}

void hardPrintUInt64ln(uint64_t value) {
	hardPrintUInt64(value);
	uartTransmit('\n');
}

void hardPrintFloatln(float value) {}
void hardPrintFloatPlacesln(float value, uint8_t places) {}
void hardPrintDoubleln(double value) {}
void hardPrintDoublePlacesln(double value, uint8_t places) {}

void hardPrintln(void) {
	uartTransmit('\n');
}