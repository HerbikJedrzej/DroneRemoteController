#ifndef SERVOSTRUCTURE_H
#define SERVOSTRUCTURE_H

#include <cstddef>
#include <cstdint>
#include <stdint.h>
#include <cstdlib>

class Servos{
public:
    Servos(uint16_t* list, uint16_t size);
    void set(unsigned int, int);
    uint8_t size();
protected:
    const unsigned int sizeOfList;
    uint16_t* listOfTimersRegisters;
private:
};

#endif
