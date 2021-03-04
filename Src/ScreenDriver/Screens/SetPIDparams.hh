#pragma once

#include "ScreenCellIfc.hh"
#include "TreeCell.hh"
#include <RadioParser.hh>
#include <AT24C.hh>
#include <memoryMap.hh>
#include <SelectTrybe.hh>

/*
Interface of radio data:
	Main option:
	14 = Common factor for axises X and Y.	
	15 = Value of msb P param of PID axis X.
	16 = Value of msb I param of PID axis X.
	17 = Value of msb D param of PID axis X.
	18 = Value of msb P param of PID axis Y.
	19 = Value of msb I param of PID axis Y.
	20 = Value of msb D param of PID axis Y.
	21 = Value of msb P param of PID axis z.
	22 = Value of msb I param of PID axis z.
	23 = Value of msb D param of PID axis z.
	24 = Value of msb P param of PID axis H.
	25 = Value of msb I param of PID axis H.
	26 = Value of msb D param of PID axis H.

*/

TreeCell<ScreenCellIfc>* CreateSetPIDsParams(TreeCell<ScreenCellIfc>* root, LCD_ifc* const lcd, Drivers::RadioParser* radio, Drivers::Memory* memory);

class SetPIDparams : public ScreenCellIfc{
public:
	enum Axis{
		axisX,
		axisY,
		axisZ,
		axisH,		
	};
    SetPIDparams(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory, const Axis _axis);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final;
	void increment() final;
	void decrement() final;
private:
	const Axis axis;
	const char axisName;
	const char name[10];
	Drivers::RadioParser* radio;
	Drivers::Memory* memory;
	memoryMap addr;
	uint8_t param[3];
	uint8_t realParam[3];
	uint8_t pointedParam = {0};
};

class SetPIDFactor : public ScreenCellIfc{
public:
	enum Axis{
		axisX,
		axisY,
		axisZ,
		axisH,		
	};
    SetPIDFactor(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final{}
	void increment() final;
	void decrement() final;
private:
	static constexpr char name[]{"Set PID factor"};
	Drivers::RadioParser* radio;
	Drivers::Memory* memory;
	uint8_t param;
	uint8_t realParam;
};