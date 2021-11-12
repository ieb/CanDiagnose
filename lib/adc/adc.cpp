

#include "adc.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"


#define ADC_CAL_LEN 27
// These are the raw ADC readings, measured from  potentiometer wired to a scope.
uint16_t calTable[ADC_CAL_LEN] PROGMEM={
    0,
    6,
    94,
    127,
    175,
    242,
    313,
    414,
    566,
    750,
    845,
    1134,
    1360,
    1518,
    1703,
    1848,
    2056,
    2193,
    2323,
    2594,
    2859,
    3055,
    3275,
    3530,
    3803,
    4028,
    4096
};

// these are the corresponding measured voltages in mv read from the scope averaged over several seconds.
uint16_t valTable[ADC_CAL_LEN] PROGMEM={
    0,
    119,
    191,
    217,
    256,
    313,
    369,
    451,
    572,
    723,
    803,
    1029,
    1211,
    1337,
    1484,
    1601,
    1765,
    1880,
    1982,
    2200,
    2410,
    2560,
    2710,
    2860,
    2990,
    3080,
    3150
};



void ADCSensor::begin() {
    analogSetWidth(12);
    analogSetClockDiv(1);
    analogSetAttenuation(ADC_0db);

      //Check TP is burned into eFuse
  if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
    Serial.print("eFuse Two Point: Supported\n");
  } else {
    Serial.print("eFuse Two Point: NOT supported\n");
  }

  //Check Vref is burned into eFuse
  if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
    Serial.print("eFuse Vref: Supported\n");
  } else {
    Serial.print("eFuse Vref: NOT supported\n");
  }

}

void ADCSensor::measure() {
    return;
    unsigned long now = millis();
    if ( now > lastRead + readPeriod )  {
        lastRead = now;
        read();
    }
}

void ADCSensor::printVoltages() {
    for (int i = 0; i < MAX_ADC_CHANNELS; i++) {
        Serial.print("v");Serial.print(i);Serial.print(":");Serial.print(voltages[i]/MA_SAMPLES);Serial.println("mV");
    }
}

void ADCSensor::read() {
    for (int i = 0; i < MAX_ADC_CHANNELS; i++) {
        // moving average over 20 samples.
        uint32_t raw = readRaw(adcPins[i]);
        adcRawReading[i] = adcRawReading[i]-(adcRawReading[i]/MA_SAMPLES) + raw;
        uint16_t v = convertMv(raw);
        voltages[i] = voltages[i]-(voltages[i]/MA_SAMPLES)+v;
        Serial.print("v");Serial.print(i);Serial.print(": instant:(");
        Serial.print(raw);Serial.print(") ");
        Serial.print(v);Serial.print(" mV averaged:(");
        Serial.print(adcRawReading[i]/MA_SAMPLES);Serial.print(") ");
        Serial.print(voltages[i]/MA_SAMPLES);Serial.println(" mV");
    }
}



uint16_t ADCSensor::convertMv(uint16_t raw) {
    uint16_t cv0 = ((int16_t)pgm_read_dword(&calTable[0]));
    for (int i = 1; i < ADC_CAL_LEN; i++) {
        uint16_t cv1 = ((uint16_t)pgm_read_dword(&calTable[i]));
        if ( raw < cv1) {
            uint16_t adc0 = ((uint16_t)pgm_read_dword(&valTable[i-1]));
            uint16_t adc1 = ((uint16_t)pgm_read_dword(&valTable[i]));
            return (uint16_t)round(adc0+(1.0*(adc1 - adc0)*(raw-cv0))/(1.0*(cv1-cv0)));
        }
        cv0 = cv1;
    }
    return ((int16_t)pgm_read_dword(ADC_CAL_LEN-1));
}

uint16_t ADCSensor::readRaw(int gpio) {
    uint32_t sum = 0;
    for (int j = 0; j < ADC_SAMPLES; j++) {
//        sum += adc1_get_raw((adc1_channel_t)get_adc1_chanel(gpio));
        sum += analogRead(gpio);
    }
    return (sum/ADC_SAMPLES);
}

void ADCSensor::checkCalibration() {

    analogSetWidth(12);
    analogSetClockDiv(1);
    analogSetAttenuation(ADC_0db);
    Serial.print("Div 1, db 0");

    for ( int i = 0; i < 100; i++) {
        Serial.println(adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_5));
    }

    analogSetClockDiv(4);
    analogSetAttenuation(ADC_0db);
    Serial.print("Div 4, db 0");

    for ( int i = 0; i < 100; i++) {
        Serial.println(adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_5));
    }

    analogSetClockDiv(16);
    analogSetAttenuation(ADC_0db);
    Serial.print("Div 16, db 0");

    for ( int i = 0; i < 100; i++) {
        Serial.println(adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_5));
    }


    analogSetClockDiv(64);
    analogSetAttenuation(ADC_0db);
    Serial.print("Div 64, db 0");

    for ( int i = 0; i < 100; i++) {
        Serial.println(adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_5));
    }


    analogSetClockDiv(1);
    analogSetAttenuation(ADC_11db);
    Serial.print("Div 1, db 11");

    for ( int i = 0; i < 100; i++) {
        Serial.println(adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_5));
    }

    analogSetClockDiv(4);
    analogSetAttenuation(ADC_11db);
    Serial.print("Div 4, db 11");

    for ( int i = 0; i < 100; i++) {
        Serial.println(adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_5));
    }

    analogSetClockDiv(16);
    analogSetAttenuation(ADC_11db);
    Serial.print("Div 16, db 11");

    for ( int i = 0; i < 100; i++) {
        Serial.println(adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_5));
    }


    analogSetClockDiv(64);
    analogSetAttenuation(ADC_11db);
    Serial.print("Div 64, db 11");

    for ( int i = 0; i < 100; i++) {
        Serial.println(adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_5));
    }


//    uint32_t raw = readRaw(GPIO_NUM_33);
//    uint16_t v = convertMv(raw);
//    Serial.print("raw:");Serial.print(raw);Serial.print(" v:");Serial.println(v);
}



void ADCSensor::outputJson(AsyncResponseStream *outputStream) {
    read();
    startJson(outputStream);
    append("t",millis());
    startArray("voltages");
    for (int i = 0; i < MAX_ADC_CHANNELS; i++){
        appendCommaIfRequired();
        outputStream->print(voltages[i]/MA_SAMPLES);
    }    
    endArray();
    endJson();
}
