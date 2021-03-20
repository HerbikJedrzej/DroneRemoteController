#include <gtest/gtest.h>
#include "LCDforScreenTesting.hh"

namespace{
    void test_HAL_Delay(uint32_t){}

    void write(OutputList, bool){}
    bool read(OutputList){return false;}
    bool readInput(InputList){return false;}
    Drivers::GPIO test_gpio(write, read, readInput, test_HAL_Delay);
    Drivers::BusStatus writeDMAptr(uint8_t, uint16_t, uint16_t, uint8_t*, uint16_t){return Drivers::BusStatus::OK;}
    Drivers::BusStatus readDMAptr(uint8_t, uint16_t, uint16_t, uint8_t*, uint16_t){return Drivers::BusStatus::OK;}
    Drivers::BusStatus writePtr(uint8_t, uint16_t, uint16_t, uint8_t*, uint16_t, uint32_t){return Drivers::BusStatus::OK;}
    Drivers::BusStatus readPtr(uint8_t, uint16_t, uint16_t, uint8_t*, uint16_t, uint32_t){return Drivers::BusStatus::OK;}
    bool isBussyPtr(){return false;}
    const uint8_t _key = 0x4a;
    unsigned int _time = 0;
	class ScreenRadio : public Drivers::RadioIfc{
        uint8_t toReturn = {0};
    public:
        uint8_t dataIn[7];
        uint8_t dataOut[7];
        ScreenRadio():
            RadioIfc(_key, _time){}
        bool isKyeCorrect(){return true;}
        bool isAckCorrect(){return true;}
        bool isComunicationCorrect(){return true;}
        void init(){}
        uint8_t getTx(unsigned int i) const{return dataOut[i];}
        uint8_t getRx(unsigned int i) const{return dataIn[i];}
        uint8_t& setTx(unsigned int i){return dataOut[i];}
        uint8_t& setRx(unsigned int i){return dataIn[i];}
        void handleTimeEvent(Drivers::DriverIfc*){}
    };
    ScreenRadio test_radio;
    Drivers::I2C test_i2c(writeDMAptr, readDMAptr, writePtr, readPtr, isBussyPtr, test_HAL_Delay);
}

ScreenLCD::ScreenLCD(const bool& _line0Protection):
    LDC(test_gpio, OutputList::none, OutputList::none, OutputList::none, port, test_HAL_Delay),
    possitionX(0),
    possitionY(0),
    line0Protection(_line0Protection){
    for(uint8_t x = 0; x < 4; x++)
        for(uint8_t y = 0; y < 16; y++)
            screen[x][y] = 'x';

}

ScreenLCD::~ScreenLCD(){}

void ScreenLCD::operator()(const uint8_t x, const uint8_t y){
    if(x == 0 && line0Protection)
        throw TryOverrideSpecialScreenBar();
    possitionX = x;
    possitionY = y;
}

void ScreenLCD::compare(const char* line1, const char* line2, const char* line3, const unsigned int line){
    if(line == 0){
        EXPECT_STREQ(line1, screen[1].c_str());
        EXPECT_STREQ(line2, screen[2].c_str());
        EXPECT_STREQ(line3, screen[3].c_str());
    }else{
        EXPECT_STREQ(line1, screen[1].c_str()) << "Testine nr: " << line;
        EXPECT_STREQ(line2, screen[2].c_str()) << "Testine nr: " << line;
        EXPECT_STREQ(line3, screen[3].c_str()) << "Testine nr: " << line;
    }
}

void ScreenLCD::writeData(const uint8_t value){
    if(possitionY < 16)
        screen[possitionX][possitionY++] = value;
}

ScreenMemory::ScreenMemory():
    Drivers::Memory(&test_i2c, &test_gpio, 0xAA, OutputList::none, test_HAL_Delay),
    dataMap(){}

ScreenMemory::~ScreenMemory(){}

bool ScreenMemory::write(uint16_t addr, uint8_t* data, uint16_t dataSize){
    for(uint16_t i = 0; i < dataSize; i++){
        dataMap[addr + i] = data[i];
    }
    return true;
}

bool ScreenMemory::read(uint16_t addr, uint8_t* data, uint16_t dataSize){
    for(uint16_t i = 0; i < dataSize; i++){
        data[i] = dataMap.at(i + addr);
    }
    return true;
}

void ScreenMemory::writeDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish){
    write(addr, data, dataSize);
    if(finish)
        *finish = true;
}

void ScreenMemory::writeDMAwithoutDataAlocate(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish){
    writeDMA(addr, data, dataSize, finish);
}

bool ScreenMemory::readDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish){
    read(addr, data, dataSize);
    if(finish)
        *finish = true;
    return true;
}

ScreenRadioParser::ScreenRadioParser():
    Drivers::RadioParser(&test_radio, Drivers::RadioMode::RemoteControl),
    dataIn(test_radio.dataIn),
    dataOut(test_radio.dataOut){}

ScreenRadioParser::~ScreenRadioParser(){}

