#include "robot.hpp"

Robot robot;

void robotInterruptHandler()
{
    switch (robot.currentState)
    {
    case (DISCOVERING):
        robot.changeState(POSTDISCO);
        break;
    case (POSTDISCO):
        robot.changeState(CLEANING);
        break;
    case (CLEANING):
        robot.changeState(IDLING);
        break;
    case (HOMING):
        robot.changeState(DISCOVERING);
        break;
    default:
        robot.changeState(HOMING);
        break;
    }
    log_d("interrupt D1");
}
void setup()
{
    robot.setup();
    attachInterrupt(D1, robotInterruptHandler, RISING);
    delay(1000);
};

void loop()
{
    robot.loop();
}
