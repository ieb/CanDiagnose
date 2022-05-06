#ifndef _BMP280SENSOR_H_ 
#define _BMP280SENSOR_H_


#include <SPI.h>
#include <Adafruit_BME280.h>
#include "httpserver.h"
#include "display.h"


class BME280Sensor : public JsonOutput, public CsvOutput, public DisplayPage, public History128over24 {
    public:
        BME280Sensor(unsigned long readPeriod=10000) : 
                History128over24{800.0,100.0,675000}, readPeriod{readPeriod} {
            bme_temp = bme.getTemperatureSensor();
            bme_pressure = bme.getPressureSensor();
            bme_humidity = bme.getHumiditySensor();

        };
        void begin();
        void read();
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
        bool drawPage(Adafruit_SSD1306 * display);
        double getPressure() { return pressure_event.pressure; }
        double getHumidity() { return humidity_event.relative_humidity; }

    private:
        Adafruit_BME280 bme=Adafruit_BME280(); 
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