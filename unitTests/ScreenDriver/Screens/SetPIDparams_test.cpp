#include <gtest/gtest.h>
#include <SetPIDparams.hh>
#include "LCDforScreenTesting.hh"
#include <memoryMap.hh>

TEST(Screen_SetPIDsParameters_test, CheckAsises){
    ScreenLCD lcd;
    ScreenMemory memory;
    ScreenRadioParser radio;
    memory.dataMap[memoryMap::commonFactorForAxisXY] = 3;
    memory.dataMap[memoryMap::pid_P_XY_00] = 3;
    memory.dataMap[memoryMap::pid_I_XY_00] = 3;
    memory.dataMap[memoryMap::pid_D_XY_00] = 3;
    memory.dataMap[memoryMap::pid_P_XY_11] = 3;
    memory.dataMap[memoryMap::pid_I_XY_11] = 3;
    memory.dataMap[memoryMap::pid_D_XY_11] = 3;
    memory.dataMap[memoryMap::pid_P_Z] = 3;
    memory.dataMap[memoryMap::pid_I_Z] = 3;
    memory.dataMap[memoryMap::pid_D_Z] = 3;
    memory.dataMap[memoryMap::pid_P_H] = 3;
    memory.dataMap[memoryMap::pid_I_H] = 3;
    memory.dataMap[memoryMap::pid_D_H] = 3;
    SetPIDparams screenX((LCD_ifc*)&lcd, &radio, &memory, SetPIDparams::Axis::axisX);
    SetPIDparams screenY((LCD_ifc*)&lcd, &radio, &memory, SetPIDparams::Axis::axisY);
    SetPIDparams screenZ((LCD_ifc*)&lcd, &radio, &memory, SetPIDparams::Axis::axisZ);
    SetPIDparams screenH((LCD_ifc*)&lcd, &radio, &memory, SetPIDparams::Axis::axisH);
    screenX.print();
    lcd.compare(
        "  P_X: x.xxxxx  ",
        "  I_X: x.xxxxx  ",
        "  D_X: x.xxxxx  ",
        __LINE__);
    screenY.print();
    lcd.compare(
        "  P_Y: x.xxxxx  ",
        "  I_Y: x.xxxxx  ",
        "  D_Y: x.xxxxx  ",
        __LINE__);
    screenZ.print();
    lcd.compare(
        "  P_Z: x.xxxxx  ",
        "  I_Z: x.xxxxx  ",
        "  D_Z: x.xxxxx  ",
        __LINE__);
    screenH.print();
    lcd.compare(
        "  P_H: x.xxxxx  ",
        "  I_H: x.xxxxx  ",
        "  D_H: x.xxxxx  ",
        __LINE__);
}
