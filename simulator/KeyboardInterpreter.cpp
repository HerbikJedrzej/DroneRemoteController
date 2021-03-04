#include "KeyboardInterpreter.h"
#include <stdexcept>
#include <string>

KeyboardInterpreter::KeyboardInterpreter(Visualizer& _visual, Drivers::RadioParser* const _radioParser, uint8_t _coordinates[4], bool& _triggerButton):
    visual(_visual),
    radioParser(_radioParser),
    coordinates(_coordinates),
    triggerButton(_triggerButton){}

KeyboardInterpreter::~KeyboardInterpreter(){
}

void KeyboardInterpreter::run(const unsigned int& key){
    triggerButton = false;
    switch(key){
        case 0:
            return;
        case ']':
            return;
        case '[':
            return;
        case 'f':
            visual.Down();
            break;
        case 'h':
            visual.Up();
            break;
        case 'g':
            visual.Cancel();
            break;
        case 'e':
            visual.Ok();
            break;
        case 'u':
            visual.Change();
            break;
        case 'q':
            radioParser->setJoyLeftButtonOption();
            break;
        case 'o':
            radioParser->setJoyRightButtonOption();
            triggerButton = true;
            break;
        case '1':
            radioParser->setEngineOnOption(true);
            break;
        case '2':
            radioParser->setFlyOnOption(true);
            break;
        case '3':
            radioParser->setSpecialButtonOption();
            break;
        case '4':
            radioParser->setEngineOnOption(false);
            break;
        case 'a':
            coordinates[2]--;
            break;
        case 'w':
            coordinates[3]++;
            break;
        case 's':
            coordinates[3]--;
            break;
        case 'd':
            coordinates[2]++;
            break;
        case 'j':
            coordinates[0]--;
            break;
        case 'i':
            coordinates[1]++;
            break;
        case 'k':
            coordinates[1]--;
            break;
        case 'l':
            coordinates[0]++;
            break;
        default:
            throw std::invalid_argument("Unknown char input = " + std::to_string(key));
    }
}
