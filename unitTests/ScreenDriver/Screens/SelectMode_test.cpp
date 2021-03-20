#include <gtest/gtest.h>
#include <SelectMode.hh>
#include "LCDforScreenTesting.hh"
#include <memoryMap.hh>
// LDC* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory

TEST(Screen_SelectMode_test, print){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    memory.dataMap[memoryMap::modeOfMainloop] = 1;
    SelectMode screen((LCD_ifc*)&lcd, &radio, &memory);
    screen.print();
    lcd.compare(
        "                ", 
        "->              ", 
        "                ",
        __LINE__);
    screen.refresh();
    screen.increment();
    lcd.compare(
        "  default fly   ", 
        "->tune axis X <-", 
        "  tune axis Y   ",
        __LINE__);
    EXPECT_EQ(memory.dataMap[memoryMap::modeOfMainloop], 1);
    screen.refresh();
    screen.increment();
    lcd.compare(
        "  tune axis X <-", 
        "->tune axis Y   ", 
        "   PID only X   ",
        __LINE__);
    EXPECT_EQ(memory.dataMap[memoryMap::modeOfMainloop], 1);
    screen.refresh();
    screen.decrement();
    screen.execute();
    lcd.compare(
        "  tune axis Y   ", 
        "-> PID only X   ", 
        "   PID only Y   ",
        __LINE__);
    EXPECT_EQ(memory.dataMap[memoryMap::modeOfMainloop], 2);
    screen.decrement();
    screen.refresh();
    lcd.compare(
        "  default fly   ", 
        "->tune axis X   ", 
        "  tune axis Y <-",
        __LINE__);
    EXPECT_EQ(memory.dataMap[memoryMap::modeOfMainloop], 2);
}

TEST(Screen_SelectMode_test, getMode){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    memory.dataMap[memoryMap::modeOfMainloop] = 0;
    SelectMode screen1((LCD_ifc*)&lcd, &radio, &memory);
    memory.dataMap[memoryMap::modeOfMainloop] = 1;
    SelectMode screen2((LCD_ifc*)&lcd, &radio, &memory);
    memory.dataMap[memoryMap::modeOfMainloop] = 2;
    SelectMode screen3((LCD_ifc*)&lcd, &radio, &memory);
    EXPECT_EQ(screen1.getMode(), 0);
    EXPECT_EQ(screen2.getMode(), 1);
    EXPECT_EQ(screen3.getMode(), 2);
    screen1.decrement();
    screen2.decrement();
    screen3.decrement();
    screen1.execute();
    screen2.execute();
    screen3.execute();
    EXPECT_EQ(screen1.getMode(), SelectMode::modesSize - 1);
    EXPECT_EQ(screen2.getMode(), 0);
    EXPECT_EQ(screen3.getMode(), 1);
    screen1.decrement();
    screen2.decrement();
    screen3.decrement();
    screen1.execute();
    screen2.execute();
    screen3.execute();
    EXPECT_EQ(screen1.getMode(), SelectMode::modesSize - 2);
    EXPECT_EQ(screen2.getMode(), SelectMode::modesSize - 1);
    EXPECT_EQ(screen3.getMode(), 0);
    EXPECT_EQ(memory.dataMap[memoryMap::modeOfMainloop], 0);
}
