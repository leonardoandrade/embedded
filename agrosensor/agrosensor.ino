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
    delay(2000);
    Serial.println("Connecting..");
  }
  Serial.println("Connected!");
}


void send_to_remote_influxdb(double air_temperature, int soil_moisture, int wifi_rssi)
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

  client.setInsecure(); // because this device does not know current date to validate certificates
  if (client.connect("andrade.io", 443)) {
    
    char data_temperature[100];
    
    sprintf(data_temperature, "outside,sensor=air_temperature value=%d.%0.2d",  (int)air_temperature, (int)(air_temperature*100)%100);
    
    char data_moisture[100];
    sprintf(data_moisture, "outside,sensor=soil_moisture value=%d", (int)soil_moisture);


    char data_wifi_rssi[100];
    sprintf(data_wifi_rssi, "outside,sensor=wifi_rssi value=%d", (int)wifi_rssi);


    char authorization_header[200];
    sprintf(authorization_header, "Authorization: Basic %s", http_credentials);

    char data[300];
    sprintf(data, "%s\n%s\n%s", data_temperature, data_moisture, data_wifi_rssi);

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
    char buf[1000];
    client.getLastSSLError(buf, 100); 
    Serial.print("ERROR: ");
    Serial.println(buf);
  }
  
}

void loop() {
 


 double air_temperature = -127;

 int retries_temperature = 5;
 for(int i=0; i<retries_temperature; i++) {
   sensors.requestTemperatures(); 
   air_temperature = sensors.getTempCByIndex(0);
   if(air_temperature > -100.0) {
     Serial.print("Temp celsius: "); 
     Serial.println(air_temperature);
     break;
   }
   Serial.println("ERROR getting temperature"); 

   delay(1000);
   
 }

  
 int soil_moisture = analogRead(A0);
 Serial.print("Soil moisture is: ");
 Serial.println(soil_moisture);

 
 int rssi_dbm = WiFi.RSSI();
 Serial.print("Wifi strength: ");
 Serial.println(rssi_dbm);

 send_to_remote_influxdb(air_temperature, soil_moisture, rssi_dbm);
 delay(60000);
}
