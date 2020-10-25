
#include "dc_motor.h"

#define MAX_SPEED 1024
#define MIN_SPEED 0
#define SPEED_STEP 100

DCMotor::DCMotor(int controlPin1, int controlPin2, int speedPin)
{
    this->controlPin1 = controlPin1;
    this->controlPin2 = controlPin2;
    this->speedPin = speedPin;
    this->speed = MIN_SPEED;
    pinMode(this->controlPin1, OUTPUT);
    pinMode(this->controlPin2, OUTPUT);

    pinMode(this->speedPin, OUTPUT);
}

void DCMotor::start()
{
    digitalWrite(this->controlPin1, HIGH);
    digitalWrite(this->controlPin2, LOW);
}

void DCMotor::stop()
{
    this->speed = MIN_SPEED;
    digitalWrite(this->controlPin1, LOW);
    digitalWrite(this->controlPin2, LOW);
}

void DCMotor::increaseSpeed()
{
    this->speed = std::min(this->speed + SPEED_STEP, MAX_SPEED);
    analogWrite(this->speedPin, this->speed);
}

void DCMotor::decreaseSpeed()
{
    this->speed = std::max(this->speed - SPEED_STEP, MIN_SPEED);
    analogWrite(this->speedPin, this->speed);
}