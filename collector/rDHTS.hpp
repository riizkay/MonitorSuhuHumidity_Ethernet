class rDHTS : public ReportCollector{
  protected:
    void setup(){
        Serial.println("rdhts setup");
    }
    void update()override{
       Serial.print("RDHTS UPDATE ");
    }
    void grabParameter(Vector<ReportParam*> & paramCollections)override
    {
        paramCollections.PushBack(new ReportParam("temp","50"));
        paramCollections.PushBack(new ReportParam("hum","50"));
        
    }

    public:
    rDHTS():ReportCollector("DHTS")
    {}
};