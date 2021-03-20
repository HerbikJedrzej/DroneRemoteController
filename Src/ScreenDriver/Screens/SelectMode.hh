#pragma once

#include "ScreenCellIfc.hh"
#include "TreeCell.hh"
#include <RadioParser.hh>
#include <AT24C.hh>

/*
Interface of radio data:
	Main option = 1 - selecting mode.
	Main value (converted to uint8):
		0 - casual fly mode.
		1 - tuning mode of axis X.
		2 - tuning mode of axis Y.
		3 - mode of working PID but only for X axis.
		4 - mode of working PID but only for Y axis.
		5 - Recognize engines.
		6 - Percent value of engine 1
		7 - Percent value of engine 2
		8 - Percent value of engine 3
		9 - Percent value of engine 4
		10 - Percent value of engine 5
		11 - Percent value of engine 6
		12 - Percent value of engine 7
		13 - Percent value of engine 8
		14 - fly mode with altitude measurement
*/

class SelectMode : public ScreenCellIfc{
public:
	enum modeID{
		casualFlyMode = 0,
		tuningAxisXMode,
		tuningAxisYMode,
		pidForAxisX,
		pidForAxisY,
		recognizeEngines
	};
    SelectMode(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final{}
	void increment() final;
	void decrement() final;
	uint8_t getMode();
	static constexpr unsigned int modesSize = {16};
protected:
	static constexpr char name[]{"Select mode"};
	static constexpr uint8_t modesNames[modesSize][13]{
		"default fly ",
		"tune axis X ",
		"tune axis Y ",
		" PID only X ",
		" PID only Y ",
		"check engine",
		" set % eng 1",
		" set % eng 2",
		" set % eng 3",
		" set % eng 4",
		" set % eng 5",
		" set % eng 6",
		" set % eng 7",
		" set % eng 8",
		" H measure  ",
		" fly auto H "};
	Drivers::RadioParser* radio;
	Drivers::Memory* memory;
	uint8_t mode;
	uint8_t pointedMode;
	bool changed;
};
