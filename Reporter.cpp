#include <b64.h>
#include <HttpClient.h>
#include "Reporter.h"
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
ReportParam::ReportParam(){

}
ReportParam::ReportParam( String name, String value){
    this->name = name;
    this->value = value;
}
Reporter::Reporter(void * mac, char * base_domain){
    this->mac = mac;
    this->base_domain = base_domain;
    Serial.println(this->base_domain);
}
Reporter::Reporter(){
    OnResultReport = 0;
}
Reporter::~Reporter(){
    
}
class setupPredicate : public Predicate<ReportCollector*>
{
   
    void operator() (ReportCollector* &element) { 
        element->setup();
    }
}setupfunc;
void Reporter::setup(){
    Ethernet.init(10);
    this->dataCollections.ForEach(setupfunc);
    Serial.println(F("init DHCP..."));
    
    while (Ethernet.begin((byte*)this->mac) != 1)
    {
        Serial.println(F("IP Error"));
        delay(1000);
    }  
    Serial.println(Ethernet.localIP());
}
void Reporter::addCollector(ReportCollector * dTemplate){
    dataCollections.PushBack(dTemplate);
    dTemplate->reporter = this;
    dTemplate->base_domain = this->base_domain;
}
class deleteRPPredicate : public Predicate<ReportParam*>
{
    public:

    void operator() (ReportParam* &element) { 
        delete element;
    }
}deleteRPPredicatefunc;

class updatePredicate : public Predicate<ReportCollector*>
{
    public:
    void operator() (ReportCollector* &element) { 
        Vector<ReportParam*> paramCollections;
        element->update();
        element->grabParameter(paramCollections);
        element->report(paramCollections);
        paramCollections.ForEach(deleteRPPredicatefunc);

    }
}updateFunc;

void Reporter::update(){
    this->dataCollections.ForEach(updateFunc);
}

void ReportCollector::setup(){
    
}

 String ReportCollector::getName(){
     return this->name;
 }

ReportCollector::ReportCollector(String name){
    this->name = name;
}

//namespace{
//        unsigned char h2int(char c)
//    {
//        if (c >= '0' && c <='9'){
//            return((unsigned char)c - '0');
//        }
//        if (c >= 'a' && c <='f'){
//            return((unsigned char)c - 'a' + 10);
//        }
//        if (c >= 'A' && c <='F'){
//            return((unsigned char)c - 'A' + 10);
//        }
//        return(0);
//    }
//    String urldecode(String str)
//    {
//        
//        String encodedString="";
//        char c;
//        char code0;
//        char code1;
//        for (int i =0; i < str.length(); i++){
//            c=str.charAt(i);
//        if (c == '+'){
//            encodedString+=' ';  
//        }else if (c == '%') {
//            i++;
//            code0=str.charAt(i);
//            i++;
//            code1=str.charAt(i);
//            c = (h2int(code0) << 4) | h2int(code1);
//            encodedString+=c;
//        } else{
//            
//            encodedString+=c;  
//        }
//        
//        yield();
//        }
//        
//    return encodedString;
//    }
//
//    String urlencode(String str)
//    {
//        String encodedString="";
//        char c;
//        char code0;
//        char code1;
//        char code2;
//        for (int i =0; i < str.length(); i++){
//        c=str.charAt(i);
//        if (c == ' '){
//            encodedString+= '+';
//        } else if (isalnum(c)){
//            encodedString+=c;
//        } else{
//            code1=(c & 0xf)+'0';
//            if ((c & 0xf) >9){
//                code1=(c & 0xf) - 10 + 'A';
//            }
//            c=(c>>4)&0xf;
//            code0=c+'0';
//            if (c > 9){
//                code0=c - 10 + 'A';
//            }
//            code2='\0';
//            encodedString+='%';
//            encodedString+=code0;
//            encodedString+=code1;
//            //encodedString+=code2;
//        }
//        yield();
//        }
//        return encodedString;
//        
//    }
//
//
//}
class paramBuilderPredicate : public Predicate<ReportParam*>
{
    public:
   String *result;
    void operator() (ReportParam* &element) { 
        if (result->length() > 0)
            *result += "&";
        *result += element->name + "=" + element->value;
    }
}paramBuilderfunc;
void ReportCollector::report(Vector<ReportParam*> & paramCollections){
         
    int err =0;
    EthernetClient Clinet;
    HttpClient http(Clinet);
    int kNetworkTimeout = 5*1000;
    int kNetworkDelay = 1000;

    String queryResult = "";
    paramBuilderfunc.result = &queryResult;
    paramCollections.ForEach(paramBuilderfunc);
   
    queryResult = "arduino.php?" + queryResult;
    //String fullURL = *this->base_domain + queryResult;
    Serial.println(BASE_DOMAIN);
    err = http.get("http://es01.falcontech.co.id", queryResult.c_str());
 
    if (err == 0)
    {
        err = http.skipResponseHeaders();
            Serial.print("Return Code :");
            Serial.println(http.responseStatusCode());
            
    if (err >= 0)
      {
        int bodyLen = http.contentLength();
         Serial.print(F("L"));
         Serial.println(bodyLen);
        // Serial.println();
        // Serial.println("Body returned follows:");
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout) )
        {
            if (http.available())
            {
                c = http.read();
                // Print out this character
                //Serial.print(c);
               
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                delay(kNetworkDelay);
            }
        }
      }
      else
      {
//        Serial.print("Failed to skip response headers: ");
//        Serial.println(err);
      }
    }
    if (this->reporter->OnResultReport != 0){
        this->reporter->OnResultReport(this, (err),&paramCollections);
    }
    
}
