#include <gtest/gtest.h>
#include <cstdlib>
#include "MemoryTestHelper.hh"

// using Drivers::memoryMap;

// MemoryHelper::MemoryHelper():
//     Memory(drivers_test::i2c, drivers_test::gpio, 0xA0, OutputList::MemoryWriteProtect, drivers_test::HAL_Delay),
// 	lock(true),
// 	autoMode(false),
// 	initialised(true){}

// MemoryHelper::~MemoryHelper(){}

// void MemoryHelper::initFailure(){
// 	initialised = false;
// }

// void MemoryHelper::autoModeOn(){
// 	autoMode = true;
// }

// bool MemoryHelper::mainCheck(const bool& empty, const dataCell& cell, const uint16_t& addr, const uint16_t& dataSize){
// 	if(empty){
// 		EXPECT_FALSE(empty) << "Failure reason: Queue is empty." << std::endl;
// 		return false;
// 	}
// 	if(cell.data.size() != dataSize){
// 		EXPECT_FALSE(cell.data.size() != dataSize) << "Failure reason: Not equal size, test data = " << cell.data.size() << ", readed data = " << dataSize << std::endl;
// 		return false;
// 	}
// 	if(cell.addr != addr){
// 		EXPECT_FALSE(cell.addr != addr) << "Failure reason: Not equal address, test addr = " << cell.addr << ", readed addr = " << addr << std::endl;
// 		return false;
// 	}
// 	return true;
// }

// bool MemoryHelper::read(uint16_t addr, uint8_t* data, uint16_t dataSize){
// 	if(!mainCheck(recived.empty(), recived.front(), addr, dataSize)){
// 		for(unsigned int i = 0; i < dataSize; i++)
// 			data[i] = 0;
// 		return false;
// 	}
// 	auto& elem = recived.front();
// 	for(unsigned int i = 0; i < dataSize; i++)
// 		data[i] = elem.data[i];
// 	recived.pop();
// 	return true;
// }

// bool MemoryHelper::readDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish){
// 	EXPECT_TRUE(finish);
// 	*finish = read(addr, data, dataSize);
// 	return *finish;
// }

// bool MemoryHelper::write(uint16_t addr, uint8_t* data, uint16_t dataSize){
// 	if(lock){
// 		EXPECT_FALSE(lock);
// 		return false;
// 	}
// 	if(!mainCheck(sended.empty(), sended.front(), addr, dataSize))
// 		return false;
// 	auto& testCell = sended.front();
// 	for(unsigned int i = 0; i < dataSize; i++)
// 		EXPECT_EQ(data[i], testCell.data[i]) << "Failure for addr = " << addr << ", data nr = " << i << ", data size = " << dataSize << ". Expected = " << (uint)data[i] << ", acctual = " << (uint)testCell.data[i] << "." << std::endl;
// 	sended.pop();
// 	return true;
// }

// void MemoryHelper::writeDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish){
// 	EXPECT_TRUE(finish);
// 	*finish = write(addr, data, dataSize);
// }

// void MemoryHelper::writeDMAwithoutDataAlocate(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish){
// 	writeDMA(addr, data, dataSize, finish);
// }

// void MemoryHelper::empty(bool check){
// 	if(check){
// 		EXPECT_TRUE(sended.empty());
// 		EXPECT_TRUE(recived.empty());
// 	}
// 	while(!sended.empty()) sended.pop();
// 	while(!recived.empty()) recived.pop();
// }

// bool MemoryHelper::init(){
// 	uint8_t readedData[3];
// 	uint16_t minBat = MIN_BATTERY;
// 	uint16_t maxBat = MAX_BATTERY;
// 	uint32_t rezistor1 = REZISTOR1;
// 	uint32_t rezistor2 = REZISTOR2;
// 	uint8_t autoTune = autoMode? (0b10000000 | ADC_MASK) : ADC_MASK;
// 	empty(false);

// 	readedData[0] = (minBat >> 8) & 0xff;
// 	readedData[1] = minBat & 0xff;
// 	addReadTest(memoryMap::minOfBatteryMSB, {readedData[0], readedData[1]});
// 	readedData[0] = (maxBat >> 8) & 0xff;
// 	readedData[1] = maxBat & 0xff;
// 	addReadTest(memoryMap::maxOfBatteryMSB, {readedData[0], readedData[1]});
// 	readedData[0] = (rezistor1 >> 16) & 0xff;
// 	readedData[1] = (rezistor1 >> 8) & 0xff;
// 	readedData[2] = rezistor1 & 0xff;
// 	addReadTest(memoryMap::rezistor1h, {readedData[0], readedData[1], readedData[2]});
// 	readedData[0] = (rezistor2 >> 16) & 0xff;
// 	readedData[1] = (rezistor2 >> 8) & 0xff;
// 	readedData[2] = rezistor2 & 0xff;
// 	addReadTest(memoryMap::rezistor2h, {readedData[0], readedData[1], readedData[2]});
// 	addReadTest(memoryMap::settings, {autoTune});

// 	return initialised;
// }

// void MemoryHelper::lockMemory(){
// 	lock = true;
// }

// const bool MemoryHelper::getLock(){
// 	return lock;
// }

// void MemoryHelper::unlockMemory(){
// 	lock = false;
// }

// void MemoryHelper::addReadTest(uint16_t addr, std::vector<uint8_t> data){
// 	recived.push(dataCell{addr, {}});
// 	auto& vec = recived.back().data;
// 	for(unsigned int i = 0; i < data.size(); i++)
// 		vec.push_back(data[i]);
// }

// void MemoryHelper::addWriteTest(uint16_t addr, std::vector<uint8_t> data){
// 	sended.push(dataCell{addr, {}});
// 	auto& vec = sended.back().data;
// 	for(unsigned int i = 0; i < data.size(); i++)
// 		vec.push_back(data[i]);
// }
