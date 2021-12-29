#ifndef _LOGBOOK_H
#define _LOGBOOK_H

#include <SPIFFS.h>
#include "dataoutput.h"
#include "bme280sensor.h"
#include "adc.h"




class LogBook {
    public:
        LogBook(DataCollector &dataCollector, BME280Sensor &bme280, ADCSensor &adc);
        void log();
        void demoMode();
    private:
        double relativeAngleToDeg(double a);
        double headingAngleToDeg(double a);

        DataCollector &dataCollector;
        BME280Sensor &bme280;
        ADCSensor &adc;
        unsigned long lastLogUpdate = 0;
        unsigned long logPeriod = 300000; // every 5m, 288 log entries per day.
        unsigned long lastDemoUpdate = 0;
};

#endif