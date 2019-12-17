class SensorCounter : public ReportCollector{
  protected:
    void update()override{

    }
    void grabParameter(Vector<ReportParam*> & paramCollections)override
    {
        paramCollections.PushBack(new ReportParam("name","counter barang"));
        paramCollections.PushBack(new ReportParam("value","10"));
    }
    void setup(){
        Serial.println("Counter setup");
    }
    public:
    SensorCounter():ReportCollector("Sensor Counter")
    {}
};