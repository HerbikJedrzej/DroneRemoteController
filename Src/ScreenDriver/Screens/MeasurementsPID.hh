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
	2  = Record data to slot of memory - main value means slot number. Main value = [1 ... 5]
	3  = Request to send data from slot of memory via uart - main value means slot number. Main value = [1 ... 5]
	4  = Set value of sinus amplitude in mesurement for slot nr 1. Represent as uint8. Amplitude [%]  = Main_value / 8 + 0.25
	5  = Set value of sinus frequency in mesurement for slot nr 1. Represent as uint8. Frequency [Hz] = Main_value * 9.9 / 255 + 0.1
	6  = Same as above for slot nr 2.
	7  = Same as above for slot nr 2.
	8  = Same as above for slot nr 3.
	9  = Same as above for slot nr 3.
	10 = Same as above for slot nr 4.
	11 = Same as above for slot nr 4.
	12 = Same as above for slot nr 5.
	13 = Same as above for slot nr 5.

*/

TreeCell<ScreenCellIfc>* CreateMeasurementsPIDScreenDirectory(TreeCell<ScreenCellIfc>* root, LCD_ifc* const lcd, Drivers::RadioParser* radio, Drivers::Memory* memory, unsigned int& time);

namespace MeasurementsPID{

class SendOrCollectRecords : public ScreenCellIfc{
	const uint8_t slotNumber;
	const char name[6];
	Drivers::RadioParser* radio;
	unsigned int& time;
	const bool modeSender; 
	SelectMode* trybe;
	const unsigned int duration;
	unsigned int endTime;
public:
    SendOrCollectRecords(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, const uint8_t _slotNumber, unsigned int& _time, const bool _modeSender);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final{}
	void change() final{}
	void increment() final{}
	void decrement() final{}
};

class SetParameters : public ScreenCellIfc{
	const uint8_t slotNumber;
	const char name[6];
	Drivers::RadioParser* radio;
	Drivers::Memory* memory;
	uint8_t amplitude;
	uint8_t frequency;
	uint8_t amplitudeSaved;
	uint8_t frequencySaved;
	memoryMap amplitudeAddr;
	memoryMap frequencyAddr;
	uint8_t* pointed;
public:
    SetParameters(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory, const uint8_t _slotNumber);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final;
	void increment() final;
	void decrement() final;
};

}