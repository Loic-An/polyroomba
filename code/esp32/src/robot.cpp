#include "robot.hpp"

const char *RobotStateMessage[5] = {"IDLING", "GOING HOME", "DISCOVERING", "POSTPROCESSING", "CLEANING"};

Robot::Robot()
{
    U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE, SCL, SDA);
    LIDARLite myLidarLite;

    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
}
void Robot::setup()
{
    setupSD();
    setupGlobalPin();

    Wire.begin(I2C_ADDR_SELF);
    Wire.setClock(I2C_FREQ);

    setupMotors();
    log_d("Setup done");
    /*
    u8x8.clear();
    u8x8.setCursor(0, 0);
    u8x8.print("Hello World!");*/
    recoverOldState();
}
void Robot::changeState(RobotState newState)
{
    if (newState != currentState)
    {
        if (newState == DISCOVERING && currentState == IDLING)
        {
            if (BASE)
            {
                move(10);
            }
        }
        currentState = newState;
    }
    // u8x8.clearLine(1);
    // u8x8.setCursor(0, 1);
    // u8x8.print(RobotStateMessage[currentState]);
}
// will travel distance centimeter, unless sensor detect obstacle
void Robot::move(uint16_t distance)
{
    coord_t target = coord;
    setMotorsSpeed(0b11000000, 0b11000000);
    while ()
}
void Robot::recoverOldState()
{
    // TODO: stocker l'etat (sur une carte sd? sur la flash ?)
    //  besoin de stocker la position et l'angle
    //  besoin de stocker aussi en permanent les points ?
    //
    changeState(IDLING);
}
void Robot::loop()
{
    switch (currentState)
    {
    case (DISCOVERING):
        discover();
        return;
    case (POSTDISCO):
        compute();
        return;
    case (CLEANING):
        clean();
        return;
    case (HOMING):
        goHome();
        return;
    default:
        return;
    }
}
void Robot::setupGlobalPin()
{
    pinMode(D1, INPUT_PULLUP);
    attachInterruptArg(
        D1, [](void *arg)
        {
            Robot *robot = static_cast<Robot *>(arg);
            robot->changeState(HOMING);
            log_d("interrupt D1"); },
        this, RISING);
}
void Robot::setupMotors()
{
    Wire.beginTransmission(I2C_ADDR_UNO);
    log_i("awaiting for write availability on %d...", I2C_ADDR_UNO);
    while (!Wire.availableForWrite())
    {
        Wire.write(0b10000000000000000000000000000000L);
    }

    Wire.endTransmission();
    setMotorsSpeed(0b11000000, 0b11000000);
}
void Robot::setMotorsSpeed(uint8_t newSpeed0, uint8_t newSpeed1)
{
    Wire.beginTransmission(I2C_ADDR_UNO);
}
void Robot::setupSD()
{
    if (!SD.begin(21))
    {
        log_e("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
    switch (cardType)
    {
    case (CARD_NONE):
        log_w("No SD Card attached");
        return;
    case (CARD_MMC):
        log_i("SD Card Type: MMC");
        break;
    case (CARD_SD):
        log_i("SD Card Type: SDSC");
        break;
    case (CARD_SDHC):
        log_i("SD Card Type: SDHC");
        break;
    default:
        log_w("SD Card Type: UNKNOWN");
        break;
    }
    log_i("SD Card Size: %" PRIu64 " BYTES", SD.cardSize());
}
void Robot::goHome()
{
}
void Robot::discover()
{
}
void Robot::spinAndPing()
{
}
int Robot::ping()
{
    return myLidar.distance();
}
void Robot::clean()
{
}
void Robot::compute()
{
}
void Robot::setupScreen()
{
    if (!u8x8.begin())
    {
        log_e("Screen not available");
        while (true)
        {
        };
    };
    u8x8.setFlipMode(0);
    u8x8.setFont(u8x8_font_chroma48medium8_r);
}