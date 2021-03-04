#include "LDC_simulator.h"

#define COLUMS 16
#define ROWS 4
#define SECOND_LINE_ADDRES 0x80

LDCsimulator::LDCsimulator():
	coordinateRows(0),
	coordinateColumns(0)
	{
		screenContent.push_back("xxxxxxxxxxxxxxxx");
		screenContent.push_back("xxxxxxxxxxxxxxxx");
		screenContent.push_back("xxxxxxxxxxxxxxxx");
		screenContent.push_back("xxxxxxxxxxxxxxxx");
	}

const std::vector<std::string>& LDCsimulator::getScreenContent(){
	return screenContent;
}

void LDCsimulator::clear(){
	for(auto& line : screenContent)
		for(auto& sign : line)
			sign = ' ';
}

void LDCsimulator::init(){
}

void LDCsimulator::writeHelloWorld(){
	writeData('H');
	writeData('e');
	writeData('l');
	writeData('l');
	writeData('o');
	writeData(' ');
	writeData('w');
	writeData('o');
	writeData('r');
	writeData('l');
	writeData('d');
	writeData('!');
}

void LDCsimulator::writeReg(const uint8_t){
}

void LDCsimulator::operator()(const uint8_t rows, const uint8_t cols){
	if(cols >= COLUMS)return;
	if(rows >= ROWS)return;
	coordinateRows = rows;
	coordinateColumns = cols;
}

void LDCsimulator::writeData(const uint8_t value){
	screenContent[coordinateRows][coordinateColumns++] = char(value);
	if(coordinateColumns >= COLUMS){
		coordinateColumns = 0;
		coordinateRows++;
	}
	if(coordinateRows >= ROWS)
		coordinateRows = 0;
}

void LDCsimulator::write(const uint8_t* text, const uint8_t size){
	for(uint8_t i = 0; i < size; i++)
		writeData(text[i]);
}

void LDCsimulator::writeU8(uint8_t value, const uint8_t precision){
	uint8_t signs[3];
	for(uint8_t i = 0; i < 3; i++){
		signs[2 - i] = hexToChar[value % 10];
		value = value / 10;
	}
	for(uint8_t i = 0; i < 2; i++){
		if(signs[i] =='0')
			signs[i] = ' ';
		else
			break;
	}
	write(signs, precision);
}

void LDCsimulator::writeInt8(int8_t value, const uint8_t precision){
	if(value < 0){
		writeData('-');
		writeU8(uint8_t(-value), precision);
	}
	else{
		writeData(' ');
		writeU8(uint8_t(value), precision);
	}
}

void LDCsimulator::writeDouble(double value, const uint8_t precision, const uint8_t partPrecision){
	uint8_t signs[16]{'-'};
	if(precision + partPrecision > 14)
		return;
	if(value < 0)
		value = -value;
	else
		signs[0] = ' ';
	double pow = 1;
	for(uint8_t i = 0; i < partPrecision; i++)
		pow *= 10;
	uint32_t above1 = value;
	uint32_t below1 = (value - above1) * pow;
	for(uint8_t i = 0; i < precision; i++){
		signs[precision - i] = hexToChar[above1 % 10];
		above1 = above1 / 10;
	}
	signs[precision + 1] = '.';
	for(uint8_t i = precision + partPrecision + 1; i > precision + 1; i--){
		signs[i] = hexToChar[below1 % 10];
		below1 = below1 / 10;
	}
	for(uint8_t i = 1; i < precision; i++){
		if(signs[i] =='0')
			signs[i] = ' ';
		else
			break;
	}
	write(signs, precision + partPrecision + 2);
}

void LDCsimulator::writeU16(uint16_t value, const uint8_t precision){
	uint8_t signs[5];
	for(uint8_t i = 0; i < 5; i++){
		signs[4 - i] = hexToChar[value % 10];
		value = value / 10;
	}
	for(uint8_t i = 0; i < 2; i++){
		if(signs[i] =='0')
			signs[i] = ' ';
		else
			break;
	}
	write(signs, precision);
}

void LDCsimulator::writeHEX(const uint32_t value){
	uint8_t signs[8];
	for(uint8_t i = 0; i < 8; i++)
		signs[7 - i] = hexToChar[(value >> (4 * i)) & 0xf];
	write(signs, 8);
}

void LDCsimulator::writeHEX(const uint16_t value){
	uint8_t signs[4];
	for(uint8_t i = 0; i < 4; i++)
		signs[3 - i] = hexToChar[(value >> (4 * i)) & 0xf];
	write(signs, 4);
}

void LDCsimulator::writeHEX(const uint8_t value){
	uint8_t signs[2];
	for(uint8_t i = 0; i < 2; i++)
		signs[1 - i] = hexToChar[(value >> (4 * i)) & 0xf];
	write(signs, 2);
}
