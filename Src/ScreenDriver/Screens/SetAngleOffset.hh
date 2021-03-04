#pragma once

#include "ScreenCellIfc.hh"
#include "TreeCell.hh"
#include <RadioParser.hh>
#include <AT24C.hh>
#include <memoryMap.hh>

/*
Interface of radio data:
	Main option:
	28 = Value of angle offset axis X.
	29 = Value of angle offset axis Y.
*/

class SetAngleOffset : public ScreenCellIfc{
public:
    SetAngleOffset(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final;
	void increment() final;
	void decrement() final;
private:
	bool axisX;
	static constexpr char name[]{"Angle offsets"};
	Drivers::RadioParser* radio;
	Drivers::Memory* memory;
	uint8_t param[2];
	uint8_t realParam[2];
};
