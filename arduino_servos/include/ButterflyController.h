#ifndef BUTTERFLY_CONTROLLER_H
#define BUTTERFLY_CONTROLLER_H

#include "MotionData.h"

class ButterflyController
{
private:
  float deadZone;

public:
  ButterflyController(float movementDeadZone)
  {
    deadZone = movementDeadZone;
  }

  MotionData applyDeadZone(MotionData data)
  {
    if (abs(data.pitch) < deadZone)
      data.pitch = 0;

    if (abs(data.roll) < deadZone)
      data.roll = 0;

    return data;
  }
};

#endif