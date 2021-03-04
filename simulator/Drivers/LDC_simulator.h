#pragma once

#include <vector>
#include <string>
#include <LCD_ifc.hh>

class LDCsimulator : public LCD_ifc{
public:
    LDCsimulator();
	void clear() final;
	void writeReg(const uint8_t value) final;
	void writeData(const uint8_t value) final;
	void write(const uint8_t* text, const uint8_t size) final;
	void writeInt8(int8_t value, const uint8_t precision) final;
	void writeU8(uint8_t value, const uint8_t precision) final;
	void writeU16(uint16_t value, const uint8_t precision) final;
	void writeDouble(double value, const uint8_t precision, const uint8_t partPrecision) final;
	void writeHEX(const uint8_t value) final;
	void writeHEX(const uint16_t value) final;
	void writeHEX(const uint32_t value) final;
	void writeHelloWorld() final;
	void operator()(const uint8_t, const uint8_t) final;
	void init() final;
	const std::vector<std::string>& getScreenContent();
protected:
	std::vector<std::string> screenContent;
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
	uint8_t coordinateRows;
	uint8_t coordinateColumns;
};
