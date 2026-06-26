#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace Config
{
  const byte LED_PIN = 6;

  const int SERVO_LEFT_PIN = 9;
  const int SERVO_RIGHT_PIN = 10;

  const int CENTER_ANGLE = 90;
  const int FLAP_AMPLITUDE = 65;
  const int MAX_PITCH_TRIM = 20;

  const int MIN_SERVO_ANGLE = 20;
  const int MAX_SERVO_ANGLE = 160;

  const int DEADBAND = 10;
  const int LED_THRESHOLD = 5;
}

#endif