/*
 * WiFi Code thanks to: https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
 * HTTP Post Code Inspired by: https://randomnerdtutorials.com/esp32-http-get-post-arduino/
 * HTTPS Code: https://medium.com/@dfa_31434/doing-ssl-requests-on-esp8266-correctly-c1f60ad46f5e
 * Preferences.h: https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
 * API Server on ESP32: https://www.survivingwithandroid.com/esp32-rest-api-esp32-api-server/
 * Web Server on ESP32: https://randomnerdtutorials.com/esp32-web-server-arduino-ide/
 * Custom Hostname on ESP32: https://randomnerdtutorials.com/esp32-set-custom-hostname-arduino/
 */
#include <Arduino.h>
#include <stdlib.h>
#include <Preferences.h>

#include <WebServer.h>
#include <ArduinoJson.h>

// Local Libraries
#include <temperature.h>
#include <network.h>
#include <server.h>

// For Persistent State
Preferences preferences;
bool ledStatus = false;
// For LED
#define LED_PIN 33

// For Server
WebServer server(80);
StaticJsonDocument<250> jsonDocument;
char buffer[250];

// For Temperature Logging
unsigned long sendTempPreviousMillis = 0;
unsigned long sendTempInterval = 600000; // in miliseconds, 600 sec = 10 min

void postTemp()
{
  Serial.print("Update Temp Interval: ");
  if (server.hasArg("plain") == false)
  {
    // handle error here
    Serial.println("\n:: Some error ocurred ::");
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  // Get RGB components
  unsigned long interval = jsonDocument["interval"];
  sendTempInterval = interval;

  preferences.begin("my-app", false);
  Serial.println(interval);
  preferences.putULong("tempInterval", interval);
  preferences.end();

  server.send(200, "application/json", "{}");
}

void postToggleLight()
{
  Serial.print("Toggle Light: ");
  if (server.hasArg("plain") == false)
  {
    //handle error here
  }

  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  // Get RGB components
  int status = jsonDocument["status"];

  if (status == 1)
  {
    preferences.begin("my-app", false);
    ledStatus = true;
    Serial.println("On");
    preferences.putBool("ledStatus", true);
    preferences.end();
  }
  else
  {
    preferences.begin("my-app", false);
    ledStatus = false;
    Serial.println("Off");
    preferences.putBool("ledStatus", false);
    preferences.end();
  }

  // Respond to the client
  server.send(200, "application/json", "{}");
}

void setupRouting()
{
  server.on("/light", HTTP_POST, postToggleLight);
  server.on("/temp", HTTP_POST, postTemp);
  server.begin();
}

void handleClient()
{
  server.handleClient();
}

void setup()
{
  Serial.begin(115200);

  // LED
  preferences.begin("my-app", false);
  ledStatus = preferences.getBool("ledStatus", false);
  preferences.end();
  pinMode(LED_PIN, OUTPUT);

  // DHT 11
  preferences.begin("my-app", false);
  sendTempInterval = preferences.getULong("tempInterval", 30000);
  preferences.end();
  setupDht();

  // WiFi
  initWifi();

  // Server
  setupRouting();

  Serial.println("Device Bootstrapping Complete");
}

void loop()
{
  handleClient();

  if (ledStatus == true)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }

  checkWiFi();

  // getData
  unsigned long currentMillis = millis();
  if (currentMillis - sendTempPreviousMillis >= sendTempInterval)
  {
    Serial.println("Recording Temp data...");
    std::string tempData = readDht();

    // Send Data to Server
    postData(tempData);

    // Set previous send
    sendTempPreviousMillis = currentMillis;
  }

  // Wait 2sec before running again
  delay(2000);
}