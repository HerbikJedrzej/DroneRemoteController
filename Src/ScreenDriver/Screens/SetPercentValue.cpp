#include "SetPercentValue.hh"
#include "Directory.hh"
#include <OwnExceptions.hh>

constexpr char SetPercentVal::name[];

SetPercentVal::SetPercentVal(LCD_ifc* const _lcd, Drivers::RadioParser* _radio):
    ScreenCellIfc(_lcd),
    radio(_radio),
    value(0){}

const uint8_t* SetPercentVal::getName() const{
    return (uint8_t*)name;
}

uint8_t SetPercentVal::getNameSize() const{
    return sizeof(name) / sizeof(char) - 1;
}

void SetPercentVal::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)"  Set % value   ", 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"    xxx.xx%     ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"|              |", 16);
    localRefresh();
}

void SetPercentVal::execute(){
    radio->setMainOption(30, int8_t(value));
    (*lcd)(2, 0);lcd->writeData('*');
    (*lcd)(2, 15);lcd->writeData('*');
}

void SetPercentVal::localRefresh(){
    double percent = 100.00 * double(value) / 255.0;
    (*lcd)(2, 0);lcd->writeData(' ');
    (*lcd)(2, 15);lcd->writeData(' ');
    (*lcd)(2, 3);lcd->writeDouble(percent, 3, 2);
    (*lcd)(3, 1);
    uint8_t i = 0;
	for(; i < value / 18; i++)
        lcd->writeData('#');
	for(; i < 14; i++)
        lcd->writeData(' ');
}

void SetPercentVal::increment(){
    value++;
    localRefresh();
}

void SetPercentVal::decrement(){
    value--;
    localRefresh();
}
