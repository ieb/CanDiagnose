#ifndef _NMEA2000_OUT_H
#define _NMEA2000_OUT_H

#include <NMEA2000.h>
#include <N2kTypes.h>

#include "temperature.h"
#include "config.h"


class Nmea2000Output {
    public:
        Nmea2000Output( tNMEA2000 *NMEA2000, 
            Temperature &temperature);
        void output();
        void begin(const char * configurationFile="/config.txt");
    private: 
        tNMEA2000 *NMEA2000;
        Temperature temperature;
        uint8_t pressureSID = 0;
        uint8_t temperatureSid = 0;
        uint8_t temperatureInstance[MAX_TEMPERATURE_SENSORS];
        uint8_t temperatureSource[MAX_TEMPERATURE_SENSORS];
        uint8_t pressureInstance;
        uint8_t humidityInstance;
        uint8_t serviceBatteryInstance;
        uint8_t serviceBatteryTemperatureSensor;
        uint8_t engineBatteryInstance;
        uint8_t engineBatteryTemperatureSensor;
        unsigned long temperaturePeriod;
        unsigned long pressurePeriod;
        unsigned long dcstatusPeriod;
        unsigned long lastTemperature;
        unsigned long lastPressure;
        unsigned long lastDCStatus;
};

#endif