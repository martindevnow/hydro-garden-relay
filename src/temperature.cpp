#include <sstream>
#include <string>
#include <temperature.h>
#include <network.h>

using namespace std;

// For Temp Sensor
#define DHT_PIN 26
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

template <typename T>
string tostr(const T &t)
{
  ostringstream os;
  os << t;
  return os.str();
}

void setupDht()
{
  dht.begin();
}

string readDht()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return NULL;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahrenheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  string tempC = tostr(t);
  string humid = tostr(h);

  string tempJson = "{\"temp\":\"" + tempC + "\",\"humid\":\"" + humid + "\",\"macAddress\":\"" + macAddress() + "\"}";

  // Print to test
  Serial.print("JSON: ");
  Serial.println(tempJson.c_str());

  return tempJson;
}