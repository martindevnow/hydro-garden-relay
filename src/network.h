#ifndef NETWORK_FUNCTIONS_H
#define NETWORK_FUNCTIONS_H

#include <string>
#include <WiFi.h>
#include <HTTPClient.h>

void initWifi();
void checkWiFi();
std::string macAddress();
void postData(std::string tempData);
void postAttendance();

#endif