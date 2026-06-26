#include <Servo.h>
#include <IBusBM.h>

IBusBM ibus;

const byte ledPin = 6;

Servo servoLeft;
Servo servoRight;

const int centerAngle = 90;
const int flapAmplitude = 65;
const int maxPitchTrim = 20;

float phase = 0;
float perchPhase = 0;
float currentOffset = 0;

float smoothL = 90.0;
float smoothR = 90.0;

bool isPerchingState = false;
bool wasFailsafe = true;

int lastPosL = 90;
int lastPosR = 90;

int lastChValues[8] =
{
  1500, 1500, 1500, 1500,
  1500, 1500, 1500, 1500
};

const int deadband = 10;

int ledDebounceCounter = 0;
const int ledThreshold = 5;

void setup()
{
  // En Arduino Pro Mini solo existe Serial.
  // El receptor iBUS debe ir conectado al pin RX.
  Serial.begin(115200);
  ibus.begin(Serial);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  servoLeft.attach(9, 500, 2500);
  servoRight.attach(10, 500, 2500);

  servoLeft.write(centerAngle);
  servoRight.write(centerAngle);
}

int readIBUSFiltered(byte ch)
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

  if (abs(raw - lastChValues[ch]) > deadband)
    lastChValues[ch] = raw;

  return lastChValues[ch];
}

void loop()
{
  int ch3Raw = ibus.readChannel(2);

  bool isConnected =
    (ch3Raw >= 1000 && ch3Raw <= 2000);

  if (!isConnected)
  {
    digitalWrite(ledPin, LOW);
    processFailsafe();
  }
  else
  {
    int steering  = readIBUSFiltered(0);
    int elevator  = readIBUSFiltered(1);
    int throttle  = readIBUSFiltered(2);
    int ch5Trim   = readIBUSFiltered(4);
    int rawCh6    = readIBUSFiltered(5);
    int ch7Switch = readIBUSFiltered(6);
    int ch8Raw    = readIBUSFiltered(7);

    if (ch8Raw > 1550)
    {
      if (ledDebounceCounter < ledThreshold)
        ledDebounceCounter++;
    }
    else if (ch8Raw < 1450)
    {
      if (ledDebounceCounter > 0)
        ledDebounceCounter--;
    }

    digitalWrite(
      ledPin,
      ledDebounceCounter >= ledThreshold
    );

    int steerValue =
      (abs(steering - 1500) > 25)
      ? map(steering, 1000, 2000, -15, 15)
      : 0;

    int finalSteer =
      steerValue +
      map(ch5Trim, 1000, 2000, -15, 15);

    int elevOffset =
      (abs(elevator - 1500) > 25)
      ? map(elevator, 1000, 2000, -15, 15)
      : 0;

    int pitchTrim =
      map(
        rawCh6,
        1000,
        2000,
        -maxPitchTrim,
         maxPitchTrim
      );

    if (throttle > 1100)
      isPerchingState = false;
    else if (ch7Switch > 1600 && throttle < 1100)
      isPerchingState = true;

    if (ch7Switch < 1400)
      isPerchingState = false;

    if (isPerchingState)
      updateCurrentOffsetPerch(rawCh6);
    else if (throttle < 1050)
      updateCurrentOffsetGlide();
    else
      updateCurrentOffsetSine(throttle);

    int outL =
      centerAngle +
      (int)currentOffset +
      finalSteer +
      elevOffset +
      pitchTrim;

    int outR =
      centerAngle -
      (int)currentOffset +
      finalSteer -
      elevOffset -
      pitchTrim;

    updateServos(outL, outR);
  }

  delay(4);
}

void updateCurrentOffsetSine(int throttle)
{
  if (wasFailsafe)
  {
    phase = PI;
    wasFailsafe = false;
  }

  float speedFactor =
    map(
      throttle,
      1050,
      2000,
      60,
      100
    ) / 1000.0;

  phase += speedFactor;

  if (phase >= (2 * PI))
    phase -= (2 * PI);

  currentOffset =
    sin(phase) * flapAmplitude;
}

void updateCurrentOffsetPerch(int rawCh6)
{
  wasFailsafe = true;

  float maxUpAngle =
    map(
      rawCh6,
      1000,
      2000,
      30,
      60
    );

  perchPhase += 0.01;

  float goalAngle =
    maxUpAngle +
    (sin(perchPhase) * 20.0) -
    20.0;

  if (abs(currentOffset - goalAngle) > 0.5)
  {
    currentOffset +=
      (currentOffset < goalAngle)
      ? 1.5
      : -1.5;
  }
}

void updateCurrentOffsetGlide()
{
  wasFailsafe = true;

  if (currentOffset < 0)
    currentOffset += 2.0;
  else if (currentOffset > 0)
    currentOffset -= 2.0;

  if (abs(currentOffset) < 2.5)
    currentOffset = 0;
}

void processFailsafe()
{
  if (currentOffset < 0)
    currentOffset += 1.5;
  else if (currentOffset > 0)
    currentOffset -= 1.5;

  if (abs(currentOffset) < 1.5)
    currentOffset = 0;

  updateServos(
    centerAngle + (int)currentOffset,
    centerAngle - (int)currentOffset
  );

  wasFailsafe = true;
}

void updateServos(int targetL, int targetR)
{
  targetL = constrain(targetL, 20, 160);
  targetR = constrain(targetR, 20, 160);

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
