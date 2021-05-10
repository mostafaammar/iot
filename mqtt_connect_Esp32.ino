#include <MQTT.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ArduinoJson.h>
IPAddress local_IP(192, 168, 1, 185);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optiona

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define ENDPOINT "192.168.1.22"
  char buff[10];

const char* ssid     = "put ssid here";
const char* password = "put password here";
MQTTClient client = MQTTClient(256);
WiFiClient net;
unsigned long lastMillis = 0;
int i=0;

void setup() {
  Serial.begin(115200);
  
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid_home);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

client.begin("192.168.1.22",net);//replace the ip address with mqtt server ip address
client.subscribe("topic");
client.onMessage(messageReceived);

}
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/test");
  // client.unsubscribe("/hello");
}
void publishMessage(int i)
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sensor_a0"] = analogRead(0);
  doc["number"]=i;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish("test", jsonBuffer);
}
void loop() {
    
  //client.loop();
    client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    
  
  }
  for (i =0; i <1000; i++)
  {
  publishMessage(i);
  snprintf (buff, sizeof(buff), "%d", i);

  client.publish("test", buff);
  delay(1000);
  }
}
