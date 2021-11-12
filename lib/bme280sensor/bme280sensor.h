#ifndef _BMP280SENSOR_H_ 
#define _BMP280SENSOR_H_


#include <SPI.h>
#include <Adafruit_BME280.h>
#include "httpserver.h"


class BME280Sensor : public JsonOutput {
    public:
        BME280Sensor(int sdaPin=GPIO_NUM_23, int sclPin=GPIO_NUM_22) : sdaPin{sdaPin}, sclPin{sclPin} {
            bme_temp = bme.getTemperatureSensor();
            bme_pressure = bme.getPressureSensor();
            bme_humidity = bme.getHumiditySensor();

        };
        void begin();
        void outputJson(AsyncResponseStream *outputStream);
    private:
        void read();
        Adafruit_BME280 bme=Adafruit_BME280(); 
        int sdaPin;
        int sclPin;
        unsigned long lastModified;
        Adafruit_Sensor *bme_temp;
        Adafruit_Sensor *bme_pressure;
        Adafruit_Sensor *bme_humidity;
        sensors_event_t temp_event;
        sensors_event_t pressure_event;
        sensors_event_t humidity_event;
};


#endif