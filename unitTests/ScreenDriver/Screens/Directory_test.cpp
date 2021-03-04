// #include <gtest/gtest.h>
// #include <Directory.hh>
// #include "LCDforScreenTesting.hh"

// TEST(Screen_Directory_test, dir_scenario1){
//     auto test = [](const std::string& name)->void{
//         ScreenLCD lcd;
//         Directory screen(name.c_str(), name.size(), (LCD_ifc*)&lcd);
//         screen.print();
//         ASSERT_EQ(screen.getNameSize(), name.size() + 1) << "Failure for \"" << name << "\"";
//         EXPECT_EQ(screen.getName()[0], '*') << "Failure for \"" << name << "\"";
//         for(unsigned int i = 0; i < name.size(); i++)
//             EXPECT_EQ(screen.getName()[i + 1], name[i]) << "Failure for \"" << name << "\" elem i: " << i;
//         lcd.compare("xxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxx");
//         screen.refresh();
//         lcd.compare("xxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxx");
//         screen.refresh();
//         lcd.compare("xxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxx");
//         for(uint8_t i = 0; i < 25; i++)
//             screen.refresh();
//         lcd.compare("xxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxx");
//     };
//     test("Test dir no. 1");
//     test("Test dir no. 2");
//     test("Next test no 1");
//     test("Test no. 1");
//     test("Test no. 25");
//     test("Next test no 2");
//     test("Next test no 3");
// }
