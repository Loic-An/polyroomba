#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#define encoder0pinA 2 // encoder0 A pin (interrupt pin 0)
#define encoder1pinA 3 // encoder1 A pin (interrupt pin 1)
#define encoder0pinB 4 // encoder0 B pin
#define encoder1pinB 5 // encoder1 B pin
#define motor0dir 6    // pin direction of motor0
#define motor1dir 7    // pin direction of motor1
#define fan0EN 8       // 12V pc fan enable pin
#define motor0EN 9     // ENABLE pin of motor0
#define motor1EN 10    // ENABLE pin of motor1
#define motor2EN 11    // ENABLE pin of remaining motors
#define Vin 12
#define Vratedmotor 6

class Motor
{
private:
    bool encoderPinALast; // last known state of encoder0pinA
    double duration;      // the number of pulses
    double val_output;    // Power supplied to the motor's EN pin.
    double Setpoint;      //
    static constexpr double Kp = 0.6;
    static constexpr double Ki = 5;
    static constexpr double Kd = 0; // PID constant
    bool Direction;                 // the rotation direction
    const uint8_t encoderpinA;      // encoder A pin (interrupt pin)
    const uint8_t encoderpinB;      // encoder B pin
    const uint8_t motordir;         // pin direction of motor
    const uint8_t motorEN;          // ENABLE pin of motor
    PID pid;

public:
    Motor(const uint8_t A, uint8_t B, uint8_t dir, uint8_t EN) : encoderpinA(A), encoderpinB(B), motordir(dir), motorEN(EN), pid(&duration, &val_output, &Setpoint, Kp, Ki, Kd, DIRECT){};
    void setup(double, bool, bool, uint8_t);
    void loop();
    void interrupt();
    uint8_t getApin() { return digitalPinToInterrupt(encoderpinA); }
    void setSpeed(uint8_t);
    uint8_t getSpeed();
};