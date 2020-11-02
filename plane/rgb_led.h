#ifndef RGB_LED
#define RGB_LED

class RGBLed
{

private:
    int pin_r;
    int pin_g;
    int pin_b;

public:
    RGBLed(int pin_r, int pin_g, int pin_b);
    void on();
    void off();
    void animate(int delay);
};

#endif