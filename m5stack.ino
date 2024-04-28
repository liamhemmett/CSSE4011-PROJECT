#include "M5Core2.h"
#include <WiFi.h>
#include <PubSubClient.h>

#define TRIG 32
#define ECHO 33
#define USMAX 3000
WiFiClient espClient;
PubSubClient client(espClient);

const char* ssid = "infrastructure";
const char* password = "udDCgaJ9adWM";
const char* mqtt_server = "csse4011-iot.zones.eait.uq.edu.au"; // IP address of your MQTT broker
const char* mqtt_topic = "s4697686/image"; // Topic to publish image data to

void reconnect() {
    while (!client.connected()) {
        String clientId = "M5StackClient-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            client.publish(mqtt_topic, "M5Stack connected");
        } else {
            delay(5000);
        }
    }
}

void setup() {
  Serial.begin(115200);
  M5.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      M5.Lcd.print(".");
  }
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
      Serial.println("connecting");
      reconnect();
  }
  client.loop();

  // Simulated function to get image data
  uint8_t* imageData;
  int imageSize = getImageData(&imageData); // You need to implement this function

  if (imageSize > 0) {
    client.publish(mqtt_topic, imageData, imageSize);
  } else {
    Serial.println("Failed to get image data");
  }

  delay(2000); // Adjust timing as needed
}
