#pragma once

#include "ScreenCellIfc.hh"
#include "TreeCell.hh"
#include <RadioParser.hh>

class PrintRadioData : public ScreenCellIfc{
	static constexpr char name[]{"Printer"};
	Drivers::RadioIfc* radio;
	Drivers::RadioParser* parser;
public:
    PrintRadioData(LCD_ifc* const _lcd, Drivers::RadioIfc* _radio, Drivers::RadioParser* _parser);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final;
	void increment() final{}
	void decrement() final{}
};
