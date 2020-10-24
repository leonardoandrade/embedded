
#include "roll_servo.h"

RollServo *rollServo;

void setup()
{
    rollServo = new RollServo(15);
}

void loop()
{

    rollServo->leftRoll();
    delay(1000);

    rollServo->rightRoll();

    delay(1000);

    rollServo->reset();

    delay(3000);
}
