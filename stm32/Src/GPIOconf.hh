#pragma once

enum OutputList{
    none = -1,
    LED_Blue2 = 10,
    LED_Yellow1,
    LED_Yellow2,
    LED_Red1 = 15,
    RadioCSN,
    MemoryWriteProtect = 21,
    LED_Green = 26,
    LED_Blue1,
    RadioCE = 28,
    LED_Yellow3 = 42,
    LED_Red2  = 50,
    LDC_RS = 45,
    LDC_RW,
    LDC_E,    
};

enum InputList{
  JoyLeft = 5,
  OptStart,
  OptSpec,
  Encoder = 41,
  JoyRight = 43,
  SwitchRight = 44,
  OptStop = 17,
  OptFly = 18,
  SwitchLeft = 20
};

enum InterruptInputList{
  RadioIRQ =0,
  endOfInterruptInputList
};

enum ADClist{
	battery,
	axisLY,
	axisLX,
	axisRY,
	axisRX,
  end
};
