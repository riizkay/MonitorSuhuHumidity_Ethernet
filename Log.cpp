#include "Log.h"
#include <ArduinoJson.h>
byte Configuration::MAC[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte Configuration::ip[] = { 192, 168, 1, 12};
byte Configuration::dns[] = { 192, 168, 1, 1};


void Configuration::loadConfiguration(const char *filename) {
      File file = SD.open(filename);
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, file);
      if (error){
                Serial.println(F("Failed to read Config file."));
                //while(1);
      }
      strlcpy(config.hostname.c_str(),                  // <- destination
              doc["hostname"] | BASE_DOMAIN,  // <- source
              config.hostname.length()+1);         // <- destination's capacity
              
      strlcpy(config.kPath.c_str(),                  // <- destination
              doc["kPath"] | SUB_URL,  // <- source
              config.kPath.length()+1);         // <- destination's capacity
      config.ReportTime = doc["ReportTime"] | DELAY_THREAD;
      for(int i=0;i<sizeof(config.IP);i++)
        config.IP[i] = doc["IP"][i];
      for(int i=0;i<sizeof(config.DNS);i++)
        config.DNS[i] = doc["DNS"][i];
      for(int i=0;i<sizeof(config.MAC);i++)
        config.MAC[i] = doc["MAC"][i];
      file.close();
    }

String IPArrToString(char *ip){
    for(int i=0; i<sizeof(ip);i++){

    }
}

void Configuration::saveConfiguration(const char *filename) {
      // Delete existing file, otherwise the configuration is appended to the file
      SD.remove(filename);
    
      // Open file for writing
      File file = SD.open(filename, FILE_WRITE);
      if (!file) {
        Serial.println(F("Failed to create file"));
        return;
      }
      StaticJsonDocument<512> doc;
      doc["hostname"] = config.hostname;
      doc["kPath"] = config.kPath;
      doc["ReportTime"] = config.ReportTime;
      JsonArray IPAddr = doc.createNestedArray("IP");
      for(int i=0;i<sizeof(config.IP);i++)
        IPAddr.add(config.IP[i]);
      
      JsonArray DNS = doc.createNestedArray("DNS");
      for(int i=0;i<sizeof(config.DNS);i++)
        DNS.add(config.DNS[i]);
      
      JsonArray MACAddr = doc.createNestedArray("MAC");
      for(int i=0;i<sizeof(config.MAC);i++)
        MACAddr.add(config.MAC[i]);
      
      
      if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write to file"));
      }
      file.close();
}


 Configuration::Configuration(){
      config.id = DEVICE_ID;
      config.hostname = BASE_DOMAIN;
      config.kPath = SUB_URL;
      config.ReportTime = DELAY_THREAD;
      memcpy(config.IP, ip, sizeof(ip));
      memcpy(config.DNS, dns,sizeof(dns));
      memcpy(config.MAC, MAC,sizeof(MAC));
     
}



void Configuration::printFile(const char *filename) {
      // Open file for reading
      File file = SD.open(filename);
      if (!file) {
        Serial.println(F("Failed to read file"));
        return;
      }
    
      // Extract each characters by one by one
      while (file.available()) {
        Serial.print((char)file.read());
      }
      Serial.println();
    
      // Close the file
      file.close();
}




void Log::setup() {
    Serial.print(F("Initializing SD card..."));

    // see if the card is present and can be initialized:
    if (!SD.begin(SD_ENABLE)) {
    Serial.println(F("Card failed, or not present"));
    // don't do anything more:
    delay(1000);
    }
    Serial.println(F("card initialized."));
    //header_init();
    SD.mkdir("Log");
    SD.mkdir("Config");
}

 void Log::header_init(String filename) {
    if(SD.exists(filename.c_str()))
    return;
    // make a string for assembling the data to log:
    
    String dataString = "Temperature (Celcius);Humidity (%);Index Temperature (Celcius);Date Time";
    Serial.println(F("datalog doesn't exist, Creating file"));
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    Serial.print(F("Write Header"));
    Serial.println(filename);
    File dataFile = SD.open(filename.c_str(), FILE_WRITE);
    dataFile.println(dataString);
    dataFile.close();
    Serial.print(F("Write Header Success"));
    Serial.println(filename);
}


void Log::write(String filename,float c,float h,float IC, String TS) {
      header_init(filename);
      // make a string for assembling the data to log:
      String dataString = String(c) + ";";
      dataString += String(h) + ";";
      dataString += String(IC) + ";";
      dataString += (TS) + ";";
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      File dataFile = SD.open(filename.c_str(), FILE_WRITE);

      // if the file is available, write to it:
      if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
        // print to the serial port too:
        Serial.println(dataString);
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.println("error opening datalog.txt");
      }
    }
