#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#include "config.h"

#define ONE_WIRE_PIN D6
OneWire oneWire(ONE_WIRE_PIN);

DallasTemperature sensors(&oneWire);

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* host = INFLUXDB_HOST;
const char* db = INFLUXDB_DB;
const char* http_credentials = INFLUXDB_DB_HTTP_CREDENTIALS;


void scan_wifi() {

    int numberOfNetworks = WiFi.scanNetworks();

    for(int i =0; i<numberOfNetworks; i++){
      Serial.print("Network name: ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Signal strength: ");
      Serial.println(WiFi.RSSI(i));
      Serial.println("-----------------------");
  }
}


void record_value(char* sensor, float value)
{
  int tries = 5;
  Serial.println(WiFi.status());
  while (WiFi.status() != WL_CONNECTED && tries > 0) {
    delay(1000);
    Serial.println("wifi lost. connecting..");
    tries --;
  }

  if(tries == 0) {
        Serial.println("couln not connect wireless");
        return;
  }
  WiFiClientSecure client;


  if (client.connect("andrade.io", 443)) {

    char data[200];
    sprintf(data, "outdoor,sensor=%s value=%d.%0.2d", sensor,  (int)value, (int)(value*100)%100);
    
    char authorization_header[200];
    sprintf(authorization_header, "Authorization: Basic %s", http_credentials);

    client.println("POST /influxdb/write?db=house_sensors HTTP/1.1");
    client.println("Host: " + (String)"andrade.io");
    client.println("User-Agent: ESP8266/1.0");
    
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.println(authorization_header);
    client.print("Content-Length: ");
    client.println(strlen(data));
    client.println();
    client.println(data);
    delay(10);
    String response = client.readString();
    Serial.print("Response: ");
    Serial.println(response);
  }
  else {
     Serial.println("ERROR");
  }
}

void setup(){

    
  Serial.begin(9600);
  Serial.setTimeout(2000);
  Serial.print("Starting outdoor temperature sensor.");
  while(!Serial) { }

  
  sensors.begin();

  // start WIFI
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }


  /*
  // get temperature and save into remote influxdb
  sensors.requestTemperatures(); 
  float temperature_celsius = sensors.getTempCByIndex(0);
  Serial.print("Temperature = ");
  Serial.println( temperature_celsius);
  record_value((char*)"temperature", temperature_celsius);
  
  // run every 5 minutes
  ESP.deepSleep(30e6); */
}

void loop()
{
   // no need for loop in deep-sleep


    sensors.requestTemperatures(); 
  float temperature_celsius = sensors.getTempCByIndex(0);
  Serial.print("Temperature = ");
  Serial.println( temperature_celsius);
  record_value((char*)"temperature", temperature_celsius);
  delay(10000);
}
