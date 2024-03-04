#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#include <Complex.h>
#include "PinChangeInterrupt.h"
#define encoder0pinA 2 // encoder0 A pin (interrupt pin 0)
#define encoder1pinA 3 // encoder1 A pin (interrupt pin 1)
#define encoder0pinB 4 // encoder0 B pin
#define encoder1pinB 5 // encoder1 B pin
#define motor0dir 6    // pin direction of motor0
#define motor1dir 7    // pin direction of motor1
#define fan0EN 8       // 12V pc fan enable pin (on/off only)
#define motor0EN 9     // ENABLE pin of motor0
#define motor1EN 10    // ENABLE pin of motor1
#define motor2EN 11    // ENABLE pin of remaining motors
#define motor0dirN 12  // pin reverse direction of motor0
#define motor1dirN 13  // pin reverse direction of motor1
#define Vin 5
#define Vratedmotor 6
#define encoderpulse 960
#define wheeldiameter 70
#define wheelposdiameter 255
#define oneroundangle M_PI * 2 * wheeldiameter / wheelposdiameter
#define onepulsedistance M_PI *wheeldiameter / encoderpulse
#define onepulseangle oneroundangle / encoderpulse

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
class Motor
{
private:
    double input;                 // the number of pulses
    double output;                // Power supplied to the motor's EN pin.
    double setpoint;              //
    volatile int lastEncoded = 0; // Here updated value of encoder store.
    volatile long encoderValue = 0;
    static constexpr double Kp = 0.6;
    static constexpr double Ki = 5;
    static constexpr double Kd = 0; // PID constant
    const uint8_t encoderpinA;      // encoder A pin (interrupt pin)
    const uint8_t encoderpinB;      // encoder B pin
    const uint8_t motordir;         // pin direction of motor
    const uint8_t motordirN;        // pin reverse direction of motor
    const uint8_t motorEN;          // ENABLE pin of motor
    PID pid;

public:
    Motor(const uint8_t A, uint8_t B, uint8_t dir, uint8_t dirN, uint8_t EN) : encoderpinA(A), encoderpinB(B), motordir(dir), motordirN(dirN), motorEN(EN), pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT){};
    void setup(double, bool, uint8_t);
    void loop();
    void interrupt();
    void adjustPWM();
    bool isIdle();
    uint8_t getApin() { return digitalPinToInterrupt(encoderpinA); }
    uint8_t getBpin() { return digitalPinToPinChangeInterrupt(encoderpinB); };
    void setConsigne(double);
    double getInput();
    void resetCounters();
    void adjustGlobalValues();
    void setDirection(bool);
};