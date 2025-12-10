#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ------ CONFIGURACIÓN WIFI ------
#define ssid      "WIFI"
#define password      "PASS"

// ------ CONFIGURACIÓN MQTT ------
#define mqtt_server ""
#define mqtt_port  1883
#define mqtt_topic ""

// ------ PINES ------
#define PIN_SOIL_ANALOG   A0    // Salida analógica MH-P103
// #define PIN_SOIL_DIGITAL  27    // Salida digital MH-P103
#define PIN_PHOTO         34    // Fotorresistor
#define PIN_DHT           14    // DHT11
#define DHTTYPE           DHT11
// ------ CONFIG DHT ------
#define DHTTYPE DHT11
DHT dht(PIN_DHT, DHTTYPE);

// ------ OBJETOS ------
WiFiClient espClient;
PubSubClient client(espClient);


// -------------------------------------
// Reconexion MQTT
// -------------------------------------
void reconnectMQTT() {
    while (!client.connected()) {
        Serial.println("Intentando conectar al broker MQTT...");
        if (client.connect("ESP32_TerraSmart")) {
            Serial.println("MQTT conectado!");
        } else {
            Serial.print("Fallo MQTT, rc=");
            Serial.println(client.state());
            Serial.println("Reintentando en 3 segundos...");
            delay(3000);
        }
    }
}

// -------------------------------------
// SETUP PRINCIPAL
// -------------------------------------

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("\n===== Iniciando ESP32 =====");

    // pinMode(PIN_SOIL_DIGITAL, INPUT);

    dht.begin();

    // --------- WIFI ----------
    Serial.print("Conectando a WiFi ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    // --------- MQTT ----------
    client.setServer(mqtt_server, mqtt_port);
}
// -------------------------------------
// LOOP PRINCIPAL
// -------------------------------------
void loop() {
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();

    // ------ Lectura del MH-P103 ------
    int soilAnalog = analogRead(PIN_SOIL_ANALOG);
    Serial.print("Soil Analog Raw: ");
    Serial.println(soilAnalog);
    // int soilDigital = digitalRead(PIN_SOIL_DIGITAL);

    // ------ Lectura del DHT11 ------
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Validar lectura del DHT
    if (isnan(h) || isnan(t)) {
        Serial.println("Error leyendo el DHT11");
    }

    // ------ Lectura de fotocelda ------
    int photoValue = analogRead(PIN_PHOTO);

    // ------ Mostrar en consola ------
    Serial.println("----- Lecturas -----");
    Serial.printf("humedad_suelo: %d\n", soilAnalog);
    // Serial.printf("Suelo (Digital): %d\n", soilDigital);
    Serial.printf("Luz: %d\n", photoValue);
    Serial.printf("Humedad: %.1f\n", h);
    Serial.printf("Temp: %.1f\n", t);

    // ------ Crear JSON ------
    char payload[256];
    snprintf(payload, sizeof(payload),
        "{\"humedad_suelo\": %d, \"luz\": %d, \"humedad_aire\": %.1f, \"temperatura\": %.1f}",
        soilAnalog, photoValue, h, t
    );

    // ------ Enviar MQTT ------
    Serial.print("Publicando MQTT: ");
    Serial.println(payload);

    client.publish(mqtt_topic, payload);

    delay(3000);
}

