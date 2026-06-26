#ifndef WIFI_COMMUNICATION_H
#define WIFI_COMMUNICATION_H

#include <WiFi.h>
#include "MotionData.h"

class WiFiCommunication
{
private:
  const char* ssid;
  const char* password;
  WiFiServer server;
  WiFiClient client;

public:
  WiFiCommunication(
    const char* wifiName,
    const char* wifiPassword,
    int port
  ) : ssid(wifiName),
      password(wifiPassword),
      server(port)
  {
  }

  void begin()
  {
    WiFi.begin(ssid, password);

    Serial.print("Conectando a WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi conectado");
    Serial.print("IP del ESP32: ");
    Serial.println(WiFi.localIP());

    server.begin();
  }

  void updateClient()
  {
    if (!client || !client.connected())
    {
      client = server.available();
    }
  }

  void sendMotion(MotionData data)
  {
    if (client && client.connected())
    {
      client.print(data.pitch);
      client.print(",");
      client.print(data.roll);
      client.print("\n");
    }
  }
};

#endif