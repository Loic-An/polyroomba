#include "robot.hpp"
#include <SD.h>

Robot::Robot()
{
    U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
    LIDARLite myLidarLite;
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
    currentState = IDLE;
}
void Robot::setup()
{
    u8x8.setFlipMode(0);
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    setupFan();
    setupSD();
}
void Robot::changeState(RobotState newState) {}
void Robot::loop()
{
    u8x8.setCursor(0, 0);
    u8x8.print("Hello World!");
    u8x8.print(currentState);
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
    ledc_set_duty_and_update(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL, speed, 0);
}
void Robot::setupGlobalPin()
{
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    analogWrite(D0, 40);
    analogWrite(D5, 40);
}
void Robot::setupSD()
{
    if (!SD.begin(21))
    {
        log_e("Card Mount Failed");
    }
    uint8_t cardType = SD.cardType();
    switch (cardType)
    {
    case (CARD_NONE):
        log_w("No SD Card attached");
        break;
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
void Robot::discover()
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