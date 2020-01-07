
#pragma once
#include <SD.h>
#include <SPI.h>

#define SD_ENABLE 4
#define ENC true
#define SDC false


#define BASE_DOMAIN "es02.falcontech.co.id"
#define SUB_URL "/logger/add-record"
#define DEVICE_ID 1
#define DELAY_THREAD 5000


struct Config {
  int id;
  String hostname;
  String kPath;
  int ReportTime;
  byte IP[4];
  byte DNS[4];
  byte MAC[6];
  };



struct Data {
  char ID[64];
  float C;
  float F;
  float H;
  float IF;
  float IC;
  long date;
};

class Configuration {
  static byte MAC[];
  static byte ip[];
  static byte dns[];
  private:
    const char *filename = "Config/config.txt";  // <- SD library uses 8.3 filenames
  public:
    Config config;
    Configuration();
    void loadConfiguration(const char *filename);
    void saveConfiguration(const char *filename);
    void printFile(const char *filename);
};

class Log {
  public:
    Data data;
    void setup();
    void header_init(String filename);
    void write(String filename,float c,float h,float IC, String TS);

};
