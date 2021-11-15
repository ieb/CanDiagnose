#ifndef _BMP280SENSOR_H_ 
#define _BMP280SENSOR_H_


#include <SPI.h>
#include <Adafruit_BME280.h>
#include "httpserver.h"


class BME280Sensor : public JsonOutput {
    public:
        BME280Sensor(int sdaPin=GPIO_NUM_23, int sclPin=GPIO_NUM_22, unsigned long readPeriod=10000) : 
                sdaPin{sdaPin}, sclPin{sclPin}, readPeriod{readPeriod} {
            bme_temp = bme.getTemperatureSensor();
            bme_pressure = bme.getPressureSensor();
            bme_humidity = bme.getHumiditySensor();

        };
        void begin();
        void read();
        void outputJson(AsyncResponseStream *outputStream);
    private:
        Adafruit_BME280 bme=Adafruit_BME280(); 
        int sdaPin;
        int sclPin;
        unsigned long readPeriod = 10000;
        Adafruit_Sensor *bme_temp;
        Adafruit_Sensor *bme_pressure;
        Adafruit_Sensor *bme_humidity;
        sensors_event_t temp_event;
        sensors_event_t pressure_event;
        sensors_event_t humidity_event;
        unsigned long lastRead = 0;

};


#endif