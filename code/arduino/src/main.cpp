#include "main.hpp"

void Motor::setup(double Set_point = 80, bool direction = true, bool pidmode = AUTOMATIC, uint8_t pidsampletime = 100)
{
  pinMode(motordir, OUTPUT); // L298P Control port settings DC motor driver board for the output mode
  pinMode(motorEN, OUTPUT);  //
  // Setpoint = Set_point;             // Set the output value of the PID
  pid.SetMode(pidmode);             // PID is set to automatic mode
  pid.SetSampleTime(pidsampletime); // Set PID sampling frequency is 100ms
  // Direction = true;                 // default -> Forward
  pid.SetOutputLimits(0, 255 * Vratedmotor / Vin);
  pinMode(encoderpinB, INPUT);

  setSpeed(161); // setpoint =80 et dir=1
}
void Motor::loop()
{
  if (Setpoint)
  {
    Serial.println("setpoint");
    Serial.println(Setpoint);
    analogWrite(motorEN, val_output);
  }
  if (pid.Compute()) // PID conversion is complete and returns 1
  {
    Serial.println("---");
    Serial.println(val_output);
    Serial.print("Pluse: ");
    Serial.println(duration);
    duration = 0; // Count clear, wait for the next count
  }
}
// TODO: voir si en changeant le type d'interrupt de change a rising on peut "accelerer" l'isr
void Motor::interrupt()
{
  bool Lstate = digitalRead(encoderpinA);
  if (!encoderPinALast && Lstate)
  {
    bool val = digitalRead(encoderpinB);
    if (!val && Direction)
    {
      Direction = false; // Reverse
    }
    else if (val && !Direction)
    {
      Direction = true; // Forward
    }
  }
  encoderPinALast = Lstate;
  duration++;
}

// speed will be adjusted with the next loop call
void Motor::setSpeed(uint8_t newSpeed)
{
  Direction = newSpeed & 1;
  Setpoint = newSpeed >> 1;
  digitalWrite(motordir, Direction);
}
uint8_t Motor::getSpeed()
{
  return ((int)val_output << 1) + Direction;
}
// Motor motor0(encoder0pinA, encoder0pinB, motor0dir, motor0EN);
Motor motor1(encoder1pinA, encoder1pinB, motor1dir, motor1EN);

void receiveEvent(int nbBytes)
{
  for (int i = 0; i < nbBytes; i++)
  {
    uint8_t c = Wire.read(); // Lisez un byte
    if (Wire.available())
    {
      long incomingData = Wire.parseInt();
      uint16_t motorData = incomingData & 0xFFFF;
      // motor0.setSpeed(motorData>>8);
      motor1.setSpeed(motorData & 0xFF);
    }
  }
}
void requestEvent()
{
  Wire.write(motor1.getSpeed()); //+(motor0.getSpeed()<<8));
}

void setup()
{
  Serial.begin(9600);
  Wire.begin(0x42);
  Wire.onRequest(requestEvent); // Enregistrez l'événement
  Wire.onReceive(receiveEvent);
  // motor0.setup();
  // attachInterrupt(motor0.getApin(), []() { motor0.interrupt(); },CHANGE);
  motor1.setup();
  attachInterrupt(
      motor1.getApin(), []()
      { motor1.interrupt(); },
      CHANGE);
}

void loop()
{
  // motor0.loop();
  motor1.loop();
}
