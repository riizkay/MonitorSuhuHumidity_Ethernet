#include <arduino.h>
#include "vector.h"


struct ReportParam{
    String name;
    String value;
    ReportParam();
    ReportParam( String name, String value);
};
class Reporter;
class ReportCollector{
    friend class updatePredicate;
    friend class setupPredicate;
    friend class Reporter;
private:
       String * base_domain;
       String * sub_url;
       String name;
protected:
    Reporter * reporter;
    virtual void update() = 0;
    virtual void grabParameter(Vector<ReportParam*> & paramCollections) = 0;
    virtual void setup() =0;
public:
    String getName();
    ReportCollector(String name);
    void report(Vector<ReportParam*> & paramCollections);
    
};
class Reporter {
private:
    void *  mac;
    String base_domain;
    String sub_url;
    Vector<ReportCollector*> dataCollections;
public:
    void  (*OnResultReport)(ReportCollector *sender,int errCode,Vector<ReportParam*> & paramCollections);
    Reporter(void * mac, String base_domain, String sub_url );
    Reporter();
    ~Reporter();
    void setup();
    void update();
    void addCollector(ReportCollector * dTemplate);
};
