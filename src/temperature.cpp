#include <temperature.h>

// For Temp Sensor
#define DHT_PIN 26
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

void setupDht()
{
  dht.begin();
}

char *readDht()
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

  // convert to JSON and return it
  char *json;
  json = (char *)malloc(80);
  char humid[12];
  char tempC[12];

  // Convert Floats to String
  dtostrf(h, 4, 2, humid);
  dtostrf(t, 4, 2, tempC);

  // build JSON object
  strcat(strcpy(json, "{\"temp\":\""), tempC);
  strcat(json, "\",\"humid\":\"");
  strcat(json, humid);
  strcat(json, "\"}");

  // Print to test
  Serial.print("JSON: ");
  Serial.println(json);

  return json;
}