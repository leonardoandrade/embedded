
#include "rgb_led.h"


RGBLed::RGBLed(int pin_r, int pin_g, int pin_b)
{
    this->pin_r = pin_r;
    this->pin_g = pin_g;
    this->pin_b = pin_b;

    pinMode(this->pin_r, OUTPUT);
    pinMode(this->pin_g, OUTPUT);
    pinMode(this->pin_b, OUTPUT);
    this->off();
}


void RGBLed::off()
{
    digitalWrite(this->pin_r, LOW);
    digitalWrite(this->pin_g, LOW);
    digitalWrite(this->pin_b, LOW);
}


void RGBLed::on()
{
    digitalWrite(this->pin_r, HIGH);
    digitalWrite(this->pin_g, HIGH);
    digitalWrite(this->pin_b, HIGH);
}



void RGBLed::animate(int delay_ms)
{

    digitalWrite(this->pin_r, HIGH);
    delay(delay_ms/3);
    this->off();
    digitalWrite(this->pin_g, HIGH);
    delay(delay_ms/3);
    this->off();
    digitalWrite(this->pin_b, HIGH);
    delay(delay_ms/3);
    this->off(); 
}
