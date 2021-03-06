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
#include <network.h>
#include <server.h>

// For Persistent State
Preferences preferences;
bool relayPort1Status = false;
bool relayPort2Status = false;
bool relayPort3Status = false;
bool relayPort4Status = false;
bool relayPort5Status = false;
bool relayPort6Status = false;
bool relayPort7Status = false;
bool relayPort8Status = false;

// For Supplying Power to Relay
#define RELAY_PORT1_PIN 32
#define RELAY_PORT2_PIN 25
#define RELAY_PORT3_PIN 26
#define RELAY_PORT4_PIN 27
#define RELAY_PORT5_PIN 23
#define RELAY_PORT6_PIN 22
#define RELAY_PORT7_PIN 21
#define RELAY_PORT8_PIN 19

// For Reading Hall Sensors
#define HALL_PORT1_PIN 36
#define HALL_PORT2_PIN 39
#define HALL_PORT3_PIN 34
#define HALL_PORT4_PIN 35
#define HALL_PORT5_PIN 18
#define HALL_PORT6_PIN 17
#define HALL_PORT7_PIN 16
#define HALL_PORT8_PIN 15

#define HALL_PIN 35

void readHall()
{
  float ACSValue = 0.0, Samples = 0.0, AvgACS = 0.0, BaseVol = 1.52; //Change BaseVol as per your reading in the first step.
  for (int x = 0; x < 500; x++)
  { //This would take 500 Samples
    ACSValue = analogRead(HALL_PIN);
    Samples = Samples + ACSValue;
    delay(3);
  }
  AvgACS = Samples / 500;
  Serial.println((((AvgACS) * (3.3 / 4095.0)) - BaseVol) / 0.066); //0.066V = 66mVol. This is sensitivity of your ACS module.
}

void setRelayPins()
{
  Serial.print("relayPort1Status: ");
  Serial.println(relayPort1Status);
  if (relayPort1Status)
  {
    Serial.println("Port 1 is HIGH");
    digitalWrite(RELAY_PORT1_PIN, HIGH);
  }
  else
  {
    Serial.println("Port 1 is LOW");
    digitalWrite(RELAY_PORT1_PIN, LOW);
  }

  if (relayPort2Status)
  {
    digitalWrite(RELAY_PORT2_PIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PORT2_PIN, LOW);
  }

  if (relayPort3Status)
  {
    digitalWrite(RELAY_PORT3_PIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PORT3_PIN, LOW);
  }

  if (relayPort4Status)
  {
    digitalWrite(RELAY_PORT4_PIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PORT4_PIN, LOW);
  }

  if (relayPort5Status)
  {
    digitalWrite(RELAY_PORT5_PIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PORT5_PIN, LOW);
  }

  if (relayPort6Status)
  {
    digitalWrite(RELAY_PORT6_PIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PORT6_PIN, LOW);
  }

  if (relayPort7Status)
  {
    digitalWrite(RELAY_PORT7_PIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PORT7_PIN, LOW);
  }

  if (relayPort8Status)
  {
    digitalWrite(RELAY_PORT8_PIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PORT8_PIN, LOW);
  }
}

// For Server
WebServer server(80);
StaticJsonDocument<250> jsonDocument;

// For Temperature Logging
unsigned long checkHallPreviousMillis = 0;
unsigned long checkHallInterval = 600000; // in miliseconds, 600 sec = 10 min

void postHallInterval()
{
  Serial.print("Update Check Hall Interval: ");
  if (server.hasArg("plain") == false)
  {
    // handle error here
    Serial.println("\n:: Some error ocurred ::");
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  // Get RGB components
  unsigned long interval = jsonDocument["interval"];
  checkHallInterval = interval;

  preferences.begin("my-app", false);
  Serial.println(interval);
  preferences.putULong("checkHallInterval", interval);
  preferences.end();

  server.send(200, "application/json", "{}");
}

void postTogglePort()
{
  Serial.print("Toggle port: ");
  if (server.hasArg("plain") == false)
  {
    //handle error here
  }

  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  // Get RGB components
  int status = jsonDocument["status"];
  Serial.print("Status: ");
  Serial.println(status);

  int port = jsonDocument["port"];
  Serial.print("Port: ");
  Serial.println(port);

  preferences.begin("my-app", false);

  switch (port)
  {
  case 1:
    relayPort1Status = status == 1 ? true : false;
    Serial.print("Toggled Port 1: ");
    Serial.println(relayPort1Status);
    break;
  case 2:
    relayPort2Status = status == 1 ? true : false;
    Serial.print("Toggled Port 2: ");
    Serial.println(relayPort2Status);
    break;
  case 3:
    relayPort3Status = status == 1 ? true : false;
    Serial.print("Toggled Port 3: ");
    Serial.println(relayPort3Status);
    break;
  case 4:
    relayPort4Status = status == 1 ? true : false;
    Serial.print("Toggled Port 4: ");
    Serial.println(relayPort4Status);
    break;
  case 5:
    relayPort5Status = status == 1 ? true : false;
    Serial.print("Toggled Port 5: ");
    Serial.println(relayPort5Status);
    break;
  case 6:
    relayPort6Status = status == 1 ? true : false;
    Serial.print("Toggled Port 6: ");
    Serial.println(relayPort6Status);
    break;
  case 7:
    relayPort7Status = status == 1 ? true : false;
    Serial.print("Toggled Port 7: ");
    Serial.println(relayPort7Status);
    break;
  case 8:
    relayPort8Status = status == 1 ? true : false;
    Serial.print("Toggled Port 8: ");
    Serial.println(relayPort8Status);
    break;
  }
  preferences.end();

  // Respond to the client
  server.send(200, "application/json", "{}");

  // Make sure pins are set properly based on new data
  setRelayPins();
}

void setupRouting()
{
  server.on("/toggle", HTTP_POST, postTogglePort);
  server.on("/interval", HTTP_POST, postHallInterval);
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
  relayPort1Status = preferences.getBool("relayPort1Status", false);
  relayPort2Status = preferences.getBool("relayPort2Status", false);
  relayPort3Status = preferences.getBool("relayPort3Status", false);
  relayPort4Status = preferences.getBool("relayPort4Status", false);
  relayPort5Status = preferences.getBool("relayPort5Status", false);
  relayPort6Status = preferences.getBool("relayPort6Status", false);
  relayPort7Status = preferences.getBool("relayPort7Status", false);
  relayPort8Status = preferences.getBool("relayPort8Status", false);
  preferences.end();

  pinMode(RELAY_PORT1_PIN, OUTPUT);
  pinMode(RELAY_PORT2_PIN, OUTPUT);
  pinMode(RELAY_PORT3_PIN, OUTPUT);
  pinMode(RELAY_PORT4_PIN, OUTPUT);
  pinMode(RELAY_PORT5_PIN, OUTPUT);
  pinMode(RELAY_PORT6_PIN, OUTPUT);
  pinMode(RELAY_PORT7_PIN, OUTPUT);
  pinMode(RELAY_PORT8_PIN, OUTPUT);

  pinMode(HALL_PORT1_PIN, INPUT);
  pinMode(HALL_PORT2_PIN, INPUT);
  pinMode(HALL_PORT3_PIN, INPUT);
  pinMode(HALL_PORT4_PIN, INPUT);
  pinMode(HALL_PORT5_PIN, INPUT);
  pinMode(HALL_PORT6_PIN, INPUT);
  pinMode(HALL_PORT7_PIN, INPUT);
  pinMode(HALL_PORT8_PIN, INPUT);

  // DHT 11
  preferences.begin("my-app", false);
  checkHallInterval = preferences.getULong("checkHallInterval", 600000);
  preferences.end();

  // WiFi
  initWifi();

  // Server
  setupRouting();

  // Set pins based on loaded preferences
  setRelayPins();

  Serial.println("Device Bootstrapping Complete");
}

void loop()
{
  handleClient();

  // shouldn't need this in the loop since
  // setup function loads last settings, and
  // the webserver is responsible for updating
  // setRelayPins();

  checkWiFi();

  readHall();

  // getData
  unsigned long currentMillis = millis();
  if (currentMillis - checkHallPreviousMillis >= checkHallInterval)
  {
    Serial.println("Checking Hall sensors compared to relay...");
    // std::string tempData = readDht();

    // Send Data to Server
    // postData(tempData);

    // Set previous send
    checkHallPreviousMillis = currentMillis;
  }

  // Wait 2sec before running again
  delay(2000);
}