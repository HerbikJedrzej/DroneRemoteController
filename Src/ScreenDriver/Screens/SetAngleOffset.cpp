#include "SetAngleOffset.hh"
#include "Directory.hh"
#include <OwnExceptions.hh>

constexpr char SetAngleOffset::name[];

SetAngleOffset::SetAngleOffset(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory):
    ScreenCellIfc(_lcd),
    axisX(true),
    radio(_radio),
    memory(_memory){
        memory->read(memoryMap::angleOffsetX, realParam, 2);
    }

const uint8_t* SetAngleOffset::getName() const{
    return (uint8_t*)name;
}

uint8_t SetAngleOffset::getNameSize() const{
    return 13;
}

void SetAngleOffset::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)"  Angle offset  ", 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"  X: xx.xx deg  ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"  Y: xx.xx deg  ", 16);
    if(axisX){
        (*lcd)(2, 0);lcd->write((uint8_t*)("->"), 2);
        (*lcd)(3, 0);lcd->write((uint8_t*)("  "), 2);
    }else{
        (*lcd)(2, 0);lcd->write((uint8_t*)("  "), 2);
        (*lcd)(3, 0);lcd->write((uint8_t*)("->"), 2);
    }
    param[0] = realParam[0];
    param[1] = realParam[1];
}

void SetAngleOffset::refresh(){
    (*lcd)(2, 4);lcd->writeDouble(double(param[0]) * 30.0 / 255 - 15.0, 2, 2); // [-15.0  -  15.0]
    (*lcd)(3, 4);lcd->writeDouble(double(param[1]) * 30.0 / 255 - 15.0, 2, 2); // [-15.0  -  15.0]
}

void SetAngleOffset::execute(){
    if(axisX){
        realParam[0] = param[0];
        memory->writeDMAwithoutDataAlocate(memoryMap::angleOffsetX, &realParam[0], 1, nullptr);
        radio->setMainOption(28, realParam[0]);
        (*lcd)(2, 14);lcd->write((uint8_t*)("<-"), 2);
    }else{
        realParam[1] = param[1];
        memory->writeDMAwithoutDataAlocate(memoryMap::angleOffsetY, &realParam[1], 1, nullptr);
        radio->setMainOption(29, realParam[1]);
        (*lcd)(3, 14);lcd->write((uint8_t*)("<-"), 2);
    }
}

void SetAngleOffset::increment(){
    param[(axisX)? 0 : 1]++;
}

void SetAngleOffset::decrement(){
    param[(axisX)? 0 : 1]--;
}

void SetAngleOffset::change(){
    (*lcd)((axisX)? 2 : 3, 0);lcd->write((uint8_t*)("  "), 2);
    (*lcd)((axisX)? 2 : 3, 14);lcd->write((uint8_t*)("  "), 2);
    axisX = !axisX;
    (*lcd)((axisX)? 2 : 3, 0);lcd->write((uint8_t*)("->"), 2);
}
