/*
	board_avr_nvm.c - nvm configuration for avr boards
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

//#include <osc_common.h>
#include <nvm/nvm.h>
#include <board.h>

#include <string.h>
#include <avr/eeprom.h>

bool nvmBegan = false;

enum NVMStartCode nvmInit(nvm_size_t setNVMSize) {
	if (nvmBegan) {
		return NVM_STARTED;
	}
	if (setNVMSize == (nvm_size_t)DEFAULT_NVM_SIZE) {
		return NVM_INVALID_SIZE;
	}
	nvmBegan = true;
	return NVM_OK;
}

bool nvmMaxSize(nvm_size_t *size) {
	if (nvmBegan) {
		*size = (nvm_size_t)(E2END + 1);
		if (*size == 0) {
			*size = NVM_MAX_SIZE;
		}
		return true;
	}

	*size = DEFAULT_NVM_SIZE;
	return false;
}

enum NVMDefaultCode nvmSetDefaults(void) {
	// ensures NVM_SIZE isn't too big for microcontroller
	nvm_size_t nvmMaxValue;
	if (nvmMaxSize(&nvmMaxValue)) {
		if (NVM_SIZE > nvmMaxValue) {
			return NVM_DEFAULT_SIZE_TOO_BIG;
		}
	}
	else {
		// if nvm not started or unable to get size
		return NVM_DEFAULT_FAIL_MAX_SIZE;
	}

	// writes critical values
	enum NVMDefaultCode code = nvmSetCritDefaults(nvmMaxValue);
	if (code != NVM_DEFAULT_OK) {
		return code;
	}

	//writes platform values
	return nvmSetEnvDefaults();
}

#ifndef NO_CHAR_ARRAY_SUPPORT

	#include <comm/hard_serial/hard_serial.h>

	bool nvmWriteCharArray(nvm_size_t key, char* value, uint8_t maxLength) {
		if (!nvmBegan) {
			return false;
		}
		if (!validCharPointer(value)) {
			return false;
		}

		uint8_t valueLen = charArraySize(value);
		if (valueLen == 0) {
			return false;
		}
		else if (valueLen > maxLength) {
			return false;
		}
		else if (valueLen == CHAR_LEN_ERROR) {
			return false;
		}

		for (uint8_t i = 0; i < valueLen; i++) {
			nvmWriteI8(key + i, value[i]);
		}

		return true;
	}

	bool nvmGetCharArray(nvm_size_t key, char* value, uint8_t maxLength) {

		if (!nvmBegan) {
			return false;
		}

		if (!validCharPointer(value)) {
			return false;
		}
		if (maxLength == 0U) {
			return false;
		}

		for (uint8_t i = 0; i < maxLength; i++) {
			int8_t letter;
			nvmGetI8(key + i, &letter, CAN_DEFAULT);
			value[i] = letter;
			if (letter == END_OF_CHAR) {
				return true;
			}
		}

		return false;
	}

#endif

void nvmWrite(uint8_t addr, uint8_t val) {
	eeprom_busy_wait();
	__EEPUT(addr, val);
}

#define NVM_WRITE(key, value, type) \
	if (!nvmBegan) { \
		return false; \
	} \
	for (uint8_t i = 0; i < sizeof(type); i++) { \
		nvmWrite(key + i, (value >> (i*8))); \
	} \
	return true;

bool nvmWriteBool(nvm_size_t key, bool value) {
	NVM_WRITE(key, value, bool);
}

bool nvmWriteI8(nvm_size_t key, int8_t value) {
	NVM_WRITE(key, value, int8_t);
}

bool nvmWriteUI8(nvm_size_t key, uint8_t value) {
	NVM_WRITE(key, value, uint8_t);
}

bool nvmWriteI16(nvm_size_t key, int16_t value) {
	NVM_WRITE(key, value, int16_t);
}

bool nvmWriteUI16(nvm_size_t key, uint16_t value) {
	NVM_WRITE(key, value, uint16_t);
}

bool nvmWriteI32(nvm_size_t key, int32_t value) {
	NVM_WRITE(key, value, int32_t);
}

bool nvmWriteUI32(nvm_size_t key, uint32_t value) {
	NVM_WRITE(key, value, uint32_t);
}

bool nvmWriteI64(nvm_size_t key, int64_t value) {
	NVM_WRITE(key, value, int64_t);
}

bool nvmWriteUI64(nvm_size_t key, uint64_t value) {
	NVM_WRITE(key, value, uint64_t);
}

bool nvmWriteFloat(nvm_size_t key, float value) {
	if (!nvmBegan) {
		return false;
	}

	if (sizeof(float) == 4) {
		uint32_t intVal;
		memcpy(&intVal, &value, sizeof(uint32_t));
		nvmWriteUI32(key, intVal);
	}
	else if (sizeof(float) == 8) {
		uint64_t intVal;
		memcpy(&intVal, &value, sizeof(uint64_t));
		nvmWriteUI64(key, intVal);
	}
	else {
		return false;
	}

	return true;
}

bool nvmWriteDouble(nvm_size_t key, double value) {
	if (!nvmBegan) {
		return false;
	}

	if (sizeof(double) == 4) {
		uint32_t intVal;
		memcpy(&intVal, &value, sizeof(uint32_t));
		nvmWriteUI32(key, intVal);
	}
	else if (sizeof(double) == 8) {
		uint64_t intVal;
		memcpy(&intVal, &value, sizeof(uint64_t));
		nvmWriteUI64(key, intVal);
	}
	else {
		return false;
	}

	return true;
}

void nvmGet(uint8_t addr, uint8_t *var) {
	eeprom_busy_wait();
	__EEGET(*var, addr);
}

#define NVM_GET(key, value, type, canDefault, defaultValue) \
	if (!nvmBegan) { \
		return false; \
	} \
	for (uint8_t i = 0; i < sizeof(type); i++) { \
		nvmGet(key + i, (uint8_t*)value + i); \
	} \
	if (!canDefault && *value == defaultValue) { \
		return false; \
	} \
	return true;

bool nvmGetBool(nvm_size_t key, bool *value, bool canDefault) {
	NVM_GET(key, value, bool, canDefault, DEFAULT_BOOL);
}

bool nvmGetI8(nvm_size_t key, int8_t *value, bool canDefault) {
	NVM_GET(key, value, int8_t, canDefault, (int8_t)DEFAULT_INT);
}

bool nvmGetUI8(nvm_size_t key, uint8_t *value, bool canDefault) {
	NVM_GET(key, value, uint8_t, canDefault, (uint8_t)DEFAULT_INT);
}

bool nvmGetI16(nvm_size_t key, int16_t *value, bool canDefault) {
	NVM_GET(key, value, int16_t, canDefault, (int16_t)DEFAULT_INT);
}

bool nvmGetUI16(nvm_size_t key, uint16_t *value, bool canDefault) {
	NVM_GET(key, value, uint16_t, canDefault, (uint16_t)DEFAULT_INT);
}

bool nvmGetI32(nvm_size_t key, int32_t *value, bool canDefault) {
	NVM_GET(key, value, int32_t, canDefault, (int32_t)DEFAULT_INT);
}

bool nvmGetUI32(nvm_size_t key, uint32_t *value, bool canDefault) {
	NVM_GET(key, value, uint32_t, canDefault, (uint32_t)DEFAULT_INT);
}

bool nvmGetI64(nvm_size_t key, int64_t *value, bool canDefault) {
	NVM_GET(key, value, int64_t, canDefault, (int64_t)DEFAULT_INT);
}

bool nvmGetUI64(nvm_size_t key, uint64_t *value, bool canDefault) {
	NVM_GET(key, value, uint64_t, canDefault, (uint64_t)DEFAULT_INT);
}

bool nvmGetFloat(nvm_size_t key, float *value, bool canDefault) {
	if (!nvmBegan) {
		return false;
	}

	if (sizeof(float) == 4) {
		uint32_t intVal;
		nvmGetUI32(key, &intVal, true);
		memcpy(value, &intVal, sizeof(uint32_t));
	}
	else if (sizeof(float) == 8) {
		uint64_t intVal;
		nvmGetUI64(key, &intVal, true);
		memcpy(value, &intVal, sizeof(uint64_t));
	}
	else {
		return false;
	}

	return true;
}

bool nvmGetDouble(nvm_size_t key, double *value, bool canDefault) {
	if (!nvmBegan) {
		return false;
	}

	if (sizeof(double) == 4) {
		uint32_t intVal;
		nvmGetUI32(key, &intVal, true);
		memcpy(value, &intVal, sizeof(uint32_t));
	}
	else if (sizeof(double) == 8) {
		uint64_t intVal;
		nvmGetUI64(key, &intVal, true);
		memcpy(value, &intVal, sizeof(uint64_t));
	}
	else {
		return false;
	}

	return true;
}