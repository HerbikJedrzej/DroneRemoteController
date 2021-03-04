#include <gtest/gtest.h>
#include <ServoStructure.hh>
#include <cstdlib>

#define AMOUNT_OF_CELLS 10
#define AMOUNT_OF_TEST 10

namespace servoTest{
    void setTest(Servos& servos, uint16_t* testValues){
        for(unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
            servos.set(i, testValues[i]);
    }
    void doTest(uint16_t* orginalVariables, uint16_t* testValues){
        for(unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
            EXPECT_EQ(orginalVariables[i], testValues[i]);
        }
}

TEST(ServoStructure, Servos){
    uint16_t variable[AMOUNT_OF_CELLS];
    Servos servos(variable, AMOUNT_OF_CELLS);
    EXPECT_EQ(servos.size(), AMOUNT_OF_CELLS);
    for(unsigned int i = 0; i < AMOUNT_OF_TEST; i++){
        uint16_t data[AMOUNT_OF_CELLS];
        for(unsigned int j = 0; j < AMOUNT_OF_CELLS; j++)
            data[j] = (uint16_t)std::rand();
        servoTest::setTest(servos, data);
        servoTest::doTest(variable, data);
    }    
}