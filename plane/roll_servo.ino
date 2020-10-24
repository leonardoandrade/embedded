
#include "roll_servo.h"

#define ROLL_LEFT_ANGLE 110
#define ROLL_RIGHT_ANGLE 70
#define BASE_ANGLE 90

RollServo::RollServo(int pin)
{
    this->servo.attach(pin);
    this->reset();
}

void RollServo::leftRoll()
{
    this->update(ROLL_LEFT_ANGLE);
}

void RollServo::rightRoll()
{
    this->update(ROLL_RIGHT_ANGLE);
}

void RollServo::reset()
{
    this->update(BASE_ANGLE);
}

void RollServo::update(int angle)
{
    this->angle = angle;
    this->servo.write(this->angle);
}