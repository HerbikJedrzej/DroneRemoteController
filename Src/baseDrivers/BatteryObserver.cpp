#include "BatteryObserver.hh"
#include <memoryMap.hh>

BatteryObserver::BatteryObserver(Drivers::Memory* _memory, uint16_t& adcBattery, const unsigned int& _time, void (*delay)(uint32_t)):
	DriverIfc(delay),
	memory(_memory),
	measure(adcBattery),
	initialised(false),
	minBat(0xffff),
	maxBat(0x0000),
	adcMask(0),
	rezistor1(47000),
	rezistor2(10000),
	writingDone(false),
	autoTune(false),
	time(_time){}

void BatteryObserver::saveMaxBat(){
	maxBat = measure;
	uint8_t data[2] = {
		(uint8_t)((maxBat >> 8) & 0xff),
		(uint8_t)(maxBat & 0xff)
	};
	data[0] = (maxBat >> 8) & 0xff;
	data[1] = maxBat & 0xff;
	memory->write(memoryMap::maxOfBatteryMSB, data, 2);
}

BatteryObserver::~BatteryObserver(){
	memory->stopWakingMe(this);
}

bool BatteryObserver::IsAutoSaveMinMode(){
	return autoTune;
}

void BatteryObserver::AutoSaveMinModeOn(){
	uint8_t readedData;
	if(autoTune)
		return;
	if(!memory->read(memoryMap::settings, &readedData, 1))
		return;
	readedData |= 0b10000000;
	autoTune = true;
	memory->write(memoryMap::settings, &readedData, 1);
}

void BatteryObserver::AutoSaveMinModeOff(){
	uint8_t readedData;
	if(!autoTune)
		return;
	if(!memory->read(memoryMap::settings, &readedData, 1))
		return;
	readedData &= 0b01111111;
	autoTune = false;
	memory->write(memoryMap::settings, &readedData, 1);
}


void BatteryObserver::saveMinBat(){
	minBat = measure;
	uint8_t data[2] = {
		(uint8_t)((minBat >> 8) & 0xff),
		(uint8_t)(minBat & 0xff)
	};
	minBat = measure;
	data[0] = (minBat >> 8) & 0xff;
	data[1] = minBat & 0xff;
	memory->write(memoryMap::minOfBatteryMSB, data, 2);
}

void BatteryObserver::handleTimeEvent(DriverIfc*){
	writingDone = true;
	run();
}

void BatteryObserver::run(){
	if(!autoTune)
		return;
	uint16_t batteryVal = measure;
	if(!writingDone)
		return;
	if(time % 100 != 0)
		return;
	if(minBat > batteryVal){
		minBat = batteryVal;
		if(batteryVal < 330){
			writingDone = true;
			return;
		}
		writingDone = false;
		dataToWrite[0] = (minBat >> 8) & 0xff;
		dataToWrite[1] = minBat & 0xff;
		memory->writeDMAwithoutDataAlocate(
			memoryMap::minOfBatteryMSB, dataToWrite, 2, &writingDone);
		return;
	}
}

void BatteryObserver::clearMemory(){
	uint8_t data[] = {0xff, 0xff, 0, 0, 12, 0x00, 0xB7, 0x98, 0x00, 0x27, 0x10};
	memory->write(memoryMap::minOfBatteryMSB, data, 11);
}

void BatteryObserver::correctResistor1whenPowerSupplayis12V(){
	double r2 = rezistor2;
	double adc = measure;
	double mask = adcMask;
	rezistor1 = (uint32_t)(r2 * (12.0 * mask / (adc * 3.3) - 1));
	uint8_t data[3] = {
		(uint8_t)((rezistor1 >> 16) & 0xff),
		(uint8_t)((rezistor1 >> 8) & 0xff),
		(uint8_t)(rezistor1 & 0xff)
	};
	memory->write(memoryMap::rezistor1h, data, 3);
}

void BatteryObserver::correctResistor1whenPowerSupplayis9V(){
	double r2 = rezistor2;
	double adc = measure;
	double mask = adcMask;
	rezistor1 = (uint32_t)(r2 * (9.0 * mask / (adc * 3.3) - 1));
	uint8_t data[3] = {
		(uint8_t)((rezistor1 >> 16) & 0xff),
		(uint8_t)((rezistor1 >> 8) & 0xff),
		(uint8_t)(rezistor1 & 0xff)
	};
	memory->write(memoryMap::rezistor1h, data, 3);
}

uint16_t BatteryObserver::batteryRawVal(){
	return measure; 
}

double BatteryObserver::batteryVoltageVal(){
	double r1 = rezistor1;
	double r2 = rezistor2;
	double adc = measure;
	double mask = adcMask;
	return
		(double)(
			adc * 3.3 * (r1 + r2) / (r2 * mask)
		);
}

uint8_t BatteryObserver::batteryPercentVal(){
	uint16_t val = measure;
	if(maxBat <= minBat)
		return 0xfd;
	if(val < minBat)
		return 0xfe;
	return (uint8_t)
		(
			((double)(val - minBat))
			/
			((double)(maxBat - minBat))
			* 100.0
		);
}

bool BatteryObserver::init(){
	if(!memory->init())
		return false;
	uint8_t readedData[3];
	if(!memory->read(memoryMap::minOfBatteryMSB, readedData, 2))
		return false;
	minBat = (readedData[0] << 8) | readedData[1];
	if(!memory->read(memoryMap::maxOfBatteryMSB, readedData, 2))
		return false;
	maxBat = (readedData[0] << 8) | readedData[1];
	if(!memory->read(memoryMap::rezistor1h, readedData, 3))
		return false;
	rezistor1 = (readedData[0] << 16) | (readedData[1] << 8) | readedData[2];
	if(!memory->read(memoryMap::settings, readedData, 1))
		return false;
	autoTune = ((readedData[0] & 0b10000000) != 0);
	uint8_t bits = readedData[0] & 0x1f;
	adcMask = 0;
	for(uint8_t i = 0; i < bits; i++)
		adcMask |= 1 << i;
	initialised = true;
	memory->wakeMeUp(this);
	return initialised;
}
