#include <gtest/gtest.h>
#include <SetAngleOffset.hh>
#include "LCDforScreenTesting.hh"
#include <memoryMap.hh>

TEST(Screen_SetAngleOffset_test, CheckAsises){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    memory.dataMap[memoryMap::angleOffsetX] = 196;
    memory.dataMap[memoryMap::angleOffsetY] = 128;
    SetAngleOffset screen((LCD_ifc*)&lcd, &radio, &memory);
    screen.print();
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetX], 196);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetY], 128);
    lcd.compare(
        "  Angle offset  ",
        "->X: xx.xx deg  ",
        "  Y: xx.xx deg  ",
        __LINE__);
    screen.refresh();
    lcd.compare(
        "  Angle offset  ",
        "->X:  8.05 deg  ",
        "  Y:  0.05 deg  ",
        __LINE__);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetX], 196);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetY], 128);
    screen.increment();
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetX], 196);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetY], 128);
    screen.refresh();
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetX], 196);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetY], 128);
    lcd.compare(
        "  Angle offset  ",
        "->X:  8.17 deg  ",
        "  Y:  0.05 deg  ",
        __LINE__);
    screen.change();
    screen.decrement();
    screen.refresh();
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetX], 196);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetY], 128);
    lcd.compare(
        "  Angle offset  ",
        "  X:  8.17 deg  ",
        "->Y:- 0.05 deg  ",
        __LINE__);
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetX], 196);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetY], 128);
    screen.execute();
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetX], 196);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetY], 127);
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    for(uint i = 0; i < 9; i++){
        radio.run();
        EXPECT_EQ(radio.dataOut[5], 29);
        EXPECT_EQ(radio.dataOut[6], 127);
    }
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    lcd.compare(
        "  Angle offset  ",
        "  X:  8.17 deg  ",
        "->Y:- 0.05 deg<-",
        __LINE__);
    screen.change();
    screen.refresh();
    lcd.compare(
        "  Angle offset  ",
        "->X:  8.17 deg  ",
        "  Y:- 0.05 deg  ",
        __LINE__);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetX], 196);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetY], 127);
    screen.execute();
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetX], 197);
    EXPECT_EQ(memory.dataMap[memoryMap::angleOffsetY], 127);
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    for(uint i = 0; i < 9; i++){
        radio.run();
        EXPECT_EQ(radio.dataOut[5], 28);
        EXPECT_EQ(radio.dataOut[6], 197);
    }
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    screen.refresh();
    lcd.compare(
        "  Angle offset  ",
        "->X:  8.17 deg<-",
        "  Y:- 0.05 deg  ",
        __LINE__);
    screen.print();
    screen.refresh();
    lcd.compare(
        "  Angle offset  ",
        "->X:  8.17 deg  ",
        "  Y:- 0.05 deg  ",
        __LINE__);
}
