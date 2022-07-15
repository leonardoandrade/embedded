/*
 Sensors for agriculture: soil moisture, sun, temperature, etc... 
 Tested on an AZDelivery ESP8266
*/

#include "config.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

double delay_seconds;

int TEMPERATURE_PIN = 12;


const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* host = INFLUXDB_HOST;
const char* db = INFLUXDB_DB;
const char* http_credentials = INFLUXDB_DB_HTTP_CREDENTIALS;

OneWire oneWire(TEMPERATURE_PIN); 
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin(); 
  pinMode(A0, INPUT);


  // start WIFI
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
}


void send_to_remote_influxdb(double air_temperature, int soil_moisture)
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
    char data_temperature[100];
    sprintf(data_temperature, "outdoor,sensor=air_temperature value=%d.%0.2d",  (int)air_temperature, (int)(air_temperature*100)%100);
    
    char data_moisture[100];
    sprintf(data_moisture, "outdoor,sensor=soil_moisture value=%d", (int)data_moisture);

    
    char authorization_header[200];
    sprintf(authorization_header, "Authorization: Basic %s", http_credentials);

    char data[200];
  
    sprintf(data, "%s\n%s", data_temperature, data_moisture);

    client.println("POST /influxdb/write?db=agro HTTP/1.1");
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

void loop() {
 sensors.requestTemperatures(); 
 double air_temperature = sensors.getTempCByIndex(0);
 Serial.print("Temp celsius: "); 
 Serial.println(air_temperature);
  
 int soil_moisture = analogRead(A0);
 Serial.print("Soil moisture is: "); 
 Serial.println(soil_moisture);

 send_to_remote_influxdb(air_temperature, soil_moisture);
 delay(1000); 
}
