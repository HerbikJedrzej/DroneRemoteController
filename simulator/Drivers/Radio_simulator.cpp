#include "Radio_simulator.hh"
#include <stdexcept>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

namespace Drivers{

RadioSimulator::RadioSimulator(const uint8_t _key, unsigned int& _time, std::vector<uint8_t>& _radioData, const std::string& pipeName):
	RadioIfc(_key, _time),
	keyIsCorrect(false),
	radioData(_radioData),
	correctRadioDataOut(new uint8_t [_radioData.size() - 1]),
	correctRadioDataIn(new uint8_t [_radioData.size() - 1]),
	dataToPipeStream(new uint8_t [_radioData.size() * 3]){
		for(uint8_t i = 0; i < radioData.size(); i++){
			correctRadioDataOut[i] = 0;
			correctRadioDataIn[i] = 0;
		}
		mkfifo(pipeName.c_str(), 0777);
		fifoPipeHandler = open(pipeName.c_str(), O_WRONLY);
	}

RadioSimulator::~RadioSimulator(){
	const char* txt = "00 00 00 00 00 00 00 00 ";
	write(fifoPipeHandler, txt, 24); 
    std::this_thread::sleep_for(20ms);
	delete [] correctRadioDataOut;
	delete [] correctRadioDataIn;
	delete [] dataToPipeStream;
	close(fifoPipeHandler);
}

bool RadioSimulator::isKyeCorrect(){
	return keyIsCorrect;
}

bool RadioSimulator::isAckCorrect(){
	return true;
}

bool RadioSimulator::isComunicationCorrect(){
	return true;
}

void RadioSimulator::init(){
}

uint8_t RadioSimulator::getTx(unsigned int i) const{
	if(i > radioData.size() - 2)
		throw std::out_of_range("Trying to get vaule out of radio data. i = " + std::to_string(i));
	return correctRadioDataOut[i];
}

uint8_t RadioSimulator::getRx(unsigned int i) const{
	if(i > radioData.size() - 2)
		throw std::out_of_range("Trying to get vaule out of radio data. i = " + std::to_string(i));
	return correctRadioDataIn[i];
}

uint8_t& RadioSimulator::setTx(unsigned int i){
	if(i > radioData.size() - 2)
		throw std::out_of_range("Trying to get vaule out of radio data. i = " + std::to_string(i));
	return correctRadioDataOut[i];
}

uint8_t& RadioSimulator::setRx(unsigned int i){
	if(i > radioData.size() - 2)
		throw std::out_of_range("Trying to get vaule out of radio data. i = " + std::to_string(i));
	return correctRadioDataIn[i];
}

void RadioSimulator::handleTimeEvent(DriverIfc*){
	for(uint8_t i = 1; i < radioData.size() - 1; i++)
		radioData[i] = correctRadioDataOut[i];
	radioData[0] = (key & 0xf0) | (correctRadioDataOut[0] & 0x0f);
	radioData[radioData.size() - 1] = (key & 0x0f) | (correctRadioDataOut[0] & 0xf0);
	for(unsigned int i = 0; i < radioData.size(); i++){
		auto& stringNumber = uintToHex(radioData[i]);
		for(unsigned int j = 0; j < 3; j++)
			dataToPipeStream[i * 3 + j] = stringNumber[j];
		}
	write(fifoPipeHandler, dataToPipeStream, radioData.size() * 3); 
}

const std::string RadioSimulator::uintToHex(uint8_t v){
  std::string toReturn("00 ");
  uint8_t signVal = v / 16;
  if(signVal > 9)
    toReturn[0] = 55;
  toReturn[0] += signVal;
  signVal = v % 16;
  if(signVal > 9)
    toReturn[1] = 55;
  toReturn[1] += signVal;
  return toReturn;
}

}