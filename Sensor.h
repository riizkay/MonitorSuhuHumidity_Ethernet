#pragma once
#include <arduino.h>
#include <DHT.h>
#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

struct SensorData
{
    float h;
    float t;
    float f;
};


class Sensor {
    public:
        
        void init(){
            dht.begin();
        }

        SensorData getTemp(){
            SensorData data;
            data.h = dht.readHumidity();
            // Read temperature as Celsius (the default)
            data.t = dht.readTemperature();
            // Read temperature as Fahrenheit (isFahrenheit = true)
            data.f = dht.readTemperature(true);

            // Check if any reads failed and exit early (to try again).
            if (isnan(data.h) || isnan(data.t) || isnan(data.f)) {
                Serial.println(F("Failed to read from DHT sensor!"));
                data.f=-9999.0;
                data.h=-9999.0;
                data.t=-9999.0;
                return data;
            }
            return data;
        }
};
