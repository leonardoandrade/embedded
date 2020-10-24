#ifndef ROLL_SERVO
#define ROLL_SERVO
#include <Servo.h>

class RollServo
{

private:
    int angle;
    Servo servo;
    void update(int);

public:
    RollServo(int pin);
    void leftRoll();
    void rightRoll();
    void reset();
};

#endif