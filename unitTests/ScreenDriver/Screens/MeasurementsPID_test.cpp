#include <gtest/gtest.h>
#include <MeasurementsPID.hh>
#include "LCDforScreenTesting.hh"
#include <memoryMap.hh>
// LDC* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory

TEST(Screen_SendOrCollectRecords_test, print_slot2){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    unsigned int localTime = 1520;
    MeasurementsPID::SendOrCollectRecords screen((LCD_ifc*)&lcd, &radio, 2, localTime, true);
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    screen.print();
    for(uint i = 0; i < 9; i++){
        radio.run();
        EXPECT_EQ(radio.dataOut[5], 3);
        EXPECT_EQ(radio.dataOut[6], 2);
    }
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    lcd.compare(
        " Sending data 2 ", 
        "        0%      ", 
        "|              |");
    localTime += 100;
    screen.refresh();
    lcd.compare(
        " Sending data 2 ", 
        "       20%      ", 
        "|##            |");
    localTime += 150;
    screen.refresh();
    lcd.compare(
        " Sending data 2 ", 
        "       50%      ", 
        "|#######       |");
    localTime += 250;
    screen.refresh();
    lcd.compare(
        " Sending data 2 ", 
        "      100%      ", 
        "|##############|");
    localTime += 250;
    screen.refresh();
    lcd.compare(
        " Sending data 2 ", 
        "      100%      ", 
        "|##############|");
}

TEST(Screen_SendOrCollectRecords_test, print_slot4){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    unsigned int localTime = 1520;
    MeasurementsPID::SendOrCollectRecords screen((LCD_ifc*)&lcd, &radio, 4, localTime, false);
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    screen.print();
    for(uint i = 0; i < 9; i++){
        radio.run();
        EXPECT_EQ(radio.dataOut[5], 2);
        EXPECT_EQ(radio.dataOut[6], 4);
    }
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    lcd.compare(
        " Record data  4 ", 
        "        0%      ", 
        "|              |");
    localTime += 100;
    screen.refresh();
    lcd.compare(
        " Record data  4 ", 
        "       10%      ", 
        "|#             |");
    localTime += 150;
    screen.refresh();
    lcd.compare(
        " Record data  4 ", 
        "       25%      ", 
        "|###           |");
}

TEST(Screen_SendOrCollectRecords_test, print_record_slot4){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    unsigned int localTime = 0;
    MeasurementsPID::SendOrCollectRecords sender((LCD_ifc*)&lcd, &radio, 4, localTime, true);
    MeasurementsPID::SendOrCollectRecords getter((LCD_ifc*)&lcd, &radio, 4, localTime, false);
    sender.print();
    lcd.compare(
        " Sending data 4 ",
        "        0%      ",
        "|              |");
    getter.print();
    lcd.compare(
        " Record data  4 ",
        "        0%      ",
        "|              |");
}

TEST(Screen_SetParameters_test, allTest){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    memory.dataMap[memoryMap::slot_5_sinusAmplitude] = 0;
    memory.dataMap[memoryMap::slot_5_sinusFrequency] = 0;
    memory.dataMap[memoryMap::slot_5_sinusAmplitude + 2] = 0;
    memory.dataMap[memoryMap::slot_5_sinusFrequency + 2] = 0;
    memory.dataMap[memoryMap::slot_1_sinusAmplitude] = 4;
    memory.dataMap[memoryMap::slot_1_sinusFrequency] = 7;
    MeasurementsPID::SetParameters screen((LCD_ifc*)&lcd, &radio, &memory, 5);
    EXPECT_NO_THROW(MeasurementsPID::SetParameters((LCD_ifc*)&lcd, &radio, &memory, 1));
    EXPECT_ANY_THROW(MeasurementsPID::SetParameters((LCD_ifc*)&lcd, &radio, &memory, 0));
    EXPECT_ANY_THROW(MeasurementsPID::SetParameters((LCD_ifc*)&lcd, &radio, &memory, 3));
    EXPECT_ANY_THROW(MeasurementsPID::SetParameters((LCD_ifc*)&lcd, &radio, &memory, 6));
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    screen.print();
    lcd.compare(
        "Set params of 5 ",
        "->A: xx.xx [%]  ",
        "  f: xx.xx [Hz] ");
    screen.refresh();
    lcd.compare(
        "Set params of 5 ",
        "->A:  0.25 [%]  ",
        "  f:  0.10 [Hz] ");
    screen.change();
    lcd.compare(
        "Set params of 5 ",
        "  A:  0.25 [%]  ",
        "->f:  0.10 [Hz] ");
    for(uint8_t i = 0; i < 101; i++)
        screen.increment();
    screen.refresh();
    lcd.compare(
        "Set params of 5 ",
        "  A:  0.25 [%]  ",
        "->f:  4.02 [Hz] ");
    radio.run();
    EXPECT_EQ(memory.dataMap[memoryMap::slot_5_sinusFrequency], 0);
    screen.execute();
    EXPECT_EQ(memory.dataMap[memoryMap::slot_5_sinusFrequency], 101);
    lcd.compare(
        "Set params of 5 ",
        "  A:  0.25 [%]  ",
        "->f:  4.02 [Hz]*");
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    for(uint i = 0; i < 9; i++){
        radio.run();
        EXPECT_EQ(radio.dataOut[5], 13);
        EXPECT_EQ(radio.dataOut[6], 101);
    }
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    screen.change();
    for(uint8_t i = 0; i < 100; i++)
        screen.decrement();
    screen.refresh();
    lcd.compare(
        "Set params of 5 ",
        "->A: 19.75 [%]  ",
        "  f:  4.02 [Hz] ");
    radio.run();
    EXPECT_EQ(memory.dataMap[memoryMap::slot_5_sinusAmplitude], 0);
    screen.execute();
    EXPECT_EQ(memory.dataMap[memoryMap::slot_5_sinusAmplitude], 156);
    lcd.compare(
        "Set params of 5 ",
        "->A: 19.75 [%] *",
        "  f:  4.02 [Hz] ");
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    for(uint i = 0; i < 9; i++){
        radio.run();
        EXPECT_EQ(radio.dataOut[5], 12);
        EXPECT_EQ(radio.dataOut[6], 156);
    }
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
}
