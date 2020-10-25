#ifndef DC_MOTOR_H
#define DC_MOTOR_H

class DCMotor
{

private:
    int controlPin1;
    int controlPin2;
    int speedPin;
    int speed;
    void setSpeed(int);
public:
    DCMotor(int, int, int);
    void start();
    void stop();
    void increaseSpeed();
    void decreaseSpeed();
};

#endif