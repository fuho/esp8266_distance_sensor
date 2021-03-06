#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h> // Needed by WifiManager
#include <ArduinoOTA.h>
#include <WiFiManager.h>


#define SENSOR_TRIGGER D0
#define SENSOR_ECHO D1
#define SENSOR_GROUND D2
#define SENSOR3 D3
#define DELAY_SHORT 10
#define DELAY_LONG 100

WiFiClient client;
int powerUpTime = DELAY_SHORT;

char msg[DELAY_SHORT];

// Initialization
// Sensor

void post(char *payload) {
    Serial.print("*POST: ");
    Serial.println(payload);

    if (client.connect("request.in", 80)) {
        Serial.println("*POST: Connected! Sending");

        client.println("POST /t27neot2 HTTP/1.1");
        client.println("Host:  requestb.in");
        client.println("User-Agent: ESP8266/1.0");
        client.println("Connection: close");
        client.println("Content-Type: application/json;");
        client.print("Content-Length: ");
        client.println(String(payload).length());
        client.println();
        client.println(String(payload));
    } else {
        Serial.println("*POST: Connection failed!");
    }
}

void setup() {
    // Init serial
    Serial.begin(115200);
    Serial.println("Booting ESP8266 Htu21d wireless sensor");

    pinMode(SENSOR_TRIGGER, OUTPUT);
    pinMode(SENSOR_ECHO, INPUT_PULLUP);
    pinMode(SENSOR_GROUND, OUTPUT);
    pinMode(SENSOR3, OUTPUT);

    // Init wifi manager
    WiFiManager wifiManager;
    wifiManager.autoConnect("ESP8266 Sensor");

    // Init ArduinoOta
    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);
    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname("ESP8266FU");

    // No authentication by default
    // ArduinoOTA.setPassword((const char *)"123");
    ArduinoOTA.onStart([]() {
        Serial.println("*OTA: Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\n*OTA: End");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("*OTA: Progress: %u%%\r",
                      (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("*OTA: Error[%u]: ", error);

        if (error ==
            OTA_AUTH_ERROR)
            Serial.println("*OTA: Auth Failed");
        else if (error ==
                 OTA_BEGIN_ERROR)
            Serial.println(
                    "*OTA: Begin Failed");
        else if (error ==
                 OTA_CONNECT_ERROR)
            Serial.println(
                    "*OTA: Connect Failed");
        else if (error ==
                 OTA_RECEIVE_ERROR)
            Serial.println(
                    "*OTA: Receive Failed");
        else if (error ==
                 OTA_END_ERROR)
            Serial.println("*OTA: End Failed");
    });
    ArduinoOTA.begin();
    Serial.println("*OTA: Ready");
    Serial.print("*OTA: IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    digitalWrite(SENSOR_TRIGGER, LOW);
    digitalWrite(SENSOR_GROUND, HIGH);
    ArduinoOTA.handle();
    if (Serial.available() > 0) {
        // read the incoming byte:
        byte incomingByte = Serial.read();
        switch (incomingByte) {
            case 43: // +
                powerUpTime++;
                break;
            case 45: // -
                powerUpTime--;
                break;
        }
            Serial.print("powerUpTime: ");
        Serial.println(powerUpTime, DEC);
    }
    delay(DELAY_SHORT);
    digitalWrite(SENSOR_GROUND, LOW);
    delayMicroseconds(powerUpTime);
    digitalWrite(SENSOR_TRIGGER, HIGH);
    delayMicroseconds(3);
    digitalWrite(SENSOR_TRIGGER, LOW);
    delay(DELAY_SHORT);
/*
    snprintf(
            msg, 75, "{\"distance\":\"%ld\",\"battery\":\"%ld\"}",
            (long) (9999),
            (long) (8888)
    );
    Serial.print("Publish message: ");
    Serial.println(msg);
    post(msg);
    delay(350);
*/
}
