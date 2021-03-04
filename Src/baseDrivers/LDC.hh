#pragma once

#include "LCD_ifc.hh"
#include <cstddef>
#include <cstdint>
#include <stdint.h>
#include <cstdlib>
#include <GPIO.hh>

class LDC : LCD_ifc{
public:
	LDC(Drivers::GPIO& gpioHandler, OutputList RSpin, OutputList RWpin, OutputList Epin, volatile uint32_t& port, void (*delay)(uint32_t));
	void clear() override;
	void writeReg(const uint8_t value) override;
	void writeData(const uint8_t value) override;
	void write(const uint8_t* text, const uint8_t size) override;
	void writeInt8(int8_t value, const uint8_t precision) override;
	void writeU8(uint8_t value, const uint8_t precision) override;
	void writeU16(uint16_t value, const uint8_t precision) override;
	void writeDouble(double value, const uint8_t precision, const uint8_t partPrecision) override;
	void writeHEX(const uint8_t value) override;
	void writeHEX(const uint16_t value) override;
	void writeHEX(const uint32_t value) override;
	void writeHelloWorld() override;
	void operator()(const uint8_t, const uint8_t) override;
	void init() override;
protected:
	Drivers::GPIO& gpio;
	const OutputList pinRS;
	const OutputList pinRW;
	const OutputList pinE;
	volatile uint32_t& dataPort;
private:
	const char hexToChar[16] = {
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'A',
		'B',
		'C',
		'D',
		'E',
		'F'
	};
	void delay10us();
	void setReg(uint8_t value);
};
