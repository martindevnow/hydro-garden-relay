#ifndef NETWORK_FUNCTIONS_H
#define NETWORK_FUNCTIONS_H

#include <WiFi.h>
#include <HTTPClient.h>

void initWifi();
void checkWiFi();
char* macAddress();
void postData(char *tempData);

#endif