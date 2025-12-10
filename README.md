# ESP32 – Sistema de Monitoreo Ambiental con MQTT

Este proyecto utiliza un ESP32 para monitorear variables ambientales y enviarlas mediante MQTT. Incluye sensores de temperatura y humedad (DHT22), humedad del suelo (analógica y digital) y está preparado para integrar actuadores como motores, servos o relés.

## Características

- Lectura de:
  - Temperatura (°C)
  - Humedad relativa (%)
  - Humedad del suelo analógica
  - Señal digital de humedad crítica
- Publicación en un broker MQTT.
- Reconexión automática a WiFi y MQTT.
- Compatible con Mosquitto, HiveMQ, Node-RED, Grafana y Home Assistant.
- Código preparado para control de actuadores.

## Arquitectura

ESP32 → WiFi → Broker MQTT → Dashboard o Servicio IoT

## Estructura del proyecto

/ESP32-MQTT
│── /src
│ └── main.cpp
│── /include
│── platformio.ini
│── README.md


## Conexiones de Hardware

| Componente          | Pin ESP32 | Descripción                   |
|--------------------|-----------|-------------------------------|
| DHT22              | GPIO 4    | Datos                         |
| Humedad suelo A0   | GPIO 2    | Lectura analógica             |
| Humedad suelo D0   | GPIO 27   | Señal digital de humedad      |
| Motor/Relay (opt.) | GPIO 14   | Control de actuador           |

## Configuración del código

Modificar las credenciales en el archivo principal:

```cpp
const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";

const char* mqtt_server = "IP_DEL_BROKER";
const int mqtt_port = 1883;


| Topic               | Descripción                    |
| ------------------- | ------------------------------ |
| terra/temperatura   | Temperatura en grados Celsius  |
| terra/humedad       | Humedad relativa (%)           |
| terra/suelo/analog  | Valor analógico del suelo      |
| terra/suelo/digital | Estado digital del suelo (0/1) |
| terra/alerta        | Mensajes de alerta (opcional)  |
pio run
pio upload
pio device monitor

Ejecución con Arduino IDE

Seleccionar placa: ESP32 Dev Module

Instalar librerías:

DHT sensor library

Adafruit Unified Sensor

PubSubClient

Cargar el código y abrir el monitor serie.
