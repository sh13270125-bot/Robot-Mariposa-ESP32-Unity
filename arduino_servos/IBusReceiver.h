#ifndef IBUS_RECEIVER_H
#define IBUS_RECEIVER_H

#include <Arduino.h>
#include <IBusBM.h>
#include "Config.h"

struct IBusChannels
{
  bool connected;
  int steering;
  int elevator;
  int throttle;
  int ch5Trim;
  int rawCh6;
  int ch7Switch;
  int ch8Raw;
};

class IBusReceiver
{
private:
  IBusBM ibus;

  int lastChValues[8] =
  {
    1500, 1500, 1500, 1500,
    1500, 1500, 1500, 1500
  };

  int readFiltered(byte ch)
  {
    long sum = 0;

    for (int i = 0; i < 3; i++)
    {
      int value = ibus.readChannel(ch);

      if (value < 900 || value > 2100)
        value = 1500;

      sum += value;
    }

    int raw = sum / 3;

    if (abs(raw - lastChValues[ch]) > Config::DEADBAND)
      lastChValues[ch] = raw;

    return lastChValues[ch];
  }

public:
  void begin()
  {
    Serial.begin(115200);
    ibus.begin(Serial);
  }

  IBusChannels read()
  {
    IBusChannels channels;

    int ch3Raw = ibus.readChannel(2);

    channels.connected =
      (ch3Raw >= 1000 && ch3Raw <= 2000);

    channels.steering = readFiltered(0);
    channels.elevator = readFiltered(1);
    channels.throttle = readFiltered(2);
    channels.ch5Trim = readFiltered(4);
    channels.rawCh6 = readFiltered(5);
    channels.ch7Switch = readFiltered(6);
    channels.ch8Raw = readFiltered(7);

    return channels;
  }
};

#endif