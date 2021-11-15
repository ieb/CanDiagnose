#ifndef _TEMPERATURE_H_ 
#define _TEMPERATURE_H_

#include <OneWire.h>
#include <DallasTemperature.h>
#include "httpserver.h"

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


class Temperature : public JsonOutput {
    public:
        Temperature(int oneWirePin, unsigned long readPeriod = 10000L): 
            oneWirePin{oneWirePin}, 
            readPeriod{readPeriod} {};
        void outputJson(AsyncResponseStream *outputStream);
        void begin();
        void read();

    private:
        void outputSensorJson(AsyncResponseStream *outputStream, int si);
        void printSensor(int si);
        bool isAvailable(int si);
        int oneWirePin;
        OneWire oneWire;
        DallasTemperature sensors;
        TemperatureSensor temperatureSensor[MAX_TEMPERATURE_SENSORS];
        unsigned long lastRead = 0;
        unsigned long readPeriod = 10000;
        bool readRequested = false;

};


#endif