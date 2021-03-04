#pragma once

#include <cstdlib>
#include <GPIOconf.hh>
#include <LDC.hh>
#include <AT24C.hh>
#include <I2C.hh>
#include <RadioParser.hh>
#include <string>
#include <map>

class ScreenLCD : public LDC{
    std::string screen[4] = {"                ", "                ", "                ", "                "};
    uint32_t port = {0};
    uint8_t possitionX;
    uint8_t possitionY;
    class TryOverrideSpecialScreenBar{};
    bool line0Protection;
public:
    ScreenLCD(const bool& _line0Protection = true);
    ~ScreenLCD();
    void compare(const char* line1, const char* line2, const char* line3, const unsigned int line = 0);
	void operator()(const uint8_t, const uint8_t) final;
	void writeData(const uint8_t value) final;
};

class ScreenMemory : public Drivers::Memory{
public:
    std::map<uint16_t, uint8_t> dataMap;
    ScreenMemory();
    ~ScreenMemory();
    bool write(uint16_t addr, uint8_t* data, uint16_t dataSize) final;
	bool read(uint16_t addr, uint8_t* data, uint16_t dataSize) final;
	void writeDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish) final;
	void writeDMAwithoutDataAlocate(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish) final;
	bool readDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish) final;
	bool init(const RegPair* = nullptr, uint16_t = 0) final{return true;}
	void lockMemory() final{}
	void unlockMemory() final{}

};

class ScreenRadioParser : public Drivers::RadioParser{
public:
    uint8_t* dataIn;
    uint8_t* dataOut;
    ScreenRadioParser();
    ~ScreenRadioParser();
};
