#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>

const char* ssid = "S24 Ultra de Samuel";
const char* password = "13270125";

WiFiServer server(5000);
WiFiClient client;

MPU6050 mpu;

float pitch = 0;
float roll = 0;

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 conectado");
  } else {
    Serial.println("Error MPU6050");
  }

  WiFi.begin(ssid, password);

  Serial.print("Conectando a WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado");
  Serial.print("IP del ESP32: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop()
{
  if (!client || !client.connected()) {
    client = server.available();
  }

  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float accelPitch = atan2(ay, az) * 180 / PI;

  float accelRoll =
    atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI;

  pitch = accelPitch;
  roll = accelRoll;

  pitch = constrain(pitch, -90, 90);
  roll = constrain(roll, -90, 90);

  String data = String(pitch) + "," + String(roll) + "\n";

  if (client && client.connected()) {
    client.print(data);
  }

  Serial.print("Enviado: ");
  Serial.print(data);

  delay(20);
}
