#include "M5Core2.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <IRremote.h>

#define TRIG 32
#define ECHO 33
#define IR_RECEIVER_PIN 33 // Define the IR receiver pin
#define USMAX 3000
WiFiClient espClient;
PubSubClient client(espClient);

const char* ssid = "infrastructure";
const char* password = "udDCgaJ9adWM";
const char* mqtt_server = "csse4011-iot.zones.eait.uq.edu.au"; // IP address of your MQTT broker
const char* mqtt_topic = "s4697686"; // Topic to publish data to
static uint8_t data[8160]; 
IRrecv irrecv(IR_RECEIVER_PIN);
decode_results results;
void reconnect() {
    while (!client.connected()) {
        String clientId = "M5StackClient-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            client.publish(mqtt_topic, "M5Stack connected");
            M5.Lcd.println("MQTT connected");
        } else {
            M5.Lcd.print("MQTT connection failed, rc=");
            M5.Lcd.println(client.state());
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    M5.begin();
    irrecv.enableIRIn();
    WiFi.begin(ssid, password);
    M5.Lcd.setTextSize(2); // Set text size for better readability
    M5.Lcd.println("Connecting to WiFi...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        M5.Lcd.print(".");
    }

    M5.Lcd.println("\nWiFi connected");
    M5.Lcd.print("IP address: ");
    M5.Lcd.println(WiFi.localIP());
    client.setServer(mqtt_server, 1883);

    pinMode(IR_RECEIVER_PIN, INPUT);
}

// Function to read IR data and convert it to an image
int getImageData() {

    decode_results results;
    int i = 0;
    // Initialize the IR receiver

    while(i < sizeof(data)) {
        if (irrecv.decode()) {
            uint64_t value = static_cast<uint64_t>(irrecv.decodedIRData.decodedRawData);
            if (value == 0xFFFFFFFF  ){
              irrecv.resume();
               delay(1); 
              continue;
            }
            // Serial.println(value, HEX);
            //  Serial.print("'");
            
            for(int shift = 0; shift <64; shift+=8){
              if(i<sizeof(data)){
                data[i++] = (value >> shift) & 0xFF;
              } 
            }
            
            // Serial.print("-");
            irrecv.resume();
        }else{
            // Serial.print(".");
        }
        delay(1); 
    }
    Serial.println("IR data read complete");

    return sizeof(data);
}

void loop() {
    if (!client.connected()) {
        Serial.println("Connecting to MQTT...");
        M5.Lcd.println("Connecting to MQTT...");
        reconnect();
    }
    client.loop();

   
    int imageSize = getImageData();
     M5.Lcd.println(imageSize);
        M5.Lcd.println(imageSize);

           M5.Lcd.println(imageSize);
    if (imageSize > 0) {
        client.publish(mqtt_topic, data, imageSize);
        M5.Lcd.println("Image data published");

        // Print image data to screen
//        M5.Lcd.clear(); // Clear the screen before printing
//        M5.Lcd.setCursor(0, 0);
//        M5.Lcd.println("Image Data:");
//        for (int i = 0; i < 64; i++) {
//            M5.Lcd.print(imageData[i]);
//            M5.Lcd.print(" ");
//            if ((i + 1) % 8 == 0) { // Print a new line after every 8 values
//                M5.Lcd.println();
//            }
//        }
    } else {
        Serial.println("Failed to get image data");
        M5.Lcd.println("Failed to get image data");
    }

    delay(2000); // Adjust timing as needed
}
