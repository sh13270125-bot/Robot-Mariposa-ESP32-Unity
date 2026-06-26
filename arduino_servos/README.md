# Módulo de Control de Alas

## Descripción

Este módulo corresponde al sistema encargado del movimiento físico de las alas de la mariposa biomimética.

El sistema fue desarrollado utilizando un Arduino Pro Mini, un receptor FlySky iBUS y dos servomotores.

Su función principal consiste en generar el movimiento de aleteo mediante diferentes modos de operación.

---

## Hardware utilizado

- Arduino Pro Mini
- Receptor FlySky iBUS
- 2 Servomotores
- LED indicador
- Fuente de alimentación

---

## Librerías utilizadas

- Servo.h
- IBusBM.h

---

## Funciones principales

- Lectura de canales del receptor FlySky.
- Movimiento senoidal de las alas.
- Modo Planeo.
- Modo Perchado.
- Sistema Failsafe.
- Suavizado del movimiento de los servos.
- Ajuste de Pitch mediante el control remoto.

---

## Archivo principal

El archivo principal es:

Mariposa_Servos.ino

---

## Integración con el proyecto

Este módulo forma parte del proyecto Robot Mariposa Biomimético y trabaja junto con:

- ESP32 + MPU6050
- Simulación en Unity
- Comunicación inalámbrica por WiFi

Cada módulo fue desarrollado de manera independiente para facilitar las pruebas y el mantenimiento del sistema.
