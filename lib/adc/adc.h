#ifndef _ADCSENSOR_H_ 
#define _ADCSENSOR_H_


#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

#include "httpserver.h"
#define MAX_ADC_CHANNELS 4

  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV




class ADCSensor : public JsonOutput {
    public:
        ADCSensor(unsigned long readPeriod=5000) : readPeriod{readPeriod} {};
        void begin(const char * configurationFile="/config.txt");
        void printVoltages();
        void outputJson(AsyncResponseStream *outputStream);
        void read();
    private:
        Adafruit_ADS1115 ads;
        int16_t raw[MAX_ADC_CHANNELS];
        float adcv[MAX_ADC_CHANNELS];
        float voltage[MAX_ADC_CHANNELS];
        adsGain_t gain[MAX_ADC_CHANNELS] = {GAIN_ONE, GAIN_ONE, GAIN_ONE, GAIN_ONE};
        float scale[MAX_ADC_CHANNELS] = {5.620350747, 5.5351667, 5.544718786, 5.548646647};
        unsigned long lastRead = 0;
        unsigned long readPeriod = 5000;

};


#endif