#include "Config.h"
#include "IBusReceiver.h"
#include "ServoMath.h"
#include "ServoController.h"
#include "FlightController.h"

IBusReceiver receiver;
ServoController servos;
FlightController flightController;

void setup()
{
  receiver.begin();
  servos.begin();
  flightController.begin();
}

void loop()
{
  IBusChannels channels = receiver.read();

  flightController.update(channels, servos);

  delay(4);
}