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
    setupScreen();
    setupFan();
    setupSD();
    setupGlobalPin();
    setFanSpeed(0);
    setupInterrupts(*this);
    log_d("Setup done");
    u8x8.clear();
    u8x8.setCursor(0, 0);
    u8x8.print("Hello World!");
    recoverOldState();
    attachInterruptArg(
        D1, [](void *arg)
        {
            Robot *robot = static_cast<Robot *>(arg);
            robot->changeState(HOMING);
            log_d("interrupt D1"); },
        this, RISING);
}
void Robot::changeState(RobotState newState)
{
    if (newState != currentState)
    {
        switch (currentState)
        {
        default:
            break;
        }
        currentState = newState;
    }
    // u8x8.clearLine(1);
    u8x8.setCursor(0, 1);
    u8x8.print(RobotStateMessage[currentState]);
}
void Robot::recoverOldState()
{
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
void Robot::setupFan()
{
    // Prepare and set configuration of timers that will be used by LED Controller
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HS_MODE,         // timer mode
        .duty_resolution = DUTY_RESOLUTION, // resolution of PWM duty
        .timer_num = LEDC_HS_TIMER,         // timer index
        .freq_hz = PWM_FREQUENCY,           // frequency of PWM signal
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);
    // Set LED Controller with previously prepared configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num = LEDC_HS_CH0_GPIO,
        .speed_mode = LEDC_HS_MODE,
        .channel = LEDC_HS_CH0_CHANNEL,
        .timer_sel = LEDC_HS_TIMER,
        .duty = 0,
        .hpoint = 0};
    // Set the configuration
    ledc_channel_config(&ledc_channel);
}
void Robot::setFanSpeed(uint32_t speed)
{
    ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL, speed);
    ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL);
}
void Robot::setupGlobalPin()
{
    pinMode(D1, INPUT_PULLUP);
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
void Robot::setupInterrupts(Robot &instance)
{
}