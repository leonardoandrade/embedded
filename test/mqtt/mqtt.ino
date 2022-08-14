/*
  Publish messages to topic, with remote MQTT broker
  Tested on an AZDelivery ESP8266
*/

#include "config.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>



const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_broker_host = MQTT_BROKER_HOST;


WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(115200);

  // start WIFI
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting..");
  }
  Serial.println("Connected!");

  //connecting to a mqtt broker
  client.setServer(mqtt_broker_host, 1883);
  //client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp8266 at: ";
    client_id += String(WiFi.macAddress());

    if (client.connect(client_id.c_str())) {
      Serial.println("connected to broker");
    } else {
      Serial.print("failed:");
      Serial.println(client.state());
      delay(2000);
      Serial.println("trying to connect again... ");

    }
  }
}



int count = 1;


void loop() {

  String message = "Hello! ";
  message += String(count);
  client.publish("test/topic", message.c_str());
  count++;
  delay(5000);
}
