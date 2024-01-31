#include <SPI.h>
#include <Wire.h>
#include <U8x8lib.h>
#include <LIDARLite.h>
#include "pin.hpp"
#include "fan.hpp"

enum RobotState
{
    IDLE,
    DISCOVERING,
    POSTDISCO,
    CLEANING,
};

class Robot
{
public:
    Robot();
    void setup();
    void loop();
    void changeState(RobotState);
    RobotState currentState;

private:
    U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;
    LIDARLite myLidar;
    void setupSD();
    void setupFan();
    void setupGlobalPin();
    void setFanSpeed(uint32_t);
    void discover();
    int ping();
    void clean();
    void compute();
};
