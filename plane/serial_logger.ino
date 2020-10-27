#include "serial_logger.h"

#define FREQUENCY_BAUD 9600

SerialLogger::SerialLogger()
{
    Serial.begin(FREQUENCY_BAUD);
}

void SerialLogger::debug(char* message) {
    Serial.println(message);
}