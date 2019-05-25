#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include "config.h"


#define DHTPIN D7

#define RELAY_PIN D8

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// LiquidCrystal_I2C lcd(0x27,16,2);

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* host = INFLUXDB_HOST;
const char* db = INFLUXDB_DB;
const char* http_credentials = INFLUXDB_DB_HTTP_CREDENTIALS;


void turn_on_relay()
{
  digitalWrite(RELAY_PIN, HIGH);
}

void turn_off_relay()
{
  digitalWrite(RELAY_PIN, LOW);
}

void display_values_lcd(LiquidCrystal_I2C lcd, float temperature, float humidity, float air_quality)
{

  char line1[16];
  sprintf(line1, "T: %d.%01dc H:%d%%", (int)temperature, (int)(temperature*10)%10, (int) humidity);
  Serial.print("line1:");
  Serial.println(line1);
  lcd.setCursor(0,0);
  lcd.print(line1);
  
  char line2[16];
  sprintf(line2, "Air quality: %d", (int)air_quality);
  Serial.print("line2:");
  Serial.println(line2);

  lcd.setCursor(0,1);
  lcd.print(line2);
} 

void reset_lcd(LiquidCrystal_I2C lcd)
{

  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.noBacklight();

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
    sprintf(data, "kitchen,sensor=%s value=%d.%0.2d", sensor,  (int)value, (int)(value*100)%100);
    
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
  pinMode(RELAY_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(2000);
  while(!Serial) { }

  dht.begin();

  // setup lcd
  LiquidCrystal_I2C lcd(0x27,16,2);
  lcd.begin(16,2);
  lcd.init();
  lcd.setBacklight(1);
  lcd.setCursor(0,0);
  lcd.print("Sensor box");
  lcd.setCursor(0,1);
  lcd.print("STARTED");

  // start WIFI
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
    lcd.setCursor(0,1);
    lcd.print("connecting...");
  }

 
  // let the air sensor heat up for 10 seconds
  turn_on_relay();
  delay(10000);

  float temperature =  dht.readTemperature();  
  Serial.print("Temperature = ");
  Serial.println( temperature);

  float humidity =  dht.readHumidity();
  Serial.print("Humidity = ");
  Serial.println(humidity);

  float air_quality = analogRead(0);    
  Serial.print("Air Quality = ");
  Serial.println(air_quality);
  
  //display_values_lcd(lcd, temperature, humidity, air_quality);

  char line1[16];
  sprintf(line1, "T: %d.%01dc H:%d%%", (int)temperature, (int)(temperature*10)%10, (int) humidity);
  Serial.print("line1:");
  Serial.println(line1);
  lcd.setCursor(0,0);
  lcd.print(line1);
  
  char line2[16];
  sprintf(line2, "Air quality: %d", (int)air_quality);
  Serial.print("line2:");
  Serial.println(line2);

  lcd.setCursor(0,1);
  lcd.print(line2);

  record_value((char*)"temperature", temperature);
  record_value((char*)"humidity", humidity);
  record_value((char*)"air_quality", air_quality);

  // show info for 3 seconds
  delay(3000);
  reset_lcd(lcd);
  turn_off_relay();

  // run every 5 minutes
  ESP.deepSleep(300e6); 
}

void loop()
{

 // no need for loop in deep-sleep
}
