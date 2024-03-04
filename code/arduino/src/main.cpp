#include "main.hpp"

volatile MotorMode motorMode;
volatile RequestMode requestMode;
volatile bool halt;
union
{
  float f;
  unsigned long l;
} angle;
Complex coord(0);

void Motor::setup(double Set_point = 80, bool pidmode = AUTOMATIC, uint8_t pidsampletime = 1)
{
  pinMode(motordir, OUTPUT); // L298P Control port settings DC motor driver board for the output mode
  pinMode(motordir, OUTPUT);
  pinMode(motorEN, OUTPUT);         //
  setpoint = Set_point;             // Set the output value of the PID
  pid.SetMode(pidmode);             // PID is set to automatic mode
  pid.SetSampleTime(pidsampletime); // Set PID sampling frequency is 100ms
  pid.SetOutputLimits(-255, 255);   // limits to safecast to int8 when abs is applied
  pinMode(encoderpinA, INPUT);
  pinMode(encoderpinB, INPUT);
  // setSpeed(255); // setpoint =80 et dir=1
}
void Motor::adjustPWM()
{
  if (halt || (long)output == 0)
  {
    digitalWrite(motorEN, LOW);
    return;
  }
  analogWrite(motorEN, (uint8_t)abs(output));
  setDirection(output > 0);
}
void Motor::loop()
{
  /*if (Setpoint)
  {
    Serial.println("setpoint");
    Serial.println(Setpoint);
    analogWrite(motorEN, val_output);
  }*/
  input = encoderValue;
  if (pid.Compute()) // PID conversion is complete and returns 1
  {
    Serial.println("---");
    Serial.println(output);
    Serial.print("Pluse: ");
    Serial.println(encoderValue);
    // encoderValue = 0; // Count clear, wait for the next count
    adjustPWM(); // modify duty cycle according to output
  }
}
// TODO: voir si en changeant le type d'interrupt de change a rising on peut "accelerer" l'isr
void Motor::interrupt()
{
  int MSB = digitalRead(encoderpinA); // MSB = most significant bit
  int LSB = digitalRead(encoderpinB); // LSB = least significant bit

  int encoded = (MSB << 1) | LSB;         // converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    encoderValue++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    encoderValue--;

  lastEncoded = encoded; // store this value for next time
}
void Motor::setDirection(bool goforward)
{
  digitalWrite(motordir, goforward);
  digitalWrite(motordirN, !goforward);
}

// target will be adjusted in the next loop call
void Motor::setConsigne(double newConsigne)
{
  if (motorMode == TRANSLATION)
  {
    setpoint = newConsigne;
    // setDirection(newSpeed >= 0); va etre appelé dans le adjustpwm
  }
}
double Motor::getInput()
{
  return (double)input;
}

bool Motor::isIdle()
{
  return (long)output == 0;
}

void Motor::resetCounters()
{
  if (motorMode == TRANSLATION)
  {
    input = 0;
    setpoint = 0;
  }
}
void Motor::adjustGlobalValues()
{
  if (input != encoderValue)
  {
    double d;
    if (motorMode == TRANSLATION)
    {
      d = (encoderValue - input) * onepulsedistance;
      coord += Complex(d * cos(angle.f), d * sin(angle.f));
    }
    if (motorMode == ROTATION)
    {
      d = (encoderValue - input) * onepulseangle;
      angle.f += d;
    }
  }
}
Motor motor0(encoder0pinA, encoder0pinB, motor0dir, motor0dirN, motor0EN);
Motor motor1(encoder1pinA, encoder1pinB, motor1dir, motor1dirN, motor1EN);

// rx event
void receiveEvent(int nbBytes)
{
  switch (nbBytes)
  {
  case 1:
  {
    uint8_t c = Wire.read();
    switch (c)
    {
    case 1:
    case 2:
      if (motorMode != c - 1)
      {
        motorMode = (MotorMode)(c - 1);
      }
    case 3:
      halt = true;
      return;
    case 4:
    case 5:
    case 6:
      if (requestMode != c - 4)
      {
        requestMode = (RequestMode)(c - 4);
      }
      return;
    case 7: // disable aux motors
      digitalWrite(fan0EN, HIGH);
      digitalWrite(motor2EN, LOW);
      return;
    case 8: // enable
      digitalWrite(fan0EN, LOW);
      digitalWrite(motor2EN, HIGH);
      return;
    default:
      return;
    }
  }
    return;
  case 2:
  {
    halt = false;
    uint16_t d = (Wire.read() << 8) | Wire.read();
    switch (motorMode)
    {
    case ROTATION:
      motor0.setConsigne(-(double)d);
      motor1.setConsigne(d);
      return;
    case TRANSLATION:
      motor0.setConsigne(d);
      motor1.setConsigne(d);
    default:
      return;
    }
  }
    return;
  default:
    return;
  }
}
// tx event
void requestEvent()
{
  switch (requestMode)
  {
  case ANGLE:
    Wire.write(angle.l);
  case DISTANCE:
  {
    float r = coord.real();
    float i = coord.imag();

    Wire.write(*(unsigned long *)&r);
    Wire.write(*(unsigned long *)&i);
  }
    return;
  default:
    Wire.write((motor0.isIdle() << 1) + motor1.isIdle());
    return;
  }
}

void setup()
{
  angle.f = 0.0F;
  TCCR1B = (TCCR1B & 0b11111000) | 1; // set 31KHz PWM to prevent motor noise
  halt = true;                        // default mode: all enable to low
  motorMode = MotorMode::TRANSLATION; // default mode: all enable to low
  requestMode = RequestMode::ISIDLE;  // do not respond on request bc halt
  pinMode(fan0EN, OUTPUT);
  pinMode(motor2EN, OUTPUT);
  digitalWrite(fan0EN, HIGH);
  digitalWrite(motor2EN, LOW);
  Serial.begin(9600);
  Wire.begin(0x42);
  Wire.onRequest(requestEvent); // Enregistrez l'événement
  Wire.onReceive(receiveEvent);
  motor0.setup();
  attachInterrupt(
      motor0.getApin(), []()
      { motor0.interrupt(); },
      CHANGE);
  attachPinChangeInterrupt(
      motor0.getBpin(), []()
      { motor0.interrupt(); },
      CHANGE);
  motor1.setup();
  attachInterrupt(
      motor1.getApin(), []()
      { motor1.interrupt(); },
      CHANGE);
  attachPinChangeInterrupt(
      motor1.getBpin(), []()
      { motor1.interrupt(); },
      CHANGE);
}

void loop()
{
  motor0.adjustGlobalValues();
  motor0.loop();
  motor1.loop();
}
