#include <gtest/gtest.h>
#include <SetPercentValue.hh>
#include "LCDforScreenTesting.hh"

TEST(Screen_SetPercentValue_test, mainTest){
    ScreenLCD lcd;
    ScreenRadioParser radio;
    SetPercentVal screen((LCD_ifc*)&lcd, &radio);
    screen.print();
    lcd.compare(
        "  Set % value   ",
        "      0.00%     ",
        "|              |",
        __LINE__);
    screen.decrement();
    lcd.compare(
        "  Set % value   ",
        "    100.00%     ",
        "|##############|",
        __LINE__);
    for(uint8_t i = 0; i < 50; i++)
        screen.decrement();
    lcd.compare(
        "  Set % value   ",
        "     80.39%     ",
        "|###########   |",
        __LINE__);
    screen.execute();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    for(uint i = 0; i < 9; i++){
        radio.run();
        EXPECT_EQ(radio.dataOut[5], 30);
        EXPECT_EQ(radio.dataOut[6], 205);
    }
    radio.run();
    EXPECT_EQ(radio.dataOut[5], 0);
    EXPECT_EQ(radio.dataOut[6], 0);
    lcd.compare(
        "  Set % value   ",
        "*    80.39%    *",
        "|###########   |",
        __LINE__);
    screen.increment();
    lcd.compare(
        "  Set % value   ",
        "     80.78%     ",
        "|###########   |",
        __LINE__);
}
