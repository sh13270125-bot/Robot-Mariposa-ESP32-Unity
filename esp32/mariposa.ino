#include "MotionData.h"
#include "IMUSensor.h"
#include "WiFiCommunication.h"
#include "ButterflyController.h"

const char* ssid = "S24 Ultra de Samuel";
const char* password = "13270125";

IMUSensor imu;
WiFiCommunication wifi(ssid, password, 5000);
ButterflyController controller(2.0);

void setup()
{
  Serial.begin(115200);

  imu.begin();

  if (imu.isConnected())
  {
    Serial.println("MPU6050 conectado");
  }
  else
  {
    Serial.println("Error al conectar MPU6050");
  }

  wifi.begin();
}

void loop()
{
  wifi.updateClient();

  MotionData motion = imu.readMotion();

  motion = controller.applyDeadZone(motion);

  wifi.sendMotion(motion);

  Serial.print("Pitch: ");
  Serial.print(motion.pitch);

  Serial.print(" Roll: ");
  Serial.println(motion.roll);

  delay(20);
}
