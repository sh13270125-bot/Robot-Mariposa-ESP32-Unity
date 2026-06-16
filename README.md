# Robot-Mariposa-ESP32-UnityREADME.txt
Nombre del proyecto

Robot Mariposa Controlado mediante ESP32 y MPU6050 con Visualización en Unity

Qué hace el proyecto

Este proyecto simula el movimiento de una mariposa utilizando un sensor MPU6050 conectado a un ESP32. El sensor detecta la inclinación física del dispositivo y el ESP32 transmite los datos mediante Wi-Fi hacia Unity. En Unity, una mariposa 3D se mueve en la dirección correspondiente a la inclinación del sensor.

Lenguajes utilizados
C++ para ESP32.
C# para Unity.
Requisitos necesarios

Hardware:

ESP32.
MPU6050.
Protoboard.
Cables Dupont.
Computadora.
Hotspot de celular.
Power bank o cable USB.

Software:

Arduino IDE.
Unity 6.
Visual Studio.
Librerías de Arduino: Wire.h, MPU6050.h, WiFi.h.
Diagrama de conexión del hardware

El diagrama se encuentra en la carpeta:

imgs/Diagrama_del_circuito.png

Conexiones:

MPU6050 → ESP32
VCC → 3.3V
GND → GND
SDA → GPIO 21
SCL → GPIO 22

Cómo implementar el código
Abrir Arduino IDE.
Instalar la librería MPU6050.
Abrir el archivo main.ino.
Configurar el nombre y contraseña del Wi-Fi.
Subir el código al ESP32.
Abrir el Monitor Serial.
Copiar la IP del ESP32.
Abrir Unity.
Colocar la IP del ESP32 en el script SerialMPU.cs.
Ejecutar la escena con Play.
Funcionamiento

El ESP32 lee el sensor MPU6050, calcula Pitch y Roll y envía los valores por Wi-Fi a Unity. Unity recibe esos valores y mueve la mariposa 3D dentro del escenario.

Conclusiones
Conclusión individual – Samuel Hernández Oliva

Durante el desarrollo de este proyecto aprendí a integrar hardware y software dentro de una misma aplicación. Uno de los aprendizajes más importantes fue comprender cómo un sensor físico puede enviar información a una aplicación gráfica y cómo esos datos pueden transformarse en movimiento dentro de Unity.

También aprendí a trabajar con el ESP32, el sensor MPU6050 y la comunicación Wi-Fi. Al principio se presentaron varios problemas, como errores de conexión, puertos ocupados, ejes invertidos y movimientos incorrectos dentro de Unity. Sin embargo, cada problema permitió entender mejor el funcionamiento del sistema.

Uno de los retos más importantes fue lograr que la mariposa virtual se moviera en la misma dirección en la que se inclinaba el sensor. Para solucionarlo fue necesario probar diferentes configuraciones de Pitch y Roll hasta que el movimiento coincidiera correctamente.

Como mejora futura, se podría implementar una mariposa física con alas móviles, agregar servomotores, mejorar la interfaz gráfica y utilizar sensores más precisos para obtener una simulación más estable.

Conclusión individual – Integrante 2

[Redactar conclusión individual]

Conclusión individual – Integrante 3

[Redactar conclusión individual]

Referencias Bibliográficas

[1] Espressif Systems, “ESP32 Technical Reference Manual,” Espressif Systems, 2024.

[2] InvenSense, “MPU-6000 and MPU-6050 Product Specification,” TDK InvenSense, 2013.

[3] Arduino, “Arduino Documentation,” Arduino, 2024.

[4] Unity Technologies, “Unity Manual,” Unity Technologies, 2024.

[5] Microsoft, “C# Documentation,” Microsoft Learn, 2024.

[6] GitHub, “GitHub Docs,” GitHub, 2024.

Anexos
Anexo A: Enlace al repositorio

[Colocar enlace de GitHub]

Anexo B: Video demostrativo

[Colocar enlace al video]

Anexo C: Diagramas
Diagrama de arquitectura.
Diagrama del circuito.
Captura de la aplicación en Unity.
Anexo D: Código fuente relevante

El código completo se encuentra en el repositorio del proyecto.

Anexo E: Manual de usuario
Encender el hotspot del celular.
Conectar la computadora al hotspot.
Encender el ESP32.
Verificar la IP en el Monitor Serial.
Abrir Unity.
Ejecutar la simulación.
Inclinar el sensor para mover la mariposa.
Anexo F: Manual técnico

El sistema utiliza comunicación I2C entre el MPU6050 y el ESP32. El ESP32 calcula Pitch y Roll y envía estos datos mediante Wi-Fi usando TCP/IP. Unity recibe los datos en C#, los inter
