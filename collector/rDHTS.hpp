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
    float hic;
};

class rDHTS : public ReportCollector{
  private:
    int Device_Id;
    SensorData data;
  protected:
    void setup(){
        Serial.println("rdhts setup");
        dht.begin();
    }
    void update()override{
       Serial.print("RDHTS UPDATE ");
    }
    void grabParameter(Vector<ReportParam*> & paramCollections)override
    {
        
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
            
        }

        data.hic = dht.computeHeatIndex(data.t, data.h, false);
        
        paramCollections.PushBack(new ReportParam("dev_id",String(Device_Id)));
        paramCollections.PushBack(new ReportParam("temp",String(data.t)));
        paramCollections.PushBack(new ReportParam("hum",String(data.h)));
        paramCollections.PushBack(new ReportParam("iheat",String(data.hic)));
        //random(10, 20);

        // float t = random(220, 240)/10;
        // int h = random(40, 60);
        // float ih = random(220, 240)/10;
        // paramCollections.PushBack(new ReportParam("dev_id",String(Device_Id)));
        // paramCollections.PushBack(new ReportParam("temp",String(t)));
        // paramCollections.PushBack(new ReportParam("hum",String(h)));
        // paramCollections.PushBack(new ReportParam("iheat",String(ih)));
        
    }

    public:

    rDHTS(int dev_id):ReportCollector("DHTS")
    {
        Device_Id = dev_id;
    }

    float getTemp(){
        return data.t;
    }
    float getHum(){
        return data.h;
    }
    float getHic(){
        return data.hic;
    }

};