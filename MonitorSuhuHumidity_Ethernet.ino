#include <SPI.h>
#include "def.h"
#include "Reporter.h"
#include "collector/rDHTS.hpp"
#include "collector/SensorCounter.hpp"
#define BASE_DOMAIN "oriskin.falcontech.co.id"
#define DELAY_THREAD 5000
#define ENC_ENABLE 10
#define SD_ENABLE 4
#define ENC true
#define SDC false
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

Reporter  reporter(mac, BASE_DOMAIN);
rDHTS collector1;
SensorCounter collector2;

void SPISelector(int en){
  pinMode(ENC_ENABLE,OUTPUT);
  pinMode(SD_ENABLE,OUTPUT);
  
  digitalWrite(ENC_ENABLE,!en);
  digitalWrite(SD_ENABLE,en);
}

void onResultReport(ReportCollector *sender,int err,Vector<ReportParam*> & paramCollections){
  String result = "result: " + (err == 0) ? "true" : "false";
  Serial.println(sender->getName());
}
void setup() {
  reporter.OnResultReport = onResultReport;
  reporter.addCollector(&collector1);
  reporter.addCollector(&collector2);
  Serial.begin(9600);
  reporter.setup();
}

void loop() {
  SPISelector(ENC);
  reporter.update();
  delay(DELAY_THREAD);

  SPISelector(SDC);
}
