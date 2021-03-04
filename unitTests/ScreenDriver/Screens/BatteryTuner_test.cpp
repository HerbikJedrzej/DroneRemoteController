#include <gtest/gtest.h>
#include <BatteryTuner.hh>
#include "LCDforScreenTesting.hh"

TEST(Screen_BatteryTuner_test, ControledDeviceBatterySaveValue){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    ControledDeviceBatterySaveValue screen((LCD_ifc*)&lcd, &radio);
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    screen.print();
    lcd.compare(
        " Connect remote ",
        "device to xx.x V",
        "  and press OK  ",
        __LINE__);
	screen.refresh();
    lcd.compare(
        " Connect remote ",
        "device to  3.3 V",
        "  and press OK  ",
        __LINE__);
	screen.change();
	screen.refresh();
    lcd.compare(
        " Connect remote ",
        "device to  3.7 V",
        "  and press OK  ",
        __LINE__);
	screen.increment();
    screen.execute();
	screen.refresh();
    lcd.compare(
        " Connect remote ",
        "device to  3.8 V",
        "  and press OK *",
        __LINE__);
	screen.change();
	screen.decrement();
	screen.refresh();
    lcd.compare(
        " Connect remote ",
        "device to  3.2 V",
        "  and press OK  ",
        __LINE__);
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    radio.run();
    for(uint i = 0; i < 9; i++){
        EXPECT_EQ(radio.dataOut[5], 27);
        EXPECT_EQ(radio.dataOut[6], 38);
        radio.run();
    }
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
}
