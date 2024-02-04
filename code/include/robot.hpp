#include <SPI.h>
#include <Wire.h>
#include <U8x8lib.h>
#include <LIDARLite.h>
#include "pin.hpp"
#include "fan.hpp"
#include <vector>
#include <SD.h>
#define BASE true

typedef std::array<short, 2> coord_t;

enum RobotState
{
    IDLING,
    HOMING,
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
    RobotState currentState = IDLING;
    void goHome();

private:
    coord_t coord = {0, 0};
    double angle = 0.0F;
    std::vector<coord_t> pointList;
    bool moving = false;
    U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;
    LIDARLite myLidar;
    static void setupInterrupts(Robot &instance);
    void recoverOldState();
    void setupSD();
    void setupFan();
    void setupGlobalPin();
    void setupScreen();
    void setFanSpeed(uint32_t);
    void discover();
    int ping();
    void spinAndPing();
    void clean();
    void compute();
};
