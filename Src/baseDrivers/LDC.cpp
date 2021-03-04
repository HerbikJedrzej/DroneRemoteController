#include "LDC.hh"

#define COLUMS 16
#define ROWS 4
#define SECOND_LINE_ADDRES 0x80

LDC::LDC(Drivers::GPIO& gpioHandler, OutputList RSpin, OutputList RWpin, OutputList Epin, volatile uint32_t& port, void (*delay)(uint32_t)):
	gpio(gpioHandler),
	pinRS(RSpin),
	pinRW(RWpin),
	pinE(Epin),
	dataPort(port){
	gpio.pin(pinRW, false);
	delay(15);
	writeReg(0x30);
	delay(5);
	writeReg(0x30);
	delay(1);
	writeReg(0x30);
	delay(1);
	writeReg(0b00111100);
	delay(1);
	writeReg(0b00010100);
	delay(1);
	writeReg(0b00001000);
	delay(1);
	writeReg(0x01);
	delay(2);
	writeReg(0x06);
	delay(1);
	writeReg(0b00001100);

}

void LDC::clear(){
	writeReg(0x01);
}

void LDC::init(){
//	writeReg(0b00100100);
//	writeReg(0b00001100);
//	writeReg(0x06);
}

void LDC::writeHelloWorld(){
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

void LDC::writeReg(const uint8_t value){
	gpio.pin(pinRS, false);
	setReg(value);
	gpio.pin(pinE, false);
	delay10us();
	gpio.pin(pinE, true);
}

void LDC::delay10us(){
	for(volatile uint8_t i = 0; i < 70; i++);
}

void LDC::operator()(const uint8_t rows, const uint8_t cols){
	if(cols >= COLUMS)return;
	if(rows >= ROWS)return;
	if(rows % 2 == 1)
		writeReg(cols | (COLUMS*(rows/2)) | SECOND_LINE_ADDRES | 0x40);
	else
		writeReg(cols | (COLUMS*(rows/2)) | SECOND_LINE_ADDRES);
}

void LDC::writeData(const uint8_t value){
	gpio.pin(pinRS, true);
	setReg(value);
	gpio.pin(pinE, false);
	delay10us();
	gpio.pin(pinE, true);
}

void LDC::write(const uint8_t* text, const uint8_t size){
	for(uint8_t i = 0; i < size; i++)
		writeData(text[i]);
}

void LDC::writeU8(uint8_t value, const uint8_t precision){
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

void LDC::writeInt8(int8_t value, const uint8_t precision){
	if(value < 0){
		writeData('-');
		writeU8(uint8_t(-value), precision);
	}
	else{
		writeData(' ');
		writeU8(uint8_t(value), precision);
	}
}

void LDC::writeDouble(double value, const uint8_t precision, const uint8_t partPrecision){ // working but not tested by UT. Before change please write UT.
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

void LDC::writeU16(uint16_t value, const uint8_t precision){
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

void LDC::writeHEX(const uint32_t value){
	uint8_t signs[8];
	for(uint8_t i = 0; i < 8; i++)
		signs[7 - i] = hexToChar[(value >> (4 * i)) & 0xf];
	write(signs, 8);
}

void LDC::writeHEX(const uint16_t value){
	uint8_t signs[4];
	for(uint8_t i = 0; i < 4; i++)
		signs[3 - i] = hexToChar[(value >> (4 * i)) & 0xf];
	write(signs, 4);
}

void LDC::writeHEX(const uint8_t value){
	uint8_t signs[2];
	for(uint8_t i = 0; i < 2; i++)
		signs[1 - i] = hexToChar[(value >> (4 * i)) & 0xf];
	write(signs, 2);
}

void LDC::setReg(uint8_t value){
	dataPort &= 0xFFFFFF00;
	dataPort |= (uint32_t)value;
}
