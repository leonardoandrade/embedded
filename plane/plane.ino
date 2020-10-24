#include <Servo.h>
Servo servo;
int angle = 0;

void setup()
{
    servo.attach(8);
    servo.write(0);
    delay(2000);
    servo.write(90);
    delay(2000);
    servo.write(180);
    delay(2000);
}

void loop()
{
}