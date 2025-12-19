#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#include <stdint.h>

class __FlashStringHelper {};

#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(string_literal))

#define memcpy_P memcpy

#endif