#ifndef _TEMPERATURE_H_ 
#define _TEMPERATURE_H_

#include <OneWire.h>
#include <DallasTemperature.h>
#include "httpserver.h"
#include <NMEA2000.h>
#include <N2kMessages.h>

#define MAX_TEMPERATURE_SENSORS 5
#define TEMPERATURE_PRECISION 12

typedef struct TemperatureSensor {
        DeviceAddress address;
        float temperature;
        int8_t id;
        int8_t connected;
        unsigned long lastModified;
        unsigned long readTime;
} TemperatureSensor;


class Temperature : public JsonOutput, public CsvOutput {
    public:
        Temperature(tNMEA2000 *NMEA2000, int oneWirePin, unsigned long readPeriod = 10000L): 
            NMEA2000{NMEA2000},
            oneWirePin{oneWirePin}, 
            readPeriod{readPeriod} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
        void begin(const char * configurationFile="/config.txt");
        void read();
        void output();

    private:
        void outputSensorJson(AsyncResponseStream *outputStream, int si);
        void printSensor(int si);
        bool isAvailable(int si);
        tNMEA2000 *NMEA2000;
        TemperatureSensor temperatureSensor[MAX_TEMPERATURE_SENSORS];
        int oneWirePin;
        OneWire oneWire;
        DallasTemperature sensors;
        unsigned long lastRead = 0;
        unsigned long readPeriod = 10000;
        bool readRequested = false;
        uint8_t temperatureSid = 0;
        uint8_t temperatureInstance[MAX_TEMPERATURE_SENSORS];
        uint8_t temperatureSource[MAX_TEMPERATURE_SENSORS];
        unsigned long temperaturePeriod;
        unsigned long lastTemperature;


};


#endif