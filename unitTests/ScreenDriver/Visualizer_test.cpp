// #include <gtest/gtest.h>
// #include <Tree.hh>
// #include <cstdlib>
// #include <Visualizer.hh>
// #include <Directory.hh>
// #include <BatteryTuner.hh>
// #include <HelloWorld.hh>
// #include "../MemoryTestHelper.hh"
// #include "Screens/LCDforScreenTesting.hh"

// namespace VisualiserTest{

// TEST(Visualiser_test, Simle_with_screens_cells){
//     MemoryHelper memory;
// 	uint16_t measure;
// 	unsigned int timer = 0;
// 	BatteryObserver battery(&memory, measure, timer, drivers_test::HAL_Delay);
//     ScreenLCD lcd(false);
//     TreeCell<ScreenCellIfc>* root = new TreeCell<ScreenCellIfc>(nullptr, nullptr);
//     TreeCell<ScreenCellIfc>* dir1 = new TreeCell<ScreenCellIfc>(new Directory("Dir1", 4, &lcd), root);
//     TreeCell<ScreenCellIfc>* dir2 = new TreeCell<ScreenCellIfc>(new Directory("Dir2", 4, &lcd), root);
//     TreeCell<ScreenCellIfc>* dir3 = new TreeCell<ScreenCellIfc>(new Directory("Dir3", 4, &lcd), dir1);
//     TreeCell<ScreenCellIfc>* dir4 = new TreeCell<ScreenCellIfc>(new Directory("Dir4", 4, &lcd), dir3);
//     EXPECT_TRUE(root->add(dir2));
//     root->add(new HelloWorld(&lcd));
//     root->add(new BatteryValue(&lcd, &battery));
//     EXPECT_TRUE(root->add(dir1));
//     EXPECT_TRUE(dir1->add(dir3));
//     EXPECT_TRUE(dir3->add(dir4));
//     dir1->add(new HelloWorld(&lcd));
//     dir1->add(new BatteryValue(&lcd, &battery));
//     dir2->add(new HelloWorld(&lcd));
//     dir2->add(new BatteryValue(&lcd, &battery));
//     dir3->add(new HelloWorld(&lcd));
//     dir4->add(new HelloWorld(&lcd));
//     Visualizer visualizer(&lcd, root, drivers_test::HAL_Delay);
//     lcd.compare("xxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxx", "xxxxxxxxxxxxxxxx");
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare(" *Dir1          ", ">*Dir2          ", " Hello World    ");
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     visualizer.Up();
//     lcd.compare(" *Dir1          ", ">*Dir2          ", " Hello World    ");
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     visualizer.Up();
//     lcd.compare(" *Dir2          ", ">Hello World    ", " Battery value   ");
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare(" *Dir2          ", ">Hello World    ", " Battery value   ");
//     visualizer.Ok();
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare("  First screen  ", "  Hello World   ", "  Time: xxxxx   ");
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare("  First screen  ", "  Hello World   ", "  Time: 00000   ");
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare("  First screen  ", "  Hello World   ", "  Time: 00001   ");
//     for(uint8_t i = 0; i < 50; i++)
//         visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare("  First screen  ", "  Hello World   ", "  Time: 00051   ");
//     visualizer.Ok();
//     visualizer.Up();
//     visualizer.Down();
//     visualizer.Cancel();
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     for(uint8_t i = 0; i < 50; i++)
//         visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     visualizer.Up();
//     for(uint8_t i = 0; i < 50; i++)
//         visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     visualizer.Up();
//         visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare(" Battery value   ", ">*Dir1          ", " *Dir2          ");
//     visualizer.Ok();
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare(" Battery value   ", ">*Dir3          ", " Hello World    ");
//     visualizer.Ok();
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare(" Battery value   ", ">*Dir3          ", " Hello World    ");
//     for(uint8_t i = 0; i < 50; i++)
//         visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     visualizer.Ok();
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare("                ", ">*Dir4          ", " Hello World    ");
//     for(uint8_t i = 0; i < 50; i++)
//         visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     visualizer.Ok();
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare("                ", ">Hello World    ", "                ");
//     for(uint8_t i = 0; i < 50; i++)
//         visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     visualizer.Up();
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     visualizer.Down();
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     visualizer.Ok();
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare("  First screen  ", "  Hello World   ", "  Time: xxxxx   ");
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare("  First screen  ", "  Hello World   ", "  Time: 00000   ");
//     visualizer.Cancel();
//     visualizer.run(Visualizer::StateOfOperation::none, false, false, 0xff, 0xff);
//     lcd.compare("                ", ">Hello World    ", "                ");
// }

// }