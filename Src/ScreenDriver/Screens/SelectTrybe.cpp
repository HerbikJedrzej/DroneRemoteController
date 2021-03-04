#include "SelectTrybe.hh"
#include "Directory.hh"
#include <OwnExceptions.hh>
#include <memoryMap.hh>

constexpr char SelectTrybe::name[];
constexpr unsigned int SelectTrybe::trybesSize;
constexpr uint8_t SelectTrybe::trybesNames[SelectTrybe::trybesSize][13];

SelectTrybe::SelectTrybe(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory):
    ScreenCellIfc(_lcd),
    radio(_radio),
    memory(_memory),
    trybe(0),
    changed(true){
        memory->read(memoryMap::trybeOfMainloop, &trybe, 1);
        pointedTrybe = trybe;
        static_assert(trybesSize <= 256, "There cannot be more trybes than 256 (8-bit value).");
    }

const uint8_t* SelectTrybe::getName() const{
    return (uint8_t*)name;
}

uint8_t SelectTrybe::getNameSize() const{
    return sizeof(name) / sizeof(char) - 1;
}

void SelectTrybe::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)"                ", 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"->              ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"                ", 16);
    changed = true;
}

void SelectTrybe::refresh(){
    if(!changed)
        return;
    changed = false;
    uint8_t startPoint = pointedTrybe - 1;
    uint8_t endPoint = pointedTrybe + 1;
    uint8_t screenPoint = 1;
    if(pointedTrybe == 0){
        (*lcd)(screenPoint++, 2);lcd->write((uint8_t*)"              ", 14);
        startPoint++;
    }
    if(pointedTrybe == trybesSize - 1){
        (*lcd)(3, 2);lcd->write((uint8_t*)"              ", 14);
        endPoint--;
    }
    for(uint8_t i = startPoint; i <= endPoint; i++, screenPoint++){
        (*lcd)(screenPoint, 2);lcd->write(trybesNames[i], 12);
        if(i == trybe)
            lcd->write((uint8_t*)"<-", 2);
        else
            lcd->write((uint8_t*)"  ", 2);
    }
}

void SelectTrybe::increment(){
    if(pointedTrybe == trybesSize - 1)
        pointedTrybe = 0;
    else
        pointedTrybe++;
    changed = true;
}

uint8_t SelectTrybe::getTrybe(){
    return trybe;
}


void SelectTrybe::decrement(){
    if(pointedTrybe == 0)
        pointedTrybe = trybesSize - 1;
    else
        pointedTrybe--;
    changed = true;
}

void SelectTrybe::execute(){
    trybe = pointedTrybe;
    memory->writeDMAwithoutDataAlocate(memoryMap::trybeOfMainloop, &trybe, 1, nullptr);
    radio->setMainOption(1, trybe);
    changed = true;
}
