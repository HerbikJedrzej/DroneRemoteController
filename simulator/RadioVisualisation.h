#pragma once
#include <string>
#include <vector>

class RadioVisualisation{
protected:
  void printToCoordinates(unsigned int x, unsigned int y, char element);
  void printToCoordinates(unsigned int x, unsigned int y, const std::string& text);
  const std::string uintToHex(uint8_t);
  const std::string uintToInt(uint8_t);
  const std::string uintToUint(uint8_t);
public:
  RadioVisualisation();
  ~RadioVisualisation();
  void printRadioTransmision(const std::vector<uint8_t>& data);
  void printCycleNumber(uint32_t data);
  void printEncoder(uint32_t data);
  void printCoordinates(const int8_t& roll,const int8_t& pitch, const int8_t& yawl, const int8_t& H);
  void printLeds(const std::vector<bool>& leds);
  void printScreenLCD(const std::vector<std::string>& txt);
};