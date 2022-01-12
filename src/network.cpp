#include <string>
#include <network.h>
#include <Credentials.h>

using namespace std;

// For WiFi
String hostname = "ESP32 Node Temperature";

// For checking WiFi connectivity
unsigned long checkWiFiPreviousMillis = 0;
unsigned long checkWiFiInterval = 30000;

// External API Server
// const char *serverName = "http://192.168.0.250:3334";
const char *attendanceEndpoint = "http://192.168.0.250:3334/devices";
const char *tempEndpoint = "http://192.168.0.250:3334/devices/temp";

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

// For sending Temperature Data to server
void postData(std::string tempData)
{
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, tempEndpoint);

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    Serial.print("Sending Temperature: ");
    Serial.println(tempData.c_str());
    int httpResponseCode = http.PUT(tempData.c_str());

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

// For registering this device with the server
void postAttendance()
{
  Serial.println("In postAttendance function");

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("WiFI is Connected");
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, attendanceEndpoint);

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    // Build Body
    string attendanceJson = "{\"macAddress\":\"" + macAddress() + "\"}";

    Serial.print("Sending Attendance: ");
    Serial.println();
    int httpResponseCode = http.POST(attendanceJson.c_str());

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

string macAddress()
{
  return string(WiFi.macAddress().c_str());
}

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

  delay(5000);

  // Register Device with Server
  postAttendance();
}
