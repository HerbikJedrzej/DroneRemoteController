#include <Visualizer.hh>
#include <RadioParser.hh>

class KeyboardInterpreter{
    Visualizer& visual;
    Drivers::RadioParser* const radioParser;
    uint8_t* coordinates;
    bool& triggerButton;
public:
    KeyboardInterpreter(Visualizer& _visual, Drivers::RadioParser* const _radioParser, uint8_t _coordinates[4], bool& _triggerButton);
    ~KeyboardInterpreter();
    void run(const unsigned int& key);
};
