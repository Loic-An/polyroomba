#include <SD.h>
#include <SPI.h>
#include "pin.hpp"
#include <Wire.h>
#include <LIDARLite.h>

LIDARLite myLidarLite;
Sd2Card sd;
SdVolume vol;
boolean isSdInit = false;

void set_fan_speed(uint16_t val)
{
  OCR1A = (val > 320) ? 320 : val;
}

void set_mot1_forward(bool dir)
{
  digitalWrite(PIN_MOT1_DIR1, !dir);
  digitalWrite(PIN_MOT1_DIR2, dir);
}

void set_mot2_forward(bool dir)
{
  digitalWrite(PIN_MOT2_DIR1, dir);
  digitalWrite(PIN_MOT2_DIR2, !dir);
}

void setup()
{
  // Serial.begin(115200); // Initialize serial connection
  // Serial.println("Serial started.");

  // if (!sd.init())
  // {
  //   Serial.println("sd card not initialized. Halting...");
  // }
  // else
  // {
  //   Serial.println("sd card initialized.");
  //   isSdInit = true;
  // };

  // myLidarLite.begin(1, true);
  // Serial.println("Lidar started.");

  pinMode(PIN_SONAR_TRIG, OUTPUT);
  pinMode(PIN_MOT1_DIR1, OUTPUT);
  pinMode(PIN_MOT2_DIR1, OUTPUT);
  pinMode(PIN_MOT1_DIR2, OUTPUT);
  pinMode(PIN_MOT2_DIR2, OUTPUT);
  pinMode(PIN_MOT1_EN, OUTPUT);
  pinMode(PIN_MOT2_EN, OUTPUT);
  pinMode(PIN_SONAR_ECHU, INPUT);
  pinMode(PIN_SONAR_ECHR, INPUT);
  pinMode(PIN_SONAR_ECHL, INPUT);

  // Configuration du Timer 1 pour une fréquence PWM de 25kHz sur PIN_FAN_PWM (9)
  TCCR1A = 0;                             // Effacer le registre TCCR1A
  TCCR1A |= (1 << COM1A1) | (1 << WGM11); // Non inversé, mode 10 (phase correcte)
  TCCR1B = 0;                             // Effacer le registre TCCR1B
  TCCR1B |= (1 << WGM13) | (1 << CS10);   // Mode 10 (phase correcte), pas de pré-diviseur
  ICR1 = 320;                             // Valeur TOP pour une fréquence de 25kHz
  pinMode(PIN_FAN_PWM, OUTPUT);           // Mettre le pin 9 en sortie
  set_fan_speed(0);
  // Serial.println("FAN PWM setup done.");

  // Serial.println("PINs setup done.");

  set_mot1_forward(true);
  set_mot2_forward(true);

  digitalWrite(PIN_MOT1_EN, HIGH);
  digitalWrite(PIN_MOT2_EN, HIGH);

  // Serial.println("SETUP DONE. ENTERING LOOP...");
}

void loop()
{

  // Serial.println(myLidarLite.distance());
}
