#ifndef _LOGBOOK_H
#define _LOGBOOK_H

#include <SPIFFS.h>
#include "dataoutput.h"




class LogBook {
    public:
        LogBook(DataCollector &dataCollector);
        void log();
        void demoMode();
    private:
        double relativeAngleToDeg(double a);
        double headingAngleToDeg(double a);

        DataCollector &dataCollector;
        unsigned long lastLogUpdate = 0;
        unsigned long logPeriod = 300000; // every 5m, 288 log entries per day.
        unsigned long lastDemoUpdate = 0;
};

#endif