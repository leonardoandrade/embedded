#include "DHT.h"


#define DHTPIN D7

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(9600);
  dht.begin();
}
int counter = 1;
void loop()
{

  counter++;
  Serial.print("#= ");
  Serial.println(counter);
  Serial.print("Temperature = ");
  Serial.println( dht.readTemperature());
  Serial.print("Humidity = ");
  Serial.println( dht.readHumidity());
  delay(3000);
}
