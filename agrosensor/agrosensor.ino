/*
 Sensors for agriculture: soil moisture, sun, temperature, etc... 
 Tested on an AZDelivery ESP8266
*/

#include <OneWire.h> 
#include <DallasTemperature.h>

double delay_seconds;

int TEMPERATURE_PIN = 12;

OneWire oneWire(TEMPERATURE_PIN); 
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin(); 
  pinMode(A0, INPUT);
}


void loop() {
 sensors.requestTemperatures(); 
 Serial.print("Temp celsius: "); 
 Serial.println(sensors.getTempCByIndex(0));
  
 int moisture = analogRead(A0);
 Serial.print("Soil moisture is: "); 
 Serial.println(mois);
 delay(1000); 
}
