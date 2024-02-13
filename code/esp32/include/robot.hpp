#include <SPI.h>
#include <Wire.h>
#include <U8x8lib.h>
#include <LIDARLite.h>
#include "pin.hpp"
#include <vector>
#include <SD.h>
#include "coord.hpp"
#define I2C_ADDR_UNO 0x42
#define I2C_ADDR_SELF 0x52
#define I2C_ADDR_LIDAR 0x62
#define I2C_FREQ 400000U
#define BASE true


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
    volatile RobotState currentState = IDLING;
    void goHome();

private:
    coord_t coord = {0, 0};
    double angle = 0.0F;
    std::vector<coord_t> pointList;
    bool moving = false;
    U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;
    LIDARLite myLidar;
    void recoverOldState();
    void setupSD();
    void setMotorsSpeed(uint8_t, uint8_t);
    void setupMotors();
    void move(uint16_t distance);
    void setupGlobalPin();
    void setupScreen();
    void discover();
    int ping();
    void spinAndPing();
    void clean();
    void compute();
};
