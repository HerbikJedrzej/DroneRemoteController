#include "RadioVisualisation.h"
#include <ncurses.h>
#include <stdexcept>

RadioVisualisation::RadioVisualisation(){
  system("setterm -cursor off");
  system("clear");
  printf("\n");
  printf("                   =============================()\n");
  printf("      Cycle number: xxxxx                       ||    LEDs legend:\n");
  printf("                                                ||    x - on   o - off\n");
  printf("                     to quit press \']\'          ||\n");
  printf("      ============================|=V=|=========II==========================\n");
  printf("     |    _______                             |   | gggbbyyyrr   _______    |\n");
  printf("     |   /       \\    0xXX       0bXXXXXXXX   | ^ | xxxxxxxxxx  /       \\   |\n");
  printf("     |  /    w    \\    XXX  XXX  XXX  XXX     | | |            /    i    \\  |\n");
  printf("     | |   a(q)d   |        0xXX XXX          | | |           |   j(o)l   | |\n");
  printf("     |  \\    s    /                           |   |            \\    k    /  |\n");
  printf("     |   \\_______/        (1)       (2)       |___|             \\_______/   |\n");
  printf("     |  yawl: -XXX                                    ___       roll: -XXX  |\n");
  printf("     |   alt: -XXX        (3)       (4)            f /   \\ h   pitch: -XXX  |\n");
  printf("     |                                              |  g  |                 |\n");
  printf("     |                                               \\___/                  |\n");
  printf("     |    (e)                                                        (u)    |\n");
  printf("     |                                                                      |\n");
  printf("     |                         ********************                         |\n");
  printf("     |                         **----------------**                         |\n");
  printf("     |                         *|                |*                         |\n");
  printf("     |                         *|                |*                         |\n");
  printf("     |                         *|                |*                         |\n");
  printf("     |                         *|                |*                         |\n");
  printf("     |                         **----------------**                         |\n");
  printf("     |                         ********************                         |\n");
  printf("     |                                                                      |\n");
  printf("      ======================================================================\n");
}
RadioVisualisation::~RadioVisualisation(){
  system("clear");
  system("setterm -cursor on");
}

void RadioVisualisation::printToCoordinates(unsigned int x, unsigned int y, char element){
  printf("\033[%d;%dH%c\n", x, y, element);
  printf("\033[0;0H");
}

void RadioVisualisation::printToCoordinates(unsigned int x, unsigned int y, const std::string& text){
  printf("\033[%d;%dH%s\n", x, y, text.c_str());
  printf("\033[0;0H");
}

void RadioVisualisation::printCycleNumber(uint32_t val){
  std::string toPrint("00000");
  toPrint[4] += val % 10;
  val /= 10;
  toPrint[3] += val % 10;
  val /= 10;
  toPrint[2] += val % 10;
  val /= 10;
  toPrint[1] += val % 10;
  val /= 10;
  toPrint[0] += val % 10;
  printToCoordinates(3, 21, toPrint);
}

const std::string RadioVisualisation::uintToHex(uint8_t v){
  std::string toReturn("00");
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

const std::string RadioVisualisation::uintToInt(uint8_t v){
  int16_t val = (int16_t)((int8_t)v);
  std::string toReturn(" 000");
  if(val < 0){
    toReturn[0] = '-';
    val = -val;
  }
  toReturn[3] += val % 10;
  val /= 10;
  toReturn[2] += val % 10;
  val /= 10;
  toReturn[1] += val % 10;
  return toReturn;
}

const std::string RadioVisualisation::uintToUint(uint8_t val){
  std::string toReturn("000");
  toReturn[2] += val % 10;
  val /= 10;
  toReturn[1] += val % 10;
  val /= 10;
  toReturn[0] += val % 10;
  return toReturn;
}

void RadioVisualisation::printRadioTransmision(const std::vector<uint8_t>& data){
  uint8_t key = (data[0] & 0xf0) | (data[7] & 0x0f);
  uint8_t bits = (data[0] & 0x0f) | (data[7] & 0xf0);
  std::string bitsTab("00000000");
  for(int8_t i = 7; i >= 0; i--){
    if((bits & 1) != 0)
      bitsTab[i] = '1';
    bits = bits >> 1;
  }
  printToCoordinates(8, 25, uintToHex(key));
  printToCoordinates(8, 36, bitsTab);
  printToCoordinates(9, 24, uintToUint(data[1]));
  printToCoordinates(9, 29, uintToUint(data[2]));
  printToCoordinates(9, 34, uintToUint(data[3]));
  printToCoordinates(9, 39, uintToUint(data[4]));
  printToCoordinates(10, 31, uintToHex(data[5]));
  printToCoordinates(10, 34, uintToUint(data[6]));
}

void RadioVisualisation::printCoordinates(const int8_t& roll, const int8_t& pitch, const int8_t& yawl, const int8_t& H){
  printToCoordinates(13, 71, uintToInt(uint8_t(roll)));
  printToCoordinates(14, 71, uintToInt(uint8_t(pitch)));
  printToCoordinates(13, 15, uintToInt(uint8_t(yawl)));
  printToCoordinates(14, 15, uintToInt(uint8_t(H)));
}

void RadioVisualisation::printEncoder(uint32_t val){
  std::string toPrint("00000");
  toPrint[4] += val % 10;
  val /= 10;
  toPrint[3] += val % 10;
  val /= 10;
  toPrint[2] += val % 10;
  val /= 10;
  toPrint[1] += val % 10;
  val /= 10;
  toPrint[0] += val % 10;
  printToCoordinates(17, 54, toPrint);
}

void RadioVisualisation::printLeds(const std::vector<bool>& leds){
    std::string bitLeds("xxxxxxxxxx");
    for(unsigned int i = 0 ;i < leds.size(); i++)
      if(leds[i])bitLeds[i] = 'o';
    printToCoordinates(8, 53, bitLeds);
}

void RadioVisualisation::printScreenLCD(const std::vector<std::string>& txt){
    if(txt[0].size() != 16)
        throw std::invalid_argument("Size of line nr. 1. is different than 16. Size = " + std::to_string(txt[0].size()));
    if(txt[1].size() != 16)
        throw std::invalid_argument("Size of line nr. 1. is different than 16. Size = " + std::to_string(txt[1].size()));
    if(txt[2].size() != 16)
        throw std::invalid_argument("Size of line nr. 1. is different than 16. Size = " + std::to_string(txt[2].size()));
    if(txt[3].size() != 16)
        throw std::invalid_argument("Size of line nr. 1. is different than 16. Size = " + std::to_string(txt[3].size()));
    printToCoordinates(21, 34, txt[0]);
    printToCoordinates(22, 34, txt[1]);
    printToCoordinates(23, 34, txt[2]);
    printToCoordinates(24, 34, txt[3]);
}
