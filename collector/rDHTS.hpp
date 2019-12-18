
#include <arduino.h>
#include "../Sensor.h"
class rDHTS : public ReportCollector{
  protected:
    void setup(){
        Serial.println(F("rdhts init"));
        
    }
    void update()override{
        Serial.println(F("Report"));
        
    }
    void grabParameter(Vector<ReportParam*> & paramCollections)override
    {
        paramCollections.PushBack(new ReportParam("temp","50"));
        paramCollections.PushBack(new ReportParam("hum","50"));
        
    }

    public:
    rDHTS():ReportCollector(F("DHTS"))
    {}
};