#ifndef IMU_SENSOR_H
#define IMU_SENSOR_H

#include <Wire.h>
#include <MPU6050.h>
#include "MotionData.h"

class IMUSensor
{
private:
  MPU6050 mpu;

public:
  void begin()
  {
    Wire.begin();
    mpu.initialize();
  }

  bool isConnected()
  {
    return mpu.testConnection();
  }

  MotionData readMotion()
  {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    MotionData data;

    data.pitch = atan2(ay, az) * 180 / PI;

    data.roll = atan2(
      -ax,
      sqrt(ay * ay + az * az)
    ) * 180 / PI;

    data.pitch = constrain(data.pitch, -90, 90);
    data.roll = constrain(data.roll, -90, 90);

    return data;
  }
};

#endif