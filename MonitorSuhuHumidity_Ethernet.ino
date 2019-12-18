#include "def.h"
#include "Reporter.h"
#include "collector/rDHTS.hpp"
#include "Log.h"
#include "Clock.h"

#define DELAY_THREAD 5000
#define ENC_ENABLE 10
#define SD_ENABLE 4
#define ENC true
#define SDC false

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

Reporter  reporter(mac, BASE_DOMAIN);
rDHTS collector1;
Log logger;

void onResultReport(ReportCollector *sender,int err,Vector<ReportParam*> * paramCollections){
  String result = "result: " + (err == 0) ? "true" : "false";
  //Serial.println(sender->getName());
}
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  
  //reporter.OnResultReport = onResultReport;
  reporter.addCollector(&collector1);
  reporter.setup();
  //logger.setup();
  //logger.header_init();
}



void loop() {
  reporter.update();
  delay(DELAY_THREAD);
}
