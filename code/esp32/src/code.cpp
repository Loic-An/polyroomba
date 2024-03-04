#include "robot.hpp"
#include <complex>

typedef std::complex<float> coord_t;

const char *RobotStateMessage[5] = {"IDLING", "GOING HOME", "DISCOVERING", "POSTPROCESSING", "CLEANING"};
LIDARLite myLidarLite;
volatile RobotState currentState = IDLING;
coord_t coord(0);
double angle = 0.0F;
std::vector<coord_t> pointsList;
bool moving = false;
volatile bool b1 = true;

// Returns  if one point doesn't have 2 adjacents points nearby
int *checkPointDistance()
{
    static int r[2];
    int k;
    int min;
    int calc;
    for (int i = 0; i < pointsList.size(); i++)
    {
        k = 0;
        min = 30;
        for (int j = 0; j < pointsList.size(); j++)
        {
            if (i != j)
            {
                calc = abs(pointsList[i] - pointsList[j]);
                if (calc < min)
                {
                    min = j;
                }
                if (calc < 30)
                {
                    k++;
                }
            }
        }
        if (2 > k)
        {
            r[0] = i;
            r[1] = min;
            return r;
        }
    }
    return r;
}

void setupSD()
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
long requestFromArduino()
{
    long r;
    uint8_t t = Wire.requestFrom(I2C_ADDR_UNO, 4);
    while (Wire.available())
    {
        r = r << 8 + Wire.read();
    }
    return r;
}
int ping()
{
    return myLidarLite.distance() - LIDAROFFSET;
}
void sendHaltSignal()
{
    Wire.beginTransmission(I2C_ADDR_UNO);
    Wire.write(3);
    Wire.endTransmission();
}
void delayUntillIdle(bool watchdog)
{
    for (;;)
    {
        if (watchdog)
        {
            if (ping() < 10)
            {
                sendHaltSignal();
            }
        }
        delay(1000);
        Wire.requestFrom(I2C_ADDR_UNO, 1);
        if (Wire.read() > 2)
        {
            return;
        };
    }
}
void setMotorMode(MotorMode motorMode)
{
    Wire.beginTransmission(I2C_ADDR_UNO);
    Wire.write(motorMode + 1);
    Wire.endTransmission();
}
void setRequestMode(RequestMode requestMode)
{
    Wire.beginTransmission(I2C_ADDR_UNO);
    Wire.write(requestMode + 4);
    Wire.endTransmission();
}

// will travel distance centimeter, unless sensor detect obstacle
void move(uint16_t distance, bool wait = true)
{
    coord_t target = coord + coord_t(distance * cos(angle), distance * sin(angle));

    setMotorMode(TRANSLATION);
    setRequestMode(ISIDLE);
    Wire.beginTransmission(I2C_ADDR_UNO);
    Wire.write(distance);
    Wire.endTransmission();
    if (wait)
    {
        delayUntillIdle(true);
        setRequestMode(DISTANCE);
        Wire.requestFrom(I2C_ADDR_UNO, 8);
        while (Wire.available() != 8)
        {
            log_e("cannot retrieve coord from arduino");
        };
        coord.real(Wire.read() << 24 | Wire.read() << 16 | Wire.read() << 8 | Wire.read());
        coord.imag(Wire.read() << 24 | Wire.read() << 16 | Wire.read() << 8 | Wire.read());
    }
}
float getanglefromarduino()
{
    Wire.beginTransmission(I2C_ADDR_UNO);
    Wire.write(5);
    Wire.endTransmission();
    Wire.requestFrom(I2C_ADDR_UNO, 4);
    while (Wire.available() != 4)
    {
        log_e("cannot retrieve angle from arduino");
    };
    unsigned long real = Wire.read() << 24 | Wire.read() << 16 | Wire.read() << 8 | Wire.read();
    return (*(float *)&real);
}
// abs angle
void rotate(uint16_t angle, bool wait = true)
{
    setMotorMode(ROTATION);
    setRequestMode(ISIDLE);
    Wire.beginTransmission(I2C_ADDR_UNO);
    Wire.write(angle);
    Wire.endTransmission();
    if (wait)
    {
        delayUntillIdle(false);
        angle = getanglefromarduino();
    }
}
void toggleAuxMotors(bool enable)
{
    Wire.beginTransmission(I2C_ADDR_UNO);
    Wire.write(7 + enable);
    Wire.endTransmission();
}
void spinAndPing()
{
    double init = angle;
    rotate(angle + 360, false);
    setRequestMode(ANGLE);
    while (angle - init < 360 - SPINANDPINGANGLE)
    {
        pointsList.push_back(coord + std::polar((float)ping(), (float)angle));
        while (getanglefromarduino() - angle < SPINANDPINGANGLE)
        {
            delay(20);
        }
        angle = getanglefromarduino();
    }
    setRequestMode(ISIDLE);
    delayUntillIdle(false);
}

void changeState(RobotState newState)
{
    if (newState != currentState)
    {
        toggleAuxMotors(newState == CLEANING);
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
void goHome()
{
    rotate((uint16_t)((180 + (long)angle) % 360));
    move(abs(coord));
}
void discover()
{
    spinAndPing();
    int *checkptr = checkPointDistance();

    if (checkptr[0] != 0 || checkptr[1] != 0)
    {
        coord_t temp = (pointsList.at(checkptr[1]) + pointsList.at(checkptr[0]));
        rotate((uint16_t)arg(temp), true);
        move(abs(temp) - 30);
    }
    else
    {
        changeState(POSTDISCO);
    }
}

void clean()
{
}
void compute()
{
}

void setupGlobalPin()
{
    pinMode(D1, INPUT_PULLUP);
    attachInterrupt(
        D1, []()
        {
            b1=true;
            log_d("interrupt D1"); },
        RISING);
}
void recoverOldState()
{
    // TODO: stocker l'etat (sur une carte sd? sur la flash ?)
    //  besoin de stocker la position et l'angle
    //  besoin de stocker aussi en permanent les points ?
    //
    changeState(IDLING);
}

void setup()
{
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
    setupSD();
    setupGlobalPin();

    Wire.begin(I2C_ADDR_SELF);
    Wire.setClock(I2C_FREQ);

    /*setupMotors();*/
    log_d("Setup done");
    /*
    u8x8.clear();
    u8x8.setCursor(0, 0);
    u8x8.print("Hello World!");*/
    recoverOldState();
    delay(1000);
};

bool dumpMap()
{
    if (SD.exists("map.2d"))
    {
        SD.remove("map.2d.old");
        SD.rename("map.2d", "map.2d.old");
    }
    File file = SD.open("map.2d", FILE_WRITE);
    if (!file)
    {
        return false;
    }
    for (int i = 0; i < pointsList.size(); i++)
    {
        file.printf("%F %F\n", pointsList.at(i).real(), pointsList.at(i).imag());
    }
    file.close();
    return true;
}

bool loadMap()
{
    if (!SD.exists("map.2d"))
    {
        return false;
    }
    pointsList.clear();
    File file = SD.open("map.2d", FILE_WRITE);
    if (!file)
    {
        return false;
    }
    String parsing;
    int spaceindex;
    while (file.available())
    {
        parsing = file.readStringUntil('\n');
        spaceindex = parsing.indexOf(' ');
        if (spaceindex < 0)
        {
            log_e("error while parsing map.2d");
            log_e("%s", parsing);
            return false;
        }
        std::istringstream iss1(parsing.substring(0, spaceindex).c_str());
        std::istringstream iss2(parsing.substring(spaceindex + 1).c_str());
        float real;
        float imag;
        iss1 >> real;
        iss2 >> imag;
    }
    return true;
}

void loop()
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
        if (b1)
        {
            if (loadMap())
            {
                changeState(CLEANING);
            }
            else
            {
                changeState(DISCOVERING);
            }
        }
        return;
    }
}
