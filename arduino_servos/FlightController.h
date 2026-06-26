#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include <Arduino.h>
#include "Config.h"
#include "IBusReceiver.h"
#include "ServoMath.h"
#include "ServoController.h"

class FlightController
{
private:
  ServoMath servoMath;

  bool isPerchingState = false;
  int ledDebounceCounter = 0;

public:
  void begin()
  {
    pinMode(Config::LED_PIN, OUTPUT);
    digitalWrite(Config::LED_PIN, LOW);
  }

  void update(IBusChannels channels, ServoController &servos)
  {
    if (!channels.connected)
    {
      digitalWrite(Config::LED_PIN, LOW);

      servoMath.updateFailsafe();

      servos.centerWithOffset(
        servoMath.getOffset()
      );

      return;
    }

    updateLed(channels.ch8Raw);

    int steerValue =
      (abs(channels.steering - 1500) > 25)
      ? map(channels.steering, 1000, 2000, -15, 15)
      : 0;

    int finalSteer =
      steerValue +
      map(channels.ch5Trim, 1000, 2000, -15, 15);

    int elevOffset =
      (abs(channels.elevator - 1500) > 25)
      ? map(channels.elevator, 1000, 2000, -15, 15)
      : 0;

    int pitchTrim =
      map(
        channels.rawCh6,
        1000,
        2000,
        -Config::MAX_PITCH_TRIM,
         Config::MAX_PITCH_TRIM
      );

    updateFlightMode(channels);

    if (isPerchingState)
      servoMath.updatePerch(channels.rawCh6);
    else if (channels.throttle < 1050)
      servoMath.updateGlide();
    else
      servoMath.updateSine(channels.throttle);

    float offset = servoMath.getOffset();

    int outL =
      Config::CENTER_ANGLE +
      (int)offset +
      finalSteer +
      elevOffset +
      pitchTrim;

    int outR =
      Config::CENTER_ANGLE -
      (int)offset +
      finalSteer -
      elevOffset -
      pitchTrim;

    servos.update(outL, outR);
  }

private:
  void updateLed(int ch8Raw)
  {
    if (ch8Raw > 1550)
    {
      if (ledDebounceCounter < Config::LED_THRESHOLD)
        ledDebounceCounter++;
    }
    else if (ch8Raw < 1450)
    {
      if (ledDebounceCounter > 0)
        ledDebounceCounter--;
    }

    digitalWrite(
      Config::LED_PIN,
      ledDebounceCounter >= Config::LED_THRESHOLD
    );
  }

  void updateFlightMode(IBusChannels channels)
  {
    if (channels.throttle > 1100)
      isPerchingState = false;
    else if (channels.ch7Switch > 1600 && channels.throttle < 1100)
      isPerchingState = true;

    if (channels.ch7Switch < 1400)
      isPerchingState = false;
  }
};

#endif