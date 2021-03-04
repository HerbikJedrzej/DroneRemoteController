#include "MeasurementsAltitude.hh"
#include "Directory.hh"
#include <OwnExceptions.hh>

TreeCell<ScreenCellIfc>* CreateMeasurementsAltitudeScreenDirectory(TreeCell<ScreenCellIfc>* root, LCD_ifc* const lcd, Drivers::RadioParser* radio, bool& triggerButton, unsigned int& time){
    TreeCell<ScreenCellIfc>* mainDir = new TreeCell<ScreenCellIfc>(new Directory("Alt(H) measure", 14, lcd), root);
    mainDir->add(new MeasurementsAltitude::SendOrCollectRecords(lcd, radio, triggerButton, time, false));
    mainDir->add(new MeasurementsAltitude::SendOrCollectRecords(lcd, radio, triggerButton, time, true));
    return mainDir;
}

namespace MeasurementsAltitude{

bool SendOrCollectRecords::waitForJoyButton = true;

SendOrCollectRecords::SendOrCollectRecords(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, bool& _triggerButton, unsigned int& _time, const bool _modeSender):
    ScreenCellIfc(_lcd),
    slotNumber(1),
    name{(_modeSender)? "Send record" : "Record"},
    radio(_radio),
    time(_time),
    modeSender(_modeSender),
    triggerButton(_triggerButton),
    duration((slotNumber > 3)? 500 : 250){}

const uint8_t* SendOrCollectRecords::getName() const{
    return (uint8_t*)name;
}

uint8_t SendOrCollectRecords::getNameSize() const{
    return (modeSender)? 11 : 6;
}

void SendOrCollectRecords::print(){
    (*lcd)(1, 0);lcd->write((modeSender)? (uint8_t*)" Sending data x " : (uint8_t*)" Record data  x ", 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"        0%      ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"|              |", 16);
}

void SendOrCollectRecords::refresh(){
    (*lcd)(1, 14);
    lcd->writeData('0' + slotNumber);
    if(waitForJoyButton){
        if(!triggerButton){
            endTime = time;
            return;
        }
        waitForJoyButton = false;
        endTime = time + duration;
        (*lcd)(3, 0);lcd->write((uint8_t*)"|              |", 16);
        radio->setMainOption((modeSender)? 3 : 31, slotNumber);
    }
    if(endTime < time){
        waitForJoyButton = true;
        return;
    }
    uint8_t percent = 100 - (endTime - time) * 100 / duration;
    if(percent > 100)
        percent = 100;
    (*lcd)(2, 6);lcd->writeU8(percent, 3);
    (*lcd)(3, 1);lcd->write((uint8_t*)"##############", percent / 7);
}

void SendOrCollectRecords::increment(){
    if(!waitForJoyButton)
        return;
    slotNumber++;
    if(slotNumber > 5)
        slotNumber = 1;
}

void SendOrCollectRecords::decrement(){
    if(!waitForJoyButton)
        return;
    slotNumber--;
    if(slotNumber == 0)
        slotNumber = 5;
}

}