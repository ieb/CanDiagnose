

#include "adc.h"
#include "config.h"



void ADCSensor::begin(const char * configurationFile) {
  String v;
  if ( ConfigurationFile::get(configurationFile, "adc.gain", v)) {
      int s = 0, iadc = 0;
      int e = v.indexOf(',');
      if ( e < 0 ) {
          e = v.length();
      }
      Serial.print("Gains ");
      while(s < v.length() && iadc < MAX_ADC_CHANNELS) {
          Serial.print(iadc==0?"":",");
          int g = v.substring(s,e).toInt();
          switch(g) {
              case 1: 
                gain[iadc] = GAIN_ONE; 
                Serial.print("one");
              break;
              case 2: 
                gain[iadc] = GAIN_TWO; 
                Serial.print("two");
              break;
              case 4: 
                gain[iadc] = GAIN_FOUR; 
                Serial.print("four");
              break;
              case 8: 
                gain[iadc] = GAIN_EIGHT; 
                Serial.print("eight");
              break;
              case 16: 
                gain[iadc] = GAIN_SIXTEEN; 
                Serial.print("sixteen");
              break;
              default:
                String vs = v.substring(s,e);
                if ( vs.equalsIgnoreCase("2/3")) {
                    gain[iadc] = GAIN_TWOTHIRDS; 
                    Serial.print("two thirds");
                } else {
                    Serial.printf("%s ?? defailt to 1 ", vs.c_str());
                    gain[iadc] = GAIN_ONE; 
                }
          }
          iadc++;
          s = e+1;
          e = v.indexOf(',',s);
          if ( e < 0 ) {
              e = v.length();
          }
      }
      Serial.println("");
  } else {
      Serial.println("No gain settings using defaults.");
  }
  if ( ConfigurationFile::get(configurationFile, "adc.scale", v)) {
      int s = 0, iadc = 0;
      int e = v.indexOf(',');
      if ( e < 0) {
          e = v.length();
      }
      Serial.print("Scales ");
      while( s < v.length() && iadc < MAX_ADC_CHANNELS) {
          scale[iadc] = v.substring(s,e).toDouble();
          Serial.print(iadc==0?"":",");
          Serial.print(scale[iadc],6);
          iadc++;
          s = e+1;
          e = v.indexOf(',',s);
          if ( e < 0 ) {
              e = v.length();
          }
      }
      Serial.println("");
  } else {
      Serial.println("No scale settings using defaults.");
  }
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
