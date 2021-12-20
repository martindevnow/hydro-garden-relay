#include <network.h>
#include <Credentials.h>

// For WiFi
String hostname = "ESP32 Node Temperature";

// For checking WiFi connectivity
unsigned long checkWiFiPreviousMillis = 0;
unsigned long checkWiFiInterval = 30000;

void initWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname.c_str());
  WiFi.begin(WIFI_SSID, WIFI_PW);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
}

void checkWiFi()
{
  // Check if still connected to WiFi
  unsigned long currentMillis = millis();

  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - checkWiFiPreviousMillis >= checkWiFiInterval))
  {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    checkWiFiPreviousMillis = currentMillis;
  }
}

// External API Server
const char *serverName = "http://192.168.0.250:3420/garden";
// For sending data to server

void postData(char *tempData)
{
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    Serial.print("Sending data: ");
    Serial.println(tempData);
    int httpResponseCode = http.POST(tempData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Free resources
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}