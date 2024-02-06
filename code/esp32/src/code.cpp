#include "robot.hpp"

Robot robot;

void robotInterruptHandler()
{
    robot.changeState(HOMING);
}
void setup()
{
    robot.setup();
    attachInterrupt(
        D1, []()
        {robot.changeState(HOMING);log_d("interrupt D1"); },
        RISING);
    delay(1000);
};

void loop()
{
    robot.loop();
}
