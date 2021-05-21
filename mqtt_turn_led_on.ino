#include <WiFi.h>
#include <PubSubClient.h>

//TODO: ESP32 MQTT user config
const char* ssid = ".........."; // Wifi SSID
const char* password = ".........."; // Wifi Password
const char* username = ".............."; // my AskSensors username
const char* subTopic = "test"; // write here your topic
const int LED_pin = 2; // LEd pin

//AskSensors MQTT config
const char* mqtt_server = "......."; // put here ip address of your mqtt server
unsigned int mqtt_port = 1883;

WiFiClient net;
PubSubClient client(net);

void setup() {
Serial.begin(115200);
Serial.println("Program Start : ESP32 controls LED over MQTT");
Serial.println("Set LED as output");
pinMode(LED_pin, OUTPUT); // set led as output

Serial.print("********** connecting to WIFI : ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("->WiFi connected");
Serial.println("->IP address: ");
Serial.println(WiFi.localIP());

client.setServer(mqtt_server, mqtt_port);
client.setCallback(callback);

if (!client.connected()) 
reconnect();
Serial.print("********** Subscribe totopic:");
Serial.print(subTopic);
// susbscribe
client.subscribe(subTopic);
}

void loop() {
client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
Serial.print("Command teceived from AskSensors[");
Serial.print(topic);
Serial.print("] ");

String message;
for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];  //Conver *byte to String
}
Serial.println("hello");
Serial.println(message);

if(message == "on") {
  digitalWrite(LED_pin,1); 
  Serial.println("LED is on");
  delay(1000);
  }   //LED on  
if(message == "0") {digitalWrite(LED_pin,0);} //LED off
Serial.println();
Serial.println("-----------------------"); 

}

void reconnect() {
// Loop until we're reconnected
while (!client.connected()) {
Serial.print("********** Attempting MQTT connection...");
// Attempt to connect
//if (client.connect("ESP32Client", username, "")) { 
if (client.connect("ESP32Client")) { 
Serial.println("-> MQTT client connected");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println("-> try again in 5 seconds");
// Wait 5 seconds before retrying
delay(5000);
}
}
}
