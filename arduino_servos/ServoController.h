#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>
#include <Servo.h>
#include "Config.h"

class ServoController
{
private:
  Servo servoLeft;
  Servo servoRight;

  float smoothL = 90.0;
  float smoothR = 90.0;

  int lastPosL = 90;
  int lastPosR = 90;

public:
  void begin()
  {
    servoLeft.attach(Config::SERVO_LEFT_PIN, 500, 2500);
    servoRight.attach(Config::SERVO_RIGHT_PIN, 500, 2500);

    servoLeft.write(Config::CENTER_ANGLE);
    servoRight.write(Config::CENTER_ANGLE);
  }

  void update(int targetL, int targetR)
  {
    targetL = constrain(
      targetL,
      Config::MIN_SERVO_ANGLE,
      Config::MAX_SERVO_ANGLE
    );

    targetR = constrain(
      targetR,
      Config::MIN_SERVO_ANGLE,
      Config::MAX_SERVO_ANGLE
    );

    smoothL += (targetL - smoothL) * 0.25;
    smoothR += (targetR - smoothR) * 0.25;

    int outL = round(smoothL);
    int outR = round(smoothR);

    if (abs(outL - lastPosL) >= 3)
    {
      servoLeft.write(outL);
      lastPosL = outL;
    }

    if (abs(outR - lastPosR) >= 3)
    {
      servoRight.write(outR);
      lastPosR = outR;
    }
  }

  void centerWithOffset(float offset)
  {
    update(
      Config::CENTER_ANGLE + (int)offset,
      Config::CENTER_ANGLE - (int)offset
    );
  }
};

#endif