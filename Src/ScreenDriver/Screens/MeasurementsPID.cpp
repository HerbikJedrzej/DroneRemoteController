#include "MeasurementsPID.hh"
#include "Directory.hh"
#include <OwnExceptions.hh>

TreeCell<ScreenCellIfc>* CreateMeasurementsPIDScreenDirectory(TreeCell<ScreenCellIfc>* root, LCD_ifc* const lcd, Drivers::RadioParser* radio, Drivers::Memory* memory, unsigned int& time){
    TreeCell<ScreenCellIfc>* mainDir = new TreeCell<ScreenCellIfc>(new Directory("PID measure", 11, lcd), root);

    TreeCell<ScreenCellIfc>* setParams = new TreeCell<ScreenCellIfc>(new Directory("Set params", 10, lcd), mainDir);
    TreeCell<ScreenCellIfc>* record = new TreeCell<ScreenCellIfc>(new Directory("Record", 6, lcd), mainDir);
    TreeCell<ScreenCellIfc>* readRecords = new TreeCell<ScreenCellIfc>(new Directory("Send record", 11, lcd), mainDir);
    mainDir->add(setParams);
    mainDir->add(record);
    mainDir->add(readRecords);
    setParams->add(new MeasurementsPID::SetParameters(lcd, radio, memory, 1));
    setParams->add(new MeasurementsPID::SetParameters(lcd, radio, memory, 2));
    setParams->add(new MeasurementsPID::SetParameters(lcd, radio, memory, 3));
    setParams->add(new MeasurementsPID::SetParameters(lcd, radio, memory, 4));
    setParams->add(new MeasurementsPID::SetParameters(lcd, radio, memory, 5));
    record->add(new MeasurementsPID::SendOrCollectRecords(lcd, radio, 1, time, false));
    record->add(new MeasurementsPID::SendOrCollectRecords(lcd, radio, 2, time, false));
    record->add(new MeasurementsPID::SendOrCollectRecords(lcd, radio, 3, time, false));
    record->add(new MeasurementsPID::SendOrCollectRecords(lcd, radio, 4, time, false));
    record->add(new MeasurementsPID::SendOrCollectRecords(lcd, radio, 5, time, false));
    readRecords->add(new MeasurementsPID::SendOrCollectRecords(lcd, radio, 1, time, true));
    readRecords->add(new MeasurementsPID::SendOrCollectRecords(lcd, radio, 2, time, true));
    readRecords->add(new MeasurementsPID::SendOrCollectRecords(lcd, radio, 3, time, true));
    readRecords->add(new MeasurementsPID::SendOrCollectRecords(lcd, radio, 4, time, true));
    readRecords->add(new MeasurementsPID::SendOrCollectRecords(lcd, radio, 5, time, true));
    return mainDir;
}

namespace MeasurementsPID{

SendOrCollectRecords::SendOrCollectRecords(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, const uint8_t _slotNumber, unsigned int& _time, const bool _modeSender):
    ScreenCellIfc(_lcd),
    slotNumber(_slotNumber),
    name{'S', 'l', 'o', 't', ' ', (char)('0' + slotNumber)},
    radio(_radio),
    time(_time),
    modeSender(_modeSender),
    duration((slotNumber > 3)? 1000 : 500){}

const uint8_t* SendOrCollectRecords::getName() const{
    return (uint8_t*)name;
}

uint8_t SendOrCollectRecords::getNameSize() const{
    return 6;
}

void SendOrCollectRecords::print(){
    endTime = time + duration;
    if(modeSender){
        uint8_t title[] = " Sending data x ";
        radio->setMainOption(3, slotNumber);
        title[14] = '0' + slotNumber;
        (*lcd)(1, 0);lcd->write(title, 16);
    }else{
        uint8_t title[] = " Record data  x ";
        radio->setMainOption(2, slotNumber);
        title[14] = '0' + slotNumber;
        (*lcd)(1, 0);lcd->write(title, 16);
    }
    (*lcd)(2, 0);lcd->write((uint8_t*)"        0%      ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"|              |", 16);
}

void SendOrCollectRecords::refresh(){
    if(endTime < time)
        return;
    uint8_t percent = 100 - (endTime - time) * 100 / duration;
    if(percent > 100)
        percent = 100;
    (*lcd)(2, 6);lcd->writeU8(percent, 3);
    (*lcd)(3, 1);lcd->write((uint8_t*)"##############", percent / 7);
}

SetParameters::SetParameters(LCD_ifc* const _lcd, Drivers::RadioParser* _radio, Drivers::Memory* _memory, const uint8_t _slotNumber):
    ScreenCellIfc(_lcd),
    slotNumber(_slotNumber),
    name{'S', 'l', 'o', 't', ' ', (char)('0' + slotNumber)},
    radio(_radio),
    memory(_memory),
    amplitude(0),
    frequency(0),
    amplitudeSaved(0),
    frequencySaved(0),
    amplitudeAddr(memoryMap::slot_1_sinusAmplitude),
    frequencyAddr(memoryMap::slot_1_sinusFrequency),
    pointed(&amplitude){
        uint8_t data[2];
        switch(slotNumber){
            case 1:
                amplitudeAddr = memoryMap::slot_1_sinusAmplitude;
                frequencyAddr = memoryMap::slot_1_sinusFrequency;
                memory->read(amplitudeAddr, data, 2);
                break;
            case 2:
                amplitudeAddr = memoryMap::slot_2_sinusAmplitude;
                frequencyAddr = memoryMap::slot_2_sinusFrequency;
                memory->read(amplitudeAddr, data, 2);
                break;
            case 3:
                amplitudeAddr = memoryMap::slot_3_sinusAmplitude;
                frequencyAddr = memoryMap::slot_3_sinusFrequency;
                memory->read(amplitudeAddr, data, 2);
                break;
            case 4:
                amplitudeAddr = memoryMap::slot_4_sinusAmplitude;
                frequencyAddr = memoryMap::slot_4_sinusFrequency;
                memory->read(amplitudeAddr, data, 2);
                break;
            case 5:
                amplitudeAddr = memoryMap::slot_5_sinusAmplitude;
                frequencyAddr = memoryMap::slot_5_sinusFrequency;
                memory->read(amplitudeAddr, data, 2);
                break;
            default:
                THROW_out_of_range("Only slots  [1, 2, 3, 4, 5] are allowed.");
        }
        amplitude = data[0];
        frequency = data[1];
        amplitudeSaved = amplitude;
        frequencySaved = frequency;        
    }

const uint8_t* SetParameters::getName() const{
    return (uint8_t*)name;
}

uint8_t SetParameters::getNameSize() const{
    return 6;
}

void SetParameters::print(){
    amplitude = amplitudeSaved;
    frequency = frequencySaved;
    uint8_t title[] = "Set params of x ";
    title[14] = '0' + slotNumber;
    (*lcd)(1, 0);lcd->write(title, 16);
    (*lcd)(2, 0);lcd->write((uint8_t*)"  A: xx.xx [%]  ", 16);
    (*lcd)(3, 0);lcd->write((uint8_t*)"  f: xx.xx [Hz] ", 16);
    (*lcd)((pointed == &amplitude)? 2 : 3, 0);lcd->write((uint8_t*)"->", 2);
}

void SetParameters::refresh(){
    (*lcd)(2, 4);lcd->writeDouble(double(amplitude) / 8 + 0.25, 2, 2);
    (*lcd)(3, 4);lcd->writeDouble(double(frequency) * 9.9 / 255 + 0.1, 2, 2);
}

void SetParameters::execute(){
    if(pointed == &amplitude){
        amplitudeSaved = amplitude;
        memory->writeDMAwithoutDataAlocate(amplitudeAddr, &amplitudeSaved, 1, nullptr);
        radio->setMainOption(2 + 2 * slotNumber, amplitudeSaved);
        (*lcd)(2, 15);lcd->write((uint8_t*)"*", 1);
    }
    else{
        frequencySaved = frequency;        
        memory->writeDMAwithoutDataAlocate(frequencyAddr, &frequencySaved, 1, nullptr);
        radio->setMainOption(3 + 2 * slotNumber, frequencySaved);
        (*lcd)(3,  15);lcd->write((uint8_t*)"*", 1);
    }
}

void SetParameters::increment(){
    (*pointed)++;
}

void SetParameters::decrement(){
    (*pointed)--;
}

void SetParameters::change(){
    bool amplitudeIsPointed = pointed == &amplitude;
    pointed = (amplitudeIsPointed)? &frequency : &amplitude;
    (*lcd)((amplitudeIsPointed)? 3 : 2, 0);lcd->write((uint8_t*)"->", 2);
    (*lcd)((amplitudeIsPointed)? 3 : 2, 15);lcd->write((uint8_t*)" ", 1);
    (*lcd)((amplitudeIsPointed)? 2 : 3,  15);lcd->write((uint8_t*)" ", 1);
    (*lcd)((amplitudeIsPointed)? 2 : 3,  0);lcd->write((uint8_t*)"  ", 2);
}

}