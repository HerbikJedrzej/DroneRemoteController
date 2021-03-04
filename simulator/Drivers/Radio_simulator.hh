#pragma once

#include <RadioIfc.hh>
#include <string> 
#include <vector>

namespace Drivers{

class RadioSimulator : public RadioIfc
{
public:
	RadioSimulator(const uint8_t _key, unsigned int& _time, std::vector<uint8_t>& _radioData, const std::string& pipeName);
	~RadioSimulator();
	bool isKyeCorrect() final;
	bool isAckCorrect() final;
	bool isComunicationCorrect() final;
	void init() final;
	uint8_t getTx(unsigned int i) const final;
	uint8_t getRx(unsigned int i) const final;
	uint8_t& setTx(unsigned int i) final;
	uint8_t& setRx(unsigned int i) final;
	void handleTimeEvent(DriverIfc*) final;
protected:
	const std::string uintToHex(uint8_t);
	bool keyIsCorrect;
	std::vector<uint8_t>& radioData;
	uint8_t* const correctRadioDataOut;
	uint8_t* const correctRadioDataIn;
	uint8_t* const dataToPipeStream;
    int fifoPipeHandler; 
};

}
