#include "dc_motor.h"

void increaseAndStopDCMotor()
{

    // D8, D7 and D6 on Wemos D1
    DCMotor *dcMotor = new DCMotor(15, 13, 12);
    dcMotor->start();
    for (int i = 0; i < 20; i++)
    {
        dcMotor->increaseSpeed();
        delay(100);
    }
    delay(10000);
    dcMotor->stop();
    delay(3000);
}

void setup()
{
}

void loop()
{
    increaseAndStopDCMotor();
}