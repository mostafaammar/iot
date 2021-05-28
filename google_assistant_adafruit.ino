#define AIO_USERNAME  "............"
#define AIO_KEY       ".............."
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "passwords.h"
#define WLAN_SSID       "...."
#define WLAN_PASS       "......"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883          
const char* ssid_home= "........";//put here your ssid
const char* wifi_pass=".........";//put here your password
int output=2;
WiFiClient client;     // Create an ESP8266 WiFiClient class to connect to the MQTT server.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);        // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Subscribe LED_Control = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led-switch");

void MQTT_connect();
void setup() {
Serial.begin(115200);
delay(10);
pinMode(2,OUTPUT);
pinMode(26,OUTPUT);
// Connect to WiFi access point.
Serial.println(); Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid_home);
WiFi.begin(ssid_home, wifi_pass);
while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
  }
Serial.println();
Serial.println("WiFi connected");
Serial.println("IP address: "); Serial.println(WiFi.localIP());
mqtt.subscribe(&LED_Control);
}

uint32_t x=0;

void loop() {
   MQTT_connect();
Adafruit_MQTT_Subscribe *subscription;
while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &LED_Control) {
      Serial.print(F("Got: "));
      Serial.println((char *)LED_Control.lastread);
       if (!strcmp((char*) LED_Control.lastread, "ledon"))
      {
        digitalWrite(26,HIGH);
   digitalWrite(2,HIGH);

        Serial.println("LED on heeeAeh");
      }
      else 
      {
        digitalWrite(26,LOW);
        digitalWrite(2,LOW);
      }
    }
  }
}
void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

Serial.print("Connecting to MQTT... ");

uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
