#include <gtest/gtest.h>
#include <SelectTrybe.hh>
#include "LCDforScreenTesting.hh"
#include <memoryMap.hh>
// LDC* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory

TEST(Screen_SelectTrybe_test, print){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    memory.dataMap[memoryMap::trybeOfMainloop] = 1;
    SelectTrybe screen((LCD_ifc*)&lcd, &radio, &memory);
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
    EXPECT_EQ(memory.dataMap[memoryMap::trybeOfMainloop], 1);
    screen.refresh();
    screen.increment();
    lcd.compare(
        "  tune axis X <-", 
        "->tune axis Y   ", 
        "   PID only X   ",
        __LINE__);
    EXPECT_EQ(memory.dataMap[memoryMap::trybeOfMainloop], 1);
    screen.refresh();
    screen.decrement();
    screen.execute();
    lcd.compare(
        "  tune axis Y   ", 
        "-> PID only X   ", 
        "   PID only Y   ",
        __LINE__);
    EXPECT_EQ(memory.dataMap[memoryMap::trybeOfMainloop], 2);
    screen.decrement();
    screen.refresh();
    lcd.compare(
        "  default fly   ", 
        "->tune axis X   ", 
        "  tune axis Y <-",
        __LINE__);
    EXPECT_EQ(memory.dataMap[memoryMap::trybeOfMainloop], 2);
}

TEST(Screen_SelectTrybe_test, getTrybe){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    memory.dataMap[memoryMap::trybeOfMainloop] = 0;
    SelectTrybe screen1((LCD_ifc*)&lcd, &radio, &memory);
    memory.dataMap[memoryMap::trybeOfMainloop] = 1;
    SelectTrybe screen2((LCD_ifc*)&lcd, &radio, &memory);
    memory.dataMap[memoryMap::trybeOfMainloop] = 2;
    SelectTrybe screen3((LCD_ifc*)&lcd, &radio, &memory);
    EXPECT_EQ(screen1.getTrybe(), 0);
    EXPECT_EQ(screen2.getTrybe(), 1);
    EXPECT_EQ(screen3.getTrybe(), 2);
    screen1.decrement();
    screen2.decrement();
    screen3.decrement();
    screen1.execute();
    screen2.execute();
    screen3.execute();
    EXPECT_EQ(screen1.getTrybe(), SelectTrybe::trybesSize - 1);
    EXPECT_EQ(screen2.getTrybe(), 0);
    EXPECT_EQ(screen3.getTrybe(), 1);
    screen1.decrement();
    screen2.decrement();
    screen3.decrement();
    screen1.execute();
    screen2.execute();
    screen3.execute();
    EXPECT_EQ(screen1.getTrybe(), SelectTrybe::trybesSize - 2);
    EXPECT_EQ(screen2.getTrybe(), SelectTrybe::trybesSize - 1);
    EXPECT_EQ(screen3.getTrybe(), 0);
    EXPECT_EQ(memory.dataMap[memoryMap::trybeOfMainloop], 0);
}
