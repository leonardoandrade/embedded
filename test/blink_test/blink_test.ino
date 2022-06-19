/*
 Blink LED
 Tested on an AZDelivery ESP8266
*/

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
 
}

double INITIAL_DELAY_SECONDS = 5.0;
double delay_seconds;

void loop() {
  if(delay_seconds < 0.01) {
    delay_seconds = INITIAL_DELAY_SECONDS;
  }
  else {
    delay_seconds = delay_seconds / 2.0;
  }
  
  digitalWrite(BUILTIN_LED, LOW);   // LED off
  
  delay(delay_seconds * 500);                  // WAIT some time with led off
  digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  
  
  delay(delay_seconds * 1000);                 // Wait double time with led on
  Serial.print("Response:");
}
