#pragma once

#include "ScreenCellIfc.hh"
#include "MeasurementsAltitude.hh"
#include "TreeCell.hh"
#include <RadioParser.hh>
#include <SelectMode.hh>

/*
Interface of radio data:
	Main option:
	31 = Record data to slot of memory - main value means slot number. Main value = [1 ... 5]
	3  = Request to send data from slot of memory via uart - main value means slot number. Main value = [1 ... 5]
*/

TreeCell<ScreenCellIfc>* CreateMeasurementsAltitudeScreenDirectory(TreeCell<ScreenCellIfc>* root, LCD_ifc* const lcd, Drivers::RadioParser* radio, bool& triggerButton, unsigned int& time);

namespace MeasurementsAltitude{

class SendOrCollectRecords : public ScreenCellIfc{
	uint8_t slotNumber;
	const char* name;
	Drivers::RadioParser* radio;
	unsigned int& time;
	const bool modeSender; 
	SelectMode* mode;
	bool& triggerButton;
	const unsigned int duration;
	unsigned int endTime;
	static bool waitForJoyButton;
public:
    SendOrCollectRecords(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, bool& _triggerButton, unsigned int& _time, const bool _modeSender);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final{}
	void change() final{}
	void increment() final;
	void decrement() final;
};

}