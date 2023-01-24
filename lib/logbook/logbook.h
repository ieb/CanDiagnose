#ifndef _LOGBOOK_H
#define _LOGBOOK_H

#include <SPIFFS.h>
#include "N2KCollector.h"




class LogBook {
    public:
        LogBook(N2KCollector &n2kCollector);
        void log();
        void demoMode();
    private:
        double relativeAngleToDeg(double a);
        double headingAngleToDeg(double a);

        N2KCollector &n2kCollector;
        unsigned long lastLogUpdate = 0;
        unsigned long logPeriod = 300000; // every 5m, 288 log entries per day.
        unsigned long lastDemoUpdate = 0;
};

#endif