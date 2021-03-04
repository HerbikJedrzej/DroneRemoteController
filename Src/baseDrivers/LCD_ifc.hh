#pragma once

#include <cstddef>
#include <cstdint>
#include <stdint.h>
#include <cstdlib>

class LCD_ifc{
public:
	virtual void clear() = 0;
	virtual void writeReg(const uint8_t value) = 0;
	virtual void writeData(const uint8_t value) = 0;
	virtual void write(const uint8_t* text, const uint8_t size) = 0;
	virtual void writeInt8(int8_t value, const uint8_t precision) = 0;
	virtual void writeU8(uint8_t value, const uint8_t precision) = 0;
	virtual void writeU16(uint16_t value, const uint8_t precision) = 0;
	virtual void writeDouble(double value, const uint8_t precision, const uint8_t partPrecision) = 0;
	virtual void writeHEX(const uint8_t value) = 0;
	virtual void writeHEX(const uint16_t value) = 0;
	virtual void writeHEX(const uint32_t value) = 0;
	virtual void writeHelloWorld() = 0;
	virtual void operator()(const uint8_t, const uint8_t) = 0;
	virtual void init() = 0;
};
