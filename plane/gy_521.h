#ifndef GY_521_H
#define GY_521_H


// wrapper for GY-521 (MPU-6050)
class GY521
{

private:
    int sdaPin;
    int sclPin;
   
public:
    GY521(int);
    int* getValues();
};

#endif