#ifndef MOVEMENT_MATH_H
#define MOVEMENT_MATH_H

#include <Arduino.h>

class MovementMath
{
public:
  static float calculatePitch(int16_t ay, int16_t az)
  {
    return atan2(ay, az) * 180.0 / PI;
  }

  static float calculateRoll(int16_t ax, int16_t ay, int16_t az)
  {
    return atan2(
      -ax,
      sqrt((long)ay * ay + (long)az * az)
    ) * 180.0 / PI;
  }

  static float limitAngle(float angle)
  {
    return constrain(angle, -90, 90);
  }
};

#endif