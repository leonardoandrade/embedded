/*
 Blink LED, the blink time goes faster, then starts again slow. 
 Tested on an AZDelivery ESP8266
*/

#include <ArduinoUniqueID.h>


void dump_info()
{
    UniqueIDdump(Serial);
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  dump_info();
}

double INITIAL_DELAY_SECONDS = 5.0;
double delay_seconds;

int iteration = 0;
void loop() {
  iteration++;
  if(delay_seconds < 0.01) {
    Serial.println("Delay reset!");
    delay_seconds = INITIAL_DELAY_SECONDS;
  }
  else {
    delay_seconds = delay_seconds / 2.0;  // halve the blinking at every iteration
  }
  
  digitalWrite(BUILTIN_LED, LOW);   // LED off
  
  delay(delay_seconds * 500);                  // WAIT some time with led off
  
  digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(delay_seconds * 1000);                 // Wait double time with led on

  
  Serial.print("Iteration: ");
  Serial.print(iteration);

  Serial.print(" delay: ");
  Serial.println(delay_seconds);
}
