#ifndef TEMP_FUNCTIONS_H
#define TEMP_FUNCTIONS_H

#include <string>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

void setupDht();
std::string readDht();

#endif