

#include "adc.h"



void ADCSensor::begin() {
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
  } else {
    Serial.println("ADS initialized.");
  }
  // // 1x gain   +/- 4.096V  1 bit 0.125mV
  ads.setGain(GAIN_ONE);
}

void ADCSensor::read() {
    unsigned long now = millis();
    if ( now > lastRead + readPeriod )  {
        lastRead = now;
        for (int i = 0; i < MAX_ADC_CHANNELS; i++) {
            ads.setGain(gain[i]);
            raw[i] = ads.readADC_SingleEnded(i);
            adcv[i] = ads.computeVolts(raw[i]);
            voltage[i] = adcv[i]*scale[i];
        }
    }
}






void ADCSensor::printVoltages() {
    for (int i = 0; i < MAX_ADC_CHANNELS; i++) {
        ads.setGain(gain[i]);
        uint16_t reading = ads.readADC_SingleEnded(i);
        float adc = ads.computeVolts(reading);
        float v =  adc*scale[i];
        Serial.print("AIN");Serial.print(i);Serial.print(": "); 
        Serial.print(reading); Serial.print("  "); 
        Serial.print(adc,6); Serial.print("Vadc ");
        Serial.print(v,6); Serial.println("V");
    }
}

void ADCSensor::calibrate(float * calibrations, int n) {
    for(int i=0; i< n && i < MAX_ADC_CHANNELS; i++) {
        scale[i] = calibrations[i];
    }
}



void ADCSensor::outputJson(AsyncResponseStream *outputStream) {
    read();
    startJson(outputStream);
    append("t",millis());
    append("lastModified",lastRead);
    startArray("voltages");
    for (int i = 0; i < MAX_ADC_CHANNELS; i++){
        startObject();
        append("channel",i);
        append("adcr",raw[i]);
        append("adcv",adcv[i],6);
        append("v",voltage[i],6);
        endObject();
    }    
    endArray();
    endJson();
}
