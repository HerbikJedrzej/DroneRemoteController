#include "ServoStructure.hh"

Servos::Servos(uint16_t* list, uint16_t size):
    sizeOfList(size),
    listOfTimersRegisters(list)
    {}

void Servos::set(unsigned int i, int angle){
    listOfTimersRegisters[i] = angle;
}

uint8_t Servos::size(){
    return sizeOfList;
}
