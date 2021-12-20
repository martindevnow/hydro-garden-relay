#ifndef NETWORK_FUNCTIONS_H
#define NETWORK_FUNCTIONS_H

#include <WiFi.h>
#include <HTTPClient.h>

void initWifi();
void checkWiFi();
void postData(char *tempData);

#endif