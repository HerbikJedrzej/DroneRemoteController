#include "Directory.hh"

Directory::Directory(const char* _name, const uint8_t _nameSize, LCD_ifc* const _lcd):
    ScreenCellIfc(_lcd),
    nameSize(_nameSize + 1),
    name(new uint8_t[nameSize]){
        for(uint8_t i = 0; i < _nameSize; i++)
            name[i + 1] = _name[i];
        name[0] = '*';
    }

Directory::~Directory(){
    delete [] name;
}

const uint8_t* Directory::getName() const{
    return (uint8_t*)name;
}

uint8_t Directory::getNameSize() const{
    return nameSize;
}

void Directory::print(){}

void Directory::refresh(){}

void Directory::execute(){
}

void Directory::change(){
}
