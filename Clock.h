#include <DS3231.h>
#include <Wire.h>
#include <arduino.h>


class Clock {
    private:
        DS3231 Clock;
    public:
    void init() {
        Wire.begin();    
    }
    int getYear(){
        return (Clock.getYear());      
    }
};
