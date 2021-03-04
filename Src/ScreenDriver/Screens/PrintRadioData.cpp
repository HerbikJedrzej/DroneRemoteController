#include "PrintRadioData.hh"
#include "Directory.hh"
constexpr char PrintRadioData::name[];

PrintRadioData::PrintRadioData(LCD_ifc* const _lcd, Drivers::RadioIfc* _radio, Drivers::RadioParser* _parser):
    ScreenCellIfc(_lcd),
    radio(_radio),
    parser(_parser){}

const uint8_t* PrintRadioData::getName() const{
    return (uint8_t*)name;
}

uint8_t PrintRadioData::getNameSize() const{
    return sizeof(name) / sizeof(char) - 1;
}

void PrintRadioData::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)"                ", 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"                ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"                ", 16);
}

void PrintRadioData::refresh(){
    uint8_t bytes[] = {' ', ' ', ' ', ' ', ' '
                     , ' ', ' ', ' ', ' ', ' '
                     , ' ', ' ', ' ', ' ', ' '};
    uint8_t tmpVal = radio->getTx(0);
    for(int8_t i = 7; i >= 0; i--){
        bytes[i*2] = ((tmpVal & 1) == 0)? '0' : '1';
        tmpVal = tmpVal >> 1;
    }
    (*lcd)(1, 1);lcd->write(bytes, 15);
    (*lcd)(2, 0);lcd->writeInt8(radio->getTx(1), 3);
    (*lcd)(2, 5);lcd->writeInt8(radio->getTx(2), 3);
    (*lcd)(2, 10);lcd->writeInt8(radio->getTx(3), 3);
    (*lcd)(3, 0);lcd->writeInt8(radio->getTx(4), 3);
    (*lcd)(3, 6);lcd->writeU8(radio->getTx(5), 3);
    (*lcd)(3, 10);lcd->writeInt8(radio->getTx(6), 3);
}

void PrintRadioData::execute(){
    parser->setMainOption(87, -126);
}
void PrintRadioData::change(){
    parser->setMainOption(87, 122);
}
