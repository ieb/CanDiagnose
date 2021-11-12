#ifndef _ADCSENSOR_H_ 
#define _ADCSENSOR_H_


#include <Arduino.h>
#include "httpserver.h"
#include "esp_adc_cal.h"
#define MAX_ADC_CHANNELS 6
#define MA_SAMPLES 5
#define ADC_SAMPLES 64


class ADCSensor : public JsonOutput {
    public:
        ADCSensor() {

        };
        void begin();
        void checkCalibration();
        void outputJson(AsyncResponseStream *outputStream);
        void measure();
        void printVoltages();
    private:
        void read();
        uint16_t readRaw(int gpio);
        uint16_t convertMv(uint16_t raw);

        uint16_t voltages[MAX_ADC_CHANNELS];
        uint16_t adcRawReading[MAX_ADC_CHANNELS];
        int adcPins[MAX_ADC_CHANNELS] = { GPIO_NUM_36, GPIO_NUM_39, GPIO_NUM_34, GPIO_NUM_35, GPIO_NUM_32, GPIO_NUM_33 };
        unsigned long lastRead = 0;
        unsigned long readPeriod = 1000;

};


#endif