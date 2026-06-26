#ifndef SERVO_MATH_H
#define SERVO_MATH_H

#include <Arduino.h>
#include "Config.h"

class ServoMath
{
private:
  float phase = 0;
  float perchPhase = 0;
  float currentOffset = 0;
  bool wasFailsafe = true;

public:
  void updateSine(int throttle)
  {
    if (wasFailsafe)
    {
      phase = PI;
      wasFailsafe = false;
    }

    float speedFactor =
      map(throttle, 1050, 2000, 60, 100) / 1000.0;

    phase += speedFactor;

    if (phase >= (2 * PI))
      phase -= (2 * PI);

    currentOffset =
      sin(phase) * Config::FLAP_AMPLITUDE;
  }

  void updatePerch(int rawCh6)
  {
    wasFailsafe = true;

    float maxUpAngle =
      map(rawCh6, 1000, 2000, 30, 60);

    perchPhase += 0.01;

    float goalAngle =
      maxUpAngle +
      (sin(perchPhase) * 20.0) -
      20.0;

    if (abs(currentOffset - goalAngle) > 0.5)
    {
      currentOffset +=
        (currentOffset < goalAngle) ? 1.5 : -1.5;
    }
  }

  void updateGlide()
  {
    wasFailsafe = true;

    if (currentOffset < 0)
      currentOffset += 2.0;
    else if (currentOffset > 0)
      currentOffset -= 2.0;

    if (abs(currentOffset) < 2.5)
      currentOffset = 0;
  }

  void updateFailsafe()
  {
    if (currentOffset < 0)
      currentOffset += 1.5;
    else if (currentOffset > 0)
      currentOffset -= 1.5;

    if (abs(currentOffset) < 1.5)
      currentOffset = 0;

    wasFailsafe = true;
  }

  float getOffset()
  {
    return currentOffset;
  }
};

#endif