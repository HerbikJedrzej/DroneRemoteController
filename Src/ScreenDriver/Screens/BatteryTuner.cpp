#include "BatteryTuner.hh"
#include "Directory.hh"
#include <OwnExceptions.hh>

constexpr char BatteryValue::name[];
constexpr char BatterySaveValue::name[];
constexpr char ControledDeviceBatterySaveValue::name[];
constexpr char BatterySaveMinMax::name[];

TreeCell<ScreenCellIfc>* CreateBatteryScreenDirectory(TreeCell<ScreenCellIfc>* root, LCD_ifc* const lcd, BatteryObserver* const battery, Drivers::RadioParser* const radio){
    TreeCell<ScreenCellIfc>* dir = new TreeCell<ScreenCellIfc>(new Directory("Battery", 7, lcd), root);
    dir->add(new BatteryValue(lcd, battery));
    dir->add(new ControledDeviceBatterySaveValue(lcd, radio));
    dir->add(new BatterySaveValue(lcd, battery));
    dir->add(new BatterySaveMinMax(lcd, battery));
    return dir;
}

BatteryValue::BatteryValue(LCD_ifc* const _lcd, BatteryObserver* const _battery):
    ScreenCellIfc(_lcd),
    battery(_battery){}

const uint8_t* BatteryValue::getName() const{
    return (uint8_t*)name;
}

uint8_t BatteryValue::getNameSize() const{
    return sizeof(name) / sizeof(char) - 1;
}

void BatteryValue::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)"   %  :    xxx  ", 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"  adc :  xxxxx  ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"   V  : xx.xxx  ", 16);
}

void BatteryValue::refresh(){
    (*lcd)(1, 11);lcd->writeU8(battery->batteryPercentVal(), 3);
    (*lcd)(2, 9);lcd->writeU16(battery->batteryRawVal(), 5);
    (*lcd)(3, 7);lcd->writeDouble(battery->batteryVoltageVal(), 2, 3);
}

void BatteryValue::execute(){}
void BatteryValue::change(){}

ControledDeviceBatterySaveValue::ControledDeviceBatterySaveValue(LCD_ifc* const _lcd, Drivers::RadioParser* const _radio):
    ScreenCellIfc(_lcd),
    radio(_radio),
    voltage(33){}

const uint8_t* ControledDeviceBatterySaveValue::getName() const{
    return (uint8_t*)name;
}

uint8_t ControledDeviceBatterySaveValue::getNameSize() const{
    return sizeof(name) / sizeof(char) - 1;
}

void ControledDeviceBatterySaveValue::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)" Connect remote ", 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"device to xx.x V", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"  and press OK  ", 16);
}

void ControledDeviceBatterySaveValue::refresh(){
    (*lcd)(2, 9);lcd->writeDouble((double(voltage) + 0.01) /10, 2, 1);
}

void ControledDeviceBatterySaveValue::execute(){
    radio->setMainOption(27, int8_t(voltage));
    (*lcd)(3, 15);lcd->writeData('*');
}

void ControledDeviceBatterySaveValue::increment(){
    voltage++;
    (*lcd)(3, 15);lcd->writeData(' ');
}

void ControledDeviceBatterySaveValue::decrement(){
    voltage--;
    (*lcd)(3, 15);lcd->writeData(' ');
}

void ControledDeviceBatterySaveValue::change(){
    switch (voltage){
        case 33:
            voltage = 37;
            break;
        case 37:
            voltage = 70;
            break;
        case 70:
            voltage = 74;
            break;
        case 74:
            voltage = 111;
            break;
        case 111:
            voltage = 120;
            break;
        default:
            voltage = 33;
            break;
    }
    (*lcd)(2, 9);lcd->writeDouble((double(voltage) + 0.01)/10, 2, 1);
    (*lcd)(3, 15);lcd->writeData(' ');
}

BatterySaveValue::BatterySaveValue(LCD_ifc* const _lcd, BatteryObserver* const _battery):
    ScreenCellIfc(_lcd),
    battery(_battery),
    mode9V(false){}

const uint8_t* BatterySaveValue::getName() const{
    return (uint8_t*)name;
}

uint8_t BatterySaveValue::getNameSize() const{
    return sizeof(name) / sizeof(char) - 1;
}

void BatterySaveValue::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)" Connect to xxV ", 16);
    (*lcd)(1, 12);lcd->write(mode9V? (uint8_t*)" 9" : (uint8_t*)"12", 2);
    (*lcd)(2, 0);lcd->write((uint8_t*)"  and press OK  ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"   V  : xx.xxx  ", 16);
}

void BatterySaveValue::refresh(){
    (*lcd)(3, 7);lcd->writeDouble(battery->batteryVoltageVal(), 2, 3);
}

void BatterySaveValue::execute(){
    if(mode9V)
        battery->correctResistor1whenPowerSupplayis9V();
    else
        battery->correctResistor1whenPowerSupplayis12V();
}

void BatterySaveValue::change(){
    mode9V = !mode9V;
    (*lcd)(1, 12);lcd->write(mode9V? (uint8_t*)" 9" : (uint8_t*)"12", 2);
}

BatterySaveMinMax::BatterySaveMinMax(LCD_ifc* const _lcd, BatteryObserver* const _battery):
    ScreenCellIfc(_lcd),
    battery(_battery),
    option(1){}

const uint8_t* BatterySaveMinMax::getName() const{
    return (uint8_t*)name;
}

uint8_t BatterySaveMinMax::getNameSize() const{
    return sizeof(name) / sizeof(char) - 1;
}

void BatterySaveMinMax::print(){
    (*lcd)(1, 0);lcd->write((uint8_t*)"  Save max val  ", 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"  Save min val  ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"  Auto save xxx ", 16);
    (*lcd)(3, 12);lcd->write(battery->IsAutoSaveMinMode()? (uint8_t*)"off" : (uint8_t*)"on ", 3);
    (*lcd)(option, 0);lcd->writeData('>');
    (*lcd)(option, 15);lcd->writeData('<');
}

void BatterySaveMinMax::refresh(){}

void BatterySaveMinMax::execute(){
    switch (option)
    {
    case 1:
        battery->saveMaxBat();
        break;
    case 2:
        battery->saveMinBat();
        break;
    case 3:
        if(battery->IsAutoSaveMinMode())
            battery->AutoSaveMinModeOff();
        else
            battery->AutoSaveMinModeOn();
        break;
    default:
        return;
    }
    (*lcd)(option, 0);lcd->writeData('*');
    (*lcd)(option, 15);lcd->writeData('*');
    (*lcd)(3, 12);lcd->write(battery->IsAutoSaveMinMode()? (uint8_t*)"off" : (uint8_t*)"on ", 3);
}

void BatterySaveMinMax::increment(){
    (*lcd)(option, 0);lcd->writeData(' ');
    (*lcd)(option, 15);lcd->writeData(' ');
    option++;
    if(option > 3)
        option = 1;
    (*lcd)(option, 0);lcd->writeData('>');
    (*lcd)(option, 15);lcd->writeData('<');
}

void BatterySaveMinMax::decrement(){
    (*lcd)(option, 0);lcd->writeData(' ');
    (*lcd)(option, 15);lcd->writeData(' ');
    option--;
    if(option == 0)
        option = 3;
    (*lcd)(option, 0);lcd->writeData('>');
    (*lcd)(option, 15);lcd->writeData('<');
}
