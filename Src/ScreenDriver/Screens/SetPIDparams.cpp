#include "SetPIDparams.hh"
#include "Directory.hh"
#include <OwnExceptions.hh>

TreeCell<ScreenCellIfc>* CreateSetPIDsParams(TreeCell<ScreenCellIfc>* root, LCD_ifc* const lcd, Drivers::RadioParser* radio, Drivers::Memory* memory){
    TreeCell<ScreenCellIfc>* mainDir = new TreeCell<ScreenCellIfc>(new Directory("Set PID params", 14, lcd), root);
    mainDir->add(new SetPIDparams(lcd, radio, memory, SetPIDparams::Axis::axisX));
    mainDir->add(new SetPIDparams(lcd, radio, memory, SetPIDparams::Axis::axisY));
    mainDir->add(new SetPIDparams(lcd, radio, memory, SetPIDparams::Axis::axisZ));
    mainDir->add(new SetPIDparams(lcd, radio, memory, SetPIDparams::Axis::axisH));
    mainDir->add(new SetPIDFactor(lcd, radio, memory));
    return mainDir;
}

SetPIDparams::SetPIDparams(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory, const Axis _axis):
    ScreenCellIfc(_lcd),
    axis(_axis),
    axisName(
        [](const Axis& axis)->char{
            switch(axis){
                case Axis::axisX:
                    return 'X';
                case Axis::axisY:
                    return 'Y';
                case Axis::axisZ:
                    return 'Z';
                case Axis::axisH:
                    return 'H';
                default:
                    THROW_out_of_range("Only axis  [X, Y, Z, H] are allowed.");
            }
    	}(_axis)),
    name{'P', 'I', 'D', ' ', 'a', 'x', 'i', 's', ' ', axisName},
    radio(_radio),
    memory(_memory){
        switch(_axis){
            case Axis::axisX:
                addr = memoryMap::pid_P_XY_00;
                break;
            case Axis::axisY:
                addr = memoryMap::pid_P_XY_11;
                break;
            case Axis::axisZ:
                addr = memoryMap::pid_P_Z;
                break;
            case Axis::axisH:
                addr = memoryMap::pid_P_H;
                break;
        }
        memory->read(addr, realParam, 3);
    }

const uint8_t* SetPIDparams::getName() const{
    return (uint8_t*)name;
}

uint8_t SetPIDparams::getNameSize() const{
    return 10;
}

void SetPIDparams::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)"  P_", 4);lcd->writeData(axisName);lcd->write((uint8_t*)": x.xxxxx  ", 11);
    (*lcd)(2, 0);lcd->write((uint8_t*)"  I_", 4);lcd->writeData(axisName);lcd->write((uint8_t*)": x.xxxxx  ", 11);
    (*lcd)(3, 0);lcd->write((uint8_t*)"  D_", 4);lcd->writeData(axisName);lcd->write((uint8_t*)": x.xxxxx  ", 11);
    param[0] = realParam[0];
    param[1] = realParam[1];
    param[2] = realParam[2];
}

void SetPIDparams::refresh(){
    if(axis == Axis::axisZ){
        (*lcd)(1, 6);lcd->writeDouble(double(param[0]) * 7.0 / 255 + 1.0, 1, 5); // [8.0 - 1.0]
        (*lcd)(2, 6);lcd->writeDouble(double(param[1]) * 1.0 / 255      , 1, 5); // [1.0 - 0.0]
        (*lcd)(3, 6);lcd->writeDouble(double(param[2]) * 1.0 / 255      , 1, 5); // [1.0 - 0.0]
    }else if(axis == Axis::axisH){
        (*lcd)(1, 6);lcd->writeDouble(double(param[0]) *   8.0 / 255 + 4.0  , 2, 4); // [ 12.0 - 4.0]
        (*lcd)(2, 6);lcd->writeDouble(double(param[1]) * 0.495 / 255 + 0.005, 1, 5); // [  0.5 - 0.005]
        (*lcd)(3, 6);lcd->writeDouble(double(param[2]) * 100.0 / 255 + 50.0 , 3, 3); // [150.0 - 50.0]
    }else{
        (*lcd)(1, 6);lcd->writeDouble(double(param[0]) * 7.0   / 255 + 1.0  , 1, 5); // [8.0   - 1.0  ]
        (*lcd)(2, 6);lcd->writeDouble(double(param[1]) * 0.03  / 255 + 0.005, 1, 5); // [0.035 - 0.005]
        (*lcd)(3, 6);lcd->writeDouble(double(param[2]) * 180.0 / 255 + 20.0 , 3, 3); // [200   - 20   ]
    }
    for(uint8_t i = 0; i < 3; i++){
        (*lcd)(i + 1, 0);lcd->write((uint8_t*)((i == pointedParam)? "->" : "  "), 2);
    }

}

void SetPIDparams::execute(){
    realParam[pointedParam] = param[pointedParam];
    memory->writeDMAwithoutDataAlocate(addr + pointedParam, &realParam[pointedParam], 1, nullptr);
    radio->setMainOption(15 + uint8_t(axis) * 3 + pointedParam, realParam[pointedParam]);
    (*lcd)(pointedParam + 1, 15);lcd->writeData('*');
}

void SetPIDparams::increment(){
    param[pointedParam]++;
}

void SetPIDparams::decrement(){
    param[pointedParam]--;
}

void SetPIDparams::change(){
    (*lcd)(++pointedParam, 15);lcd->writeData(' ');
    if(pointedParam == 3)
        pointedParam = 0;
}

constexpr char SetPIDFactor::name[];

SetPIDFactor::SetPIDFactor(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory):
    ScreenCellIfc(_lcd),
    radio(_radio),
    memory(_memory){
        memory->read(memoryMap::commonFactorForAxisXY, &realParam, 1);
        param = realParam;
    }

const uint8_t* SetPIDFactor::getName() const{
    return (uint8_t*)name;
}

uint8_t SetPIDFactor::getNameSize() const{
    return 14;
}

void SetPIDFactor::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)"  PIDs factor   ", 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"  old: x.xxxxx  ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"  new: x.xxxxx  ", 16);
}

void SetPIDFactor::refresh(){
    (*lcd)(2, 6);lcd->writeDouble(double(realParam) * 0.19 / 255 + 0.01, 1, 5); // [0.01 - 0.20]
    (*lcd)(3, 6);lcd->writeDouble(double(param)     * 0.19 / 255 + 0.01, 1, 5); // [0.01 - 0.20]
}

void SetPIDFactor::execute(){
    realParam = param;
    memory->writeDMAwithoutDataAlocate(memoryMap::commonFactorForAxisXY, &realParam, 1, nullptr);
    radio->setMainOption(14, realParam);
}

void SetPIDFactor::increment(){
    param++;
}

void SetPIDFactor::decrement(){
    param--;
}
