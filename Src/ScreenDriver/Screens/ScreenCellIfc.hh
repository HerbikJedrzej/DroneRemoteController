#pragma once

#include "LCD_ifc.hh"
#include <cstdint>
#include <stdint.h>
#include <cstdlib>

class ScreenCellIfc{
protected:
	LCD_ifc* const lcd;
public:
	ScreenCellIfc() = delete;
	ScreenCellIfc(ScreenCellIfc&) = delete;
	ScreenCellIfc(LCD_ifc* const _lcd):
		lcd(_lcd){}
	virtual ~ScreenCellIfc() = default;
	virtual const uint8_t* getName() const = 0;
	virtual uint8_t getNameSize() const = 0;
	virtual void print() = 0; // print content
	virtual void increment() = 0; // execute task
	virtual void decrement() = 0; // execute task
	virtual void execute() = 0; // execute task
	virtual void change() = 0; // option to chose fields
	virtual void refresh() = 0; // print content which could be changed
};