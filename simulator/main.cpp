#include<iostream>
#include "RadioVisualisation.h"
#include <thread>
#include <chrono>
#include "KeyboardCharProvider.h"
#include "KeyboardInterpreter.h"
#include "Drivers/LDC_simulator.h"
#include "Drivers/Radio_simulator.hh"
#include "Drivers/Memory_simulator.hh"
#include <RadioParser.hh>
#include <BatteryObserver.hh>
#include "GPIOconf.hh"

#include <Visualizer.hh>
#include <BatteryTuner.hh>
#include <PrintRadioData.hh>
#include <SelectMode.hh>
#include <SetPIDparams.hh>
#include <SetAngleOffset.hh>
#include <SetPercentValue.hh>
#include <MeasurementsAltitude.hh>

using namespace std::chrono_literals;

namespace Drivers{

int readedChar = 0;
uint32_t timerCounts = 0;

void timerMainLoop(bool* exit, bool* mainLoop){
  KeyGetter key;
  while(!(*exit)){
    readedChar = key.getChar();
    if(readedChar == ']')
      *exit = true;
    std::this_thread::sleep_for(20ms);
    *mainLoop = true;
    timerCounts++;
  }
}

}

void HAL_Delay(uint32_t){}

int main(){
  bool exit = false;
  bool doCircle = true;
  bool altitudeMeasurementsTrigger = false;
  std::vector<bool> leds = {false, false, true, true, true, true, true, true, true, true};
  uint16_t batteryValue = 1000;
  uint8_t coordinates[4] = {0, 0, 0, 0};
  std::thread timer(Drivers::timerMainLoop, &exit, &doCircle);
  RadioVisualisation radioObject;
  LDCsimulator lcd;
  Drivers::Memory_Simulator memory("RemoteMemoryRegisters.json");
  std::vector<uint8_t> radioData = {0xde, 255, 12, 51, 23, 0x34, 12, 0x55};
  Drivers::RadioSimulator radio(0x46, Drivers::timerCounts, radioData, "/tmp/radioStream");
  BatteryObserver battery(&memory, batteryValue, Drivers::timerCounts, HAL_Delay);
  Drivers::RadioParser radioParser(&radio, Drivers::RadioMode::RemoteControl);
  TreeCell<ScreenCellIfc>* root = new TreeCell<ScreenCellIfc>(nullptr, nullptr);
  {
    root->add(CreateMeasurementsAltitudeScreenDirectory(root, (LCD_ifc*)&lcd, &radioParser, altitudeMeasurementsTrigger, Drivers::timerCounts));
    root->add(CreateBatteryScreenDirectory(root, &lcd, &battery, &radioParser));
    root->add(CreateSetPIDsParams(root, &lcd, &radioParser, &memory));
    root->add(new SelectMode(&lcd, &radioParser, &memory));
    root->add(CreateMeasurementsPIDScreenDirectory(root, &lcd, &radioParser, &memory, Drivers::timerCounts));
    root->add(new PrintRadioData(&lcd, &radio, &radioParser));
    root->add(new SetAngleOffset(&lcd, &radioParser, &memory));
    root->add(new SetPercentVal(&lcd, &radioParser));
  }
  Visualizer visual(&lcd, root, [](uint32_t)->void{});
  KeyboardInterpreter interpreter(visual, &radioParser, coordinates, altitudeMeasurementsTrigger);
  srand( time( NULL ) );
	const Drivers::Memory::RegPair initMemoryRegisters[] = {
    {memoryMap::settings, 12},
	};
  if(!memory.init(initMemoryRegisters, sizeof(initMemoryRegisters) / sizeof(initMemoryRegisters[0])))
    throw std::invalid_argument("Memory initialisation error");
  visual.init();
  battery.init();

  while(!exit){
	  if(doCircle){
		  doCircle = false;
      if(Drivers::timerCounts)
        leds[OutputList::LED_Green] = !leds[OutputList::LED_Green];
      leds[OutputList::LED_Yellow1] = !radio.isAckCorrect();
      leds[OutputList::LED_Yellow2] = !radio.isKyeCorrect();
      leds[OutputList::LED_Yellow3] = !radio.isComunicationCorrect();
      radio.handleTimeEvent(nullptr);
      radioParser.run();
      interpreter.run(Drivers::readedChar);
      radioParser.setRollValue(int8_t(coordinates[0]));
      radioParser.setPitchValue(int8_t(coordinates[1]));
      radioParser.setYawlValue(int8_t(coordinates[2]));
      radioParser.setAltitudeIncremetionValue(int8_t(coordinates[3]));
      radioObject.printCycleNumber(Drivers::timerCounts);
      radioObject.printRadioTransmision(radioData);
      radioObject.printCoordinates(coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
      radioObject.printLeds(leds);
      radioObject.printScreenLCD(lcd.getScreenContent());
      visual.run(Visualizer::StateOfOperation::waiting, radio.isAckCorrect(), radio.isKyeCorrect(), battery.batteryPercentVal(), radio.getRx(1));
    }
  }
  memory.saveFile("RemoteMemoryRegisters.json");
  timer.join();
  return 0;
}

