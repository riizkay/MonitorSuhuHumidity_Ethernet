#include <SPI.h>

#include "def.h"
#include "Reporter.h"

#include "Log.h"
#include "collector/rDHTS.hpp"
#include "Clock.h"

//#include "collector/SensorCounter.hpp"
#define BASE_DOMAIN "es02.falcontech.co.id"
#define SUB_URL "/logger/add-record"
#define DEVICE_ID 2004
#define DELAY_THREAD 5000
#define ENC_ENABLE 10
#define SD_ENABLE 4
#define ENC true
#define SDC false

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

Reporter * reporter = 0;
rDHTS collector1(DEVICE_ID);
//SensorCounter collector2(DEVICE_ID);
Log Logger;
Clock rtc;
Configuration conf;



void SPISelector(int en){
  pinMode(ENC_ENABLE,OUTPUT);
  pinMode(SD_ENABLE,OUTPUT);
  
  digitalWrite(ENC_ENABLE,!en);
  digitalWrite(SD_ENABLE,en);
}

void onResultReport(ReportCollector *sender,int err,Vector<ReportParam*> & paramCollections){
  String result = "result: " + (err == 0) ? "true" : "false";
  Serial.println(sender->getName());
  
  if(sender->getName()=="DHTS"){
    rDHTS *data = (rDHTS*)sender;
    Logger.write(("/Log/"+rtc.getYearMonthStr()+".csv"),data->getTemp(),data->getHum(),data->getHic(),rtc.getDateTimeString());
  }
  
  
}
void setup() {
  Serial.begin(9600);
  //pinMode(SD_ENABLE,OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  
  pinMode(ENC_ENABLE,OUTPUT);
  digitalWrite(ENC_ENABLE, HIGH);
  
  Logger.setup();
  //conf.saveConfiguration("/Config/config.txt");
  conf.loadConfiguration("/Config/config.txt");
  conf.printFile("/Config/config.txt");
  reporter = new Reporter(conf.config.MAC,conf.config.IP,conf.config.DNS, conf.config.hostname, conf.config.kPath);
  Serial.println(F("-----------Configuration Loaded-----------"));
  Serial.print(F("hostname Length : "));
  Serial.print(conf.config.hostname.length());
  Serial.print(F(" Content : "));
  Serial.println(conf.config.hostname);

  Serial.print(F("kPath Length : "));
  Serial.print(conf.config.kPath.length());
  Serial.print(F(" Content : "));
  Serial.println(conf.config.kPath);

  Serial.print(F("ReportTime : "));
  Serial.println(conf.config.ReportTime);

  Serial.print(F("Static Ip (if dhcp does not work) : "));
  for(int i=0;i<4;i++)
  {
    if(i!=0)
      Serial.print(".");
    Serial.print(conf.config.IP[i]);
  }
  Serial.println("");

  Serial.print(F("DNS Ip (if dhcp does not work) : "));
  for(int i=0;i<4;i++)
  {
    if(i!=0)
      Serial.print(".");
    Serial.print(conf.config.DNS[i]);
  }
  Serial.println("");
  Serial.print(F("MAC Address: "));
  for(int i=0;i<6;i++)
  {
    if(i!=0)
      Serial.print(":");
    Serial.print(conf.config.MAC[i],HEX);
    
  }
  Serial.println("");
  
  //while(1);
  reporter->OnResultReport = onResultReport;
  reporter->addCollector(&collector1);
  //reporter.addCollector(&collector2);  
  reporter->setup();
  
  rtc.setup();
}

void loop() {
  //SPISelector(ENC);
  reporter->update();
  delay(conf.config.ReportTime);

  //SPISelector(SDC);
}
