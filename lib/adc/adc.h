#ifndef _ADCSENSOR_H_ 
#define _ADCSENSOR_H_


#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

#include "httpserver.h"
#include "display.h"
#define MAX_ADC_CHANNELS 4

  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

enum ChannelType {
  disabled,
  single,
  differential0_1,
  differential2_3,
};

typedef struct AdcChannel {
  ChannelType type = disabled;
  int no;
  adsGain_t gain;
  float scale;
  int16_t raw;
  float adcv;
  float voltage;

} AdcChannel;


class ADCSensor : public JsonOutput, public DisplayPage {
    public:
        ADCSensor(unsigned long readPeriod=5000) : readPeriod{readPeriod} {
          for(int i  = 0; i < MAX_ADC_CHANNELS; i++) {
            channel[i].gain = GAIN_ONE;
            channel[i].type = single;
          }
          channel[0].scale = 5.620350747;
          channel[1].scale = 5.5351667;
          channel[2].scale = 5.544718786;
          channel[3].scale = 5.548646647;
        };
        void begin(const char * configurationFile="/config.txt");
        void printVoltages();
        void outputJson(AsyncResponseStream *outputStream);
        bool drawPage(Adafruit_SSD1306 * display);
        void read();
    private:
        const char * asString(ChannelType t);
        const char * asString(adsGain_t t);
        Adafruit_ADS1115 ads;
        AdcChannel channel[MAX_ADC_CHANNELS];
        unsigned long lastRead = 0;
        unsigned long readPeriod = 5000;

};


#endif