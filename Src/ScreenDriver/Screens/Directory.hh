#pragma once

#include "ScreenCellIfc.hh"

class Directory : public ScreenCellIfc{
	const uint8_t nameSize;
	uint8_t* const name;
public:
    Directory(const char* _name, const uint8_t _nameSize, LCD_ifc* const _lcd);
	~Directory();
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final;
	void increment() final{}
	void decrement() final{}
};