#include "Reporter.h"
#include <SPI.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>
ReportParam::ReportParam(){

}
ReportParam::ReportParam( String name, String value){
    this->name = name;
    this->value = value;
}
Reporter::Reporter(void * mac,void * ip, void * dns, String base_domain,String sub_url){
    this->mac = mac;
    this->ip = ip;
    this->dns = dns;
    this->base_domain = base_domain;
    this->sub_url = sub_url; 
    //Serial.println(this->base_domain.c_str());
}
Reporter::Reporter(void * mac, String base_domain,String sub_url){
    this->mac = mac;
    this->base_domain = base_domain;
    this->sub_url = sub_url; 
    //Serial.println(this->base_domain.c_str());
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
  repeat:
  Ethernet.init(ENC_ENABLE);
  this->dataCollections.ForEach(setupfunc);
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      goto repeat;
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
      goto repeat;
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, dns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
}
void Reporter::addCollector(ReportCollector * dTemplate){
    dataCollections.PushBack(dTemplate);
    dTemplate->reporter = this;
    dTemplate->base_domain = &this->base_domain;
    dTemplate->sub_url = &this->sub_url;
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

namespace{
        unsigned char h2int(char c)
    {
        if (c >= '0' && c <='9'){
            return((unsigned char)c - '0');
        }
        if (c >= 'a' && c <='f'){
            return((unsigned char)c - 'a' + 10);
        }
        if (c >= 'A' && c <='F'){
            return((unsigned char)c - 'A' + 10);
        }
        return(0);
    }
    String urldecode(String str)
    {
        
        String encodedString="";
        char c;
        char code0;
        char code1;
        for (int i =0; i < str.length(); i++){
            c=str.charAt(i);
        if (c == '+'){
            encodedString+=' ';  
        }else if (c == '%') {
            i++;
            code0=str.charAt(i);
            i++;
            code1=str.charAt(i);
            c = (h2int(code0) << 4) | h2int(code1);
            encodedString+=c;
        } else{
            
            encodedString+=c;  
        }
        
        yield();
        }
        
    return encodedString;
    }

    String urlencode(String str)
    {
        String encodedString="";
        char c;
        char code0;
        char code1;
        char code2;
        for (int i =0; i < str.length(); i++){
        c=str.charAt(i);
        if (c == ' '){
            encodedString+= '+';
        } else if (isalnum(c)){
            encodedString+=c;
        } else{
            code1=(c & 0xf)+'0';
            if ((c & 0xf) >9){
                code1=(c & 0xf) - 10 + 'A';
            }
            c=(c>>4)&0xf;
            code0=c+'0';
            if (c > 9){
                code0=c - 10 + 'A';
            }
            code2='\0';
            encodedString+='%';
            encodedString+=code0;
            encodedString+=code1;
            //encodedString+=code2;
        }
        yield();
        }
        return encodedString;
        
    }


}
class paramBuilderPredicate : public Predicate<ReportParam*>
{
    public:
   String *result;
    void operator() (ReportParam* &element) { 
        if (result->length() > 0)
            *result += "&";
        *result += urlencode(element->name) + "=" + urlencode(element->value);
    }
}paramBuilderfunc;
void ReportCollector::report(Vector<ReportParam*> & paramCollections){
         
    int err =0;
    EthernetClient c;
    HttpClient http(c);
    int kNetworkTimeout = 30*1000;
    int kNetworkDelay = 1000;

    String queryResult = "";
    paramBuilderfunc.result = &queryResult;
    paramCollections.ForEach(paramBuilderfunc);
   
    queryResult = *this->sub_url + "?" + queryResult;
    String fullURL = *this->base_domain + queryResult;
    Serial.println(fullURL);
    err = http.get(this->base_domain->c_str(), queryResult.c_str());
    Serial.print("Response : ");
    Serial.println(http.responseStatusCode());
    if (err == 0)
    {
        err = http.skipResponseHeaders();
        Serial.println(err);
    if (err >= 0)
      {
        
        int bodyLen = http.contentLength();
         Serial.print("Content length is: ");
         Serial.println(bodyLen);
         Serial.println();
         Serial.println("Body returned follows:");
      
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
                Serial.print(c);
               
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
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
      http.stop();
    }
    if (this->reporter->OnResultReport != 0){
        this->reporter->OnResultReport(this, (err),paramCollections);
    }
}
