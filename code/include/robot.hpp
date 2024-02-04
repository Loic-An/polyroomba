#include <SPI.h>
#include <Wire.h>
#include <U8x8lib.h>
#include <LIDARLite.h>
#include "pin.hpp"
#include "fan.hpp"
#include <vector>
#include <SD.h>

enum RobotState
{
    IDLING,
    HOMING,
    DISCOVERING,
    POSTDISCO,
    CLEANING,
};
const char *StateMessage[5] = {"IDLING", "GOING HOME", "DISCOVERING", "POSTPROCESSING", "CLEANING"};

class Robot
{
public:
    Robot();
    void setup();
    void loop();
    void changeState(RobotState);
    RobotState currentState = IDLING;
    void goHome();

private:
    std::array<short, 2> coord = {0, 0};
    double angle = 0;
    std::vector<std::array<short, 2>> pointList;
    bool moving = false;
    U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;
    LIDARLite myLidar;
    void setupSD();
    void setupFan();
    void setupGlobalPin();
    void setFanSpeed(uint32_t);
    void discover();
    int ping();
    void spinAndPing();
    void clean();
    void compute();
};
