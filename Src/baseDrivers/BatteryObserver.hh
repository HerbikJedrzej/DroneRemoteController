#pragma once

#include <cstddef>
#include <cstdint>
#include <stdint.h>
#include <cstdlib>
#include <DriverIfc.hh>
#include <AT24C.hh>

class BatteryObserver : public Drivers::DriverIfc{
public:
	BatteryObserver(Drivers::Memory* _memory, uint16_t& adcBattery, const unsigned int& _time, void (*delay)(uint32_t));
	~BatteryObserver();
	bool init();
	uint8_t batteryPercentVal();
	uint16_t batteryRawVal();
	double batteryVoltageVal();
	void clearMemory();
	void run();
	bool IsAutoSaveMinMode();
	void AutoSaveMinModeOn();
	void AutoSaveMinModeOff();
	void correctResistor1whenPowerSupplayis12V();
	void correctResistor1whenPowerSupplayis9V();
	void saveMaxBat();
	void saveMinBat();
protected:
	void handleTimeEvent(DriverIfc*) override;
private:
	Drivers::Memory* const memory;
	const uint16_t& measure;
	bool initialised;
	uint16_t minBat;
	uint16_t maxBat;
	uint16_t adcMask;
	uint32_t rezistor1;
	const uint32_t rezistor2;
	uint8_t dataToWrite[2];
	bool writingDone;
	bool autoTune;
	const unsigned int& time;
};
