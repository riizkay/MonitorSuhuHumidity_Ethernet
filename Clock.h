#pragma once
#include <DS3231.h>
#include <Wire.h>
#include <arduino.h>



class Clock {
    private:
        bool Century=false;
        bool h12;
        bool PM;
        DS3231 Clock;
    public:
    void setup() {
        Wire.begin();    
    }


    String getYearMonthStr(){
      return (
        String((Clock.getYear()+2000),DEC) +
        String(Clock.getMonth(Century),DEC)
        );
    }
    
    int printClock(){
        delay(1000);
        Clock.setClockMode(false);  // set to 24h
        Serial.print(Clock.getYear(), DEC);
        Serial.print("-");
        Serial.print(Clock.getMonth(Century), DEC);
        Serial.print("-");
        Serial.print(Clock.getDate(), DEC);
        Serial.print(" ");
        Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
        Serial.print(":");
        Serial.print(Clock.getMinute(), DEC);
        Serial.print(":");
        Serial.println(Clock.getSecond(), DEC);    
    }

    String getDateString(){
      String dateStr = String(Clock.getMonth(Century), DEC) +
      "/" +
      String(Clock.getDate(), DEC) +
      "/" +
      String((Clock.getYear()+2000), DEC);
      
      return (
      dateStr
      );
    }

    String getDateTimeString(){
        Clock.setClockMode(false);  // set to 24h
        String datetimestr = String(Clock.getYear(), DEC)+
          "-" +
        String(Clock.getMonth(Century), DEC)+
          "-" +
        String(Clock.getDate(), DEC)+ 
          " " +
        String(Clock.getHour(h12, PM), DEC)+ //24-hr
          ":" +
        String(Clock.getMinute(), DEC)+
          ":" +
        String(Clock.getSecond(), DEC);    

        return datetimestr;
    }
};
