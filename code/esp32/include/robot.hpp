#include <SPI.h>
#include <Wire.h>
#include <LIDARLite.h>
#include "pin.hpp"
#include <vector>
#include <sstream>
#include <string>
#include <SD.h>
#define I2C_ADDR_UNO 0x42
#define I2C_ADDR_SELF 0x52
#define I2C_ADDR_LIDAR 0x62
#define I2C_FREQ 400000U
#define BASE false
#define LIDAROFFSET 5
#define SPINANDPINGANGLE 20

enum RobotState
{
    IDLING,
    HOMING,
    DISCOVERING,
    POSTDISCO,
    CLEANING,
};

enum MotorMode
{
    ROTATION,
    TRANSLATION
};
enum RequestMode
{
    ISIDLE,
    ANGLE,
    DISTANCE,
};