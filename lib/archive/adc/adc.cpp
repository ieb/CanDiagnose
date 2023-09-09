

#include "adc.h"
#include "config.h"



void ADCSensor::begin(const char * configurationFile) {
  String v;
  for (int i = 0; i < MAX_ADC_CHANNELS; i++) {
      String adcId = "adc";
      adcId = adcId + i;
      v = "";
      if ( ConfigurationFile::get(configurationFile, adcId+".type", v)) {
          if ( v.equals("single") ) {
              channel[i].type = single;
          } else if ( v.equals("diff0_1")) {
              channel[i].type = differential0_1;
              channel[i].no = 10;
          } else if ( v.equals("diff2_3")) {
              channel[i].type = differential2_3;
              channel[i].no = 32;
          } else {
              channel[i].type = disabled;
              channel[i].no = -1;
          }
        v = "";
        if (channel[i].type == single ) {
            if ( ConfigurationFile::get(configurationFile, adcId+".no", v)) {
                channel[i].no = v.toInt();
            } else {
                channel[i].no = i;
            }
        }
        v = "";
        if ( ConfigurationFile::get(configurationFile, adcId+".scale", v)) {
                channel[i].scale = v.toDouble();
        } else {
                channel[i].scale = 1.0;
        }
        v = "";
        if ( ConfigurationFile::get(configurationFile, adcId+".gain", v)) {
            int gain = v.toInt();
            switch (gain) {
                case 1: channel[i].gain = GAIN_ONE; break;
                case 2: channel[i].gain = GAIN_TWO; break;
                case 4: channel[i].gain = GAIN_FOUR; break;
                case 8: channel[i].gain = GAIN_EIGHT; break;
                case 16: channel[i].gain = GAIN_SIXTEEN; break;
                default:
                    if (v.equals("2/3") ) {
                        channel[i].gain = GAIN_TWOTHIRDS;
                    } else {
                        channel[i].gain = GAIN_ONE;
                    }
            }
        } else {
                channel[i].gain = GAIN_ONE;
        }
    } else {
        channel[i].type = disabled;
        channel[i].no = -1;
        channel[i].scale = 1.0;
        channel[i].gain = GAIN_ONE;
      }

      Serial.printf("Ch %d:%d, type=%s, gain=%s, scale=%f \n", 
                         i,channel[i].no,
                                    asString(channel[i].type),
                                            asString(channel[i].gain), 
                                                       channel[i].scale);

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
    if ( (now-lastRead) > readPeriod )  {
        lastRead = now;
        for (int i = 0; i < MAX_ADC_CHANNELS; i++) {
            switch(channel[i].type) {
                case single:
                    ads.setGain(channel[i].gain);
                    channel[i].raw = ads.readADC_SingleEnded(channel[i].no);
                    channel[i].adcv = ads.computeVolts(channel[i].raw);
                    channel[i].voltage = channel[i].adcv*channel[i].scale;
                    break;
                case differential0_1:
                    ads.setGain(channel[i].gain);
                    channel[i].raw = ads.readADC_Differential_0_1();
                    channel[i].adcv = ads.computeVolts(channel[i].raw);
                    channel[i].voltage = channel[i].adcv*channel[i].scale;
                    break;
                case differential2_3:
                    ads.setGain(channel[i].gain);
                    channel[i].raw = ads.readADC_Differential_2_3();
                    channel[i].adcv = ads.computeVolts(channel[i].raw);
                    channel[i].voltage = channel[i].adcv*channel[i].scale;
                    break;
                case disabled:
                    break;
            }
        }
        storeHistory(getServiceVoltage());
    }
}






void ADCSensor::printVoltages() {
    for (int i = 0; i < MAX_ADC_CHANNELS; i++) {
        switch(channel[i].type) {
            case single:
                ads.setGain(channel[i].gain);
                channel[i].raw = ads.readADC_SingleEnded(channel[i].no);
                channel[i].adcv = ads.computeVolts(channel[i].raw);
                channel[i].voltage = channel[i].adcv*channel[i].scale;
                Serial.print("Single");Serial.print(channel[i].no);Serial.print(": "); 
                break;
            case differential0_1:
                ads.setGain(channel[i].gain);
                channel[i].raw = ads.readADC_Differential_0_1();
                channel[i].adcv = ads.computeVolts(channel[i].raw);
                channel[i].voltage = channel[i].adcv*channel[i].scale;
                Serial.print("differential0_1: ");
                break;
            case differential2_3:
                ads.setGain(channel[i].gain);
                channel[i].raw = ads.readADC_Differential_2_3();
                channel[i].adcv = ads.computeVolts(channel[i].raw);
                channel[i].voltage = channel[i].adcv*channel[i].scale;
                Serial.print("differential2_3: "); 
                break;
            case disabled:
                Serial.print("disabled: "); 

        }
        Serial.print(channel[i].raw); Serial.print("  "); 
        Serial.print(channel[i].adcv,6); Serial.print("Vadc ");
        Serial.print(channel[i].voltage ,6); Serial.println("V");
    }
}

const char * ADCSensor::asString(ChannelType t) {
    switch(t) {
        case single: return "single";
        case differential0_1: return "differential0_1";
        case differential2_3: return "differential2_3";
        default: return "disabled";
    }
}

const char * ADCSensor::asString(adsGain_t t) {
    switch(t) {
        case GAIN_ONE: return "1";
        case GAIN_TWO: return "2";
        case GAIN_FOUR: return "4";
        case GAIN_EIGHT: return "8";
        case GAIN_SIXTEEN: return "16";
        case GAIN_TWOTHIRDS: return "2/3";
        default: return "1";
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
        append("type",asString(channel[i].type));
        append("adcr",channel[i].raw);
        append("adcv",channel[i].adcv,6);
        append("v",channel[i].voltage,6);
        endObject();
    }    
    endArray();
    endJson();
}



void ADCSensor::outputCsv(AsyncResponseStream *outputStream) {
    /*
    8
    #voltages,id,lastmodified,voltages...
    t,83652827

    voltages,83663614,0.23184,0.31135,0.075
    */
    read();
    startBlock(outputStream);
    startRecord("voltages");
    appendField(lastRead);
    for (int i = 0; i < MAX_ADC_CHANNELS; i++){
        appendField(channel[i].voltage);
    }    
    endRecord();
    endBlock();

}


double ADCSensor::getServiceVoltage() {
    return channel[0].voltage;
}
double ADCSensor::getEngineVoltage() {
    return channel[1].voltage;
}
double ADCSensor::getServiceCurrent() {
    return channel[2].voltage*1000.0;
}


bool ADCSensor::drawPage(Adafruit_SSD1306 * display) {
    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
    switch(subPage) {
        case 0:
            display->setTextSize(2);   // 18x24  128/24=7 chars 5,3 
            display->printf("%5.2f V Sv\n", channel[0].voltage); // ADC0 is service battery
            display->printf("%5.1f A\n", channel[2].voltage*1000.0); // ADC2-3 is service battery shunt .1A==100A
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->setTextSize(2);
            display->printf("%5.2f V En\n",channel[1].voltage); // ADC1 is engine battery
            display->display();
            subPage = 2;
            return false;
        case 2:
            if ( drawHistory(display,true,10.0,15.0) ) {
                display->display();
                subPage = 0;
                return true;
            } else {
                display->display();
                return false;
            }
        default:
            subPage = 0;
            return false;
    }
#else
    switch(subPage) {
        case 0:
            display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
            display->setCursor(0,4);  
            display->printf("%5.2f V Ev\n",channel[1].voltage); // ADC1 is engine battery
            display->setCursor(0,24); // 4+16+4
            display->printf("%5.2f V Sv\n", channel[0].voltage); // ADC0 is service battery 
            display->setCursor(0,44); // 24+16+4
            display->printf("%5.1f A Sa\n", channel[2].voltage*1000.0); // ADC2-3 is service battery shunt .1A==100A
            display->display();
            subPage = 1;
            return false;
        case 1:
            if ( drawHistory(display,true,10.0,15.0) ) {
                display->display();
                subPage = 0;
                return true;
            } else {
                display->display();
                return false;
            }
        default:
            subPage = 0;
            return false;
    }
#endif
    
}
