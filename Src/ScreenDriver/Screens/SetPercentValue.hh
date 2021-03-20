#pragma once

#include "ScreenCellIfc.hh"
#include "MeasurementsPID.hh"
#include "TreeCell.hh"
#include <RadioParser.hh>
#include <AT24C.hh>
#include <memoryMap.hh>
#include <SelectMode.hh>

/*
Interface of radio data:
	Main option:
	30  = Set percent value of double. Main value = 0 => 0% | Main value = 255 => 100%
*/

class SetPercentVal : public ScreenCellIfc{
	static constexpr char name[]{" Set % value"};
	Drivers::RadioParser* radio;
	uint8_t value;
public:
    SetPercentVal(LCD_ifc* const _lcd, Drivers::RadioParser* _radio);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void execute() final;
	void increment() final;
	void decrement() final;
	void change() final{}
	void refresh() final{}
	void localRefresh();
};
