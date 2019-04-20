#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN D7

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "KabelBox-B748";
const char* password = "35960124337487803660";

void recordValues(float temperature, float humidity) {
  int tries = 5;
   Serial.println(WiFi.status());
  while (WiFi.status() != WL_CONNECTED && tries > 0) {
    delay(1000);
    Serial.println("wifi lost. connecting..");
    tries --;
  }

  if(tries == 0) {
        Serial.println("couln not connect wireless");
  }
  HTTPClient http;
  http.begin("http://192.168.0.11:8888");
  int httpCode = http.GET();
  http.end();
  Serial.print("resp= ");
  Serial.println(httpCode);
  
}

void setup(){
  Serial.begin(9600);
  dht.begin();
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }


}


int counter = 1;

void loop()
{

  counter++;
  Serial.print("#= ");
  Serial.println(counter);
  
  float temperature =  dht.readTemperature();

  float humidity =  dht.readHumidity();
  
  Serial.print("Temperature = ");
  Serial.println( temperature);
  Serial.print("Humidity = ");
  Serial.println(humidity);
   recordValues(temperature, humidity);
  delay(3000);
}
