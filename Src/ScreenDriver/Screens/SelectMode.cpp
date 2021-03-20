#include "SelectMode.hh"
#include "Directory.hh"
#include <OwnExceptions.hh>
#include <memoryMap.hh>

constexpr char SelectMode::name[];
constexpr unsigned int SelectMode::modesSize;
constexpr uint8_t SelectMode::modesNames[SelectMode::modesSize][13];

SelectMode::SelectMode(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory):
    ScreenCellIfc(_lcd),
    radio(_radio),
    memory(_memory),
    mode(0),
    changed(true){
        memory->read(memoryMap::modeOfMainloop, &mode, 1);
        pointedMode = mode;
        static_assert(modesSize <= 256, "There cannot be more modes than 256 (8-bit value).");
    }

const uint8_t* SelectMode::getName() const{
    return (uint8_t*)name;
}

uint8_t SelectMode::getNameSize() const{
    return sizeof(name) / sizeof(char) - 1;
}

void SelectMode::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)"                ", 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"->              ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"                ", 16);
    changed = true;
}

void SelectMode::refresh(){
    if(!changed)
        return;
    changed = false;
    uint8_t startPoint = pointedMode - 1;
    uint8_t endPoint = pointedMode + 1;
    uint8_t screenPoint = 1;
    if(pointedMode == 0){
        (*lcd)(screenPoint++, 2);lcd->write((uint8_t*)"              ", 14);
        startPoint++;
    }
    if(pointedMode == modesSize - 1){
        (*lcd)(3, 2);lcd->write((uint8_t*)"              ", 14);
        endPoint--;
    }
    for(uint8_t i = startPoint; i <= endPoint; i++, screenPoint++){
        (*lcd)(screenPoint, 2);lcd->write(modesNames[i], 12);
        if(i == mode)
            lcd->write((uint8_t*)"<-", 2);
        else
            lcd->write((uint8_t*)"  ", 2);
    }
}

void SelectMode::increment(){
    if(pointedMode == modesSize - 1)
        pointedMode = 0;
    else
        pointedMode++;
    changed = true;
}

uint8_t SelectMode::getMode(){
    return mode;
}


void SelectMode::decrement(){
    if(pointedMode == 0)
        pointedMode = modesSize - 1;
    else
        pointedMode--;
    changed = true;
}

void SelectMode::execute(){
    mode = pointedMode;
    memory->writeDMAwithoutDataAlocate(memoryMap::modeOfMainloop, &mode, 1, nullptr);
    radio->setMainOption(1, mode);
    changed = true;
}
