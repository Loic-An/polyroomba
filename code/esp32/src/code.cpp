#include "robot.hpp"

Robot robot;

void robotInterruptHandler()
{
    robot.changeState(HOMING);
}
void setup()
{
    robot.setup();
    delay(1000);
};

void loop()
{
    robot.loop();
}
