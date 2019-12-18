#include <SD.h>
#include <SPI.h>

#define ENC_ENABLE 10
#define SD_ENABLE 4
#define ENC true
#define SDC false

// Field
// ID | Temp (C) | Temp (F) | Humi | Index Fahrenheit | Index Celsius | Date

struct Data {
  char ID[64];
  float C;
  float F;
  float H;
  float IF;
  float IC;
  long date;
};

class Log {
  public:
    const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
    Data data;
    void setup() {
      Serial.print(F("Initializing SD card..."));

      // see if the card is present and can be initialized:
      if (!SD.begin(SD_ENABLE)) {
        Serial.println(F("Card failed, or not present"));
        // don't do anything more:
        delay(1000);
      }
      Serial.println(F("card initialized."));
    }

    void header_init() {
      if(SD.exists("datalog.csv"))
        return false;
      // make a string for assembling the data to log:
      char dataString[14];
      sprintf(dataString,"C,H,IF,IC,D");
      
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      
      File dataFile = SD.open("datalog.csv");

      // if the file is available, write to it:
      if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.println(F("Log Err"));
      }
    }

    void write(float c,float f,float h,float IF, float IC, long TS) {
      // make a string for assembling the data to log:
      String dataString = String(c) + ",";
      dataString += String(c) + ",";
      dataString += String(f) + ",";
      dataString += String(h) + ",";
      dataString += String(IF) + ",";
      dataString += String(IC) + ",";
      dataString += String(TS) + ",";
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      File dataFile = SD.open("datalog.csv", FILE_WRITE);

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

};
