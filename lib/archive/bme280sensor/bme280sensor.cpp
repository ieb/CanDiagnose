

#include "bme280sensor.h"


void BME280Sensor::begin() {

    if (!bme.begin(0x76)) {
        Serial.println(F("Could not find a valid BME280 sensor, normally at 0x76 with ID of 0x60  "));
        return;
    } 

    bme_temp->printSensorDetails();
    bme_pressure->printSensorDetails();
    bme_humidity->printSensorDetails();
        
}

void BME280Sensor::read() {
    unsigned long now = millis();
    if ( (now-lastRead) > readPeriod )  {
        lastRead = now;  
        bme_temp->getEvent(&temp_event);
        bme_pressure->getEvent(&pressure_event);
        bme_humidity->getEvent(&humidity_event);
        storeHistory(pressure_event.pressure);
    }
}


void BME280Sensor::outputCsv(AsyncResponseStream *outputStream) {
    /*
    7
    #bmp280,id,lastmodified,temp,pressure,humidity,historyInterval,history...
    t,83652827

    bmp280,83663614,27.48,1012.19,36.58,675000,998.31,998.42,998.4,998.21,998.2
    */
    read();
    startBlock(outputStream);
    startRecord("bmp280");
    appendField(lastRead);
    appendField(temp_event.temperature);
    appendField(pressure_event.pressure);
    appendField(humidity_event.relative_humidity);
    appendField(getPeriodMs());
    startIterator();
    while(hasNext()) {
        appendField(nextValue());
    }
    endRecord();
    endBlock();
}



void BME280Sensor::outputJson(AsyncResponseStream *outputStream) {
    read();
    startJson(outputStream);
    append("t",millis());
    startObject("bmp280");
    append("lastModified", lastRead);
    append("temp",temp_event.temperature);
    append("pressure",pressure_event.pressure);
    append("humidity",humidity_event.relative_humidity);
    append("historyInterval",getPeriodMs());
    startArray("history");
    startIterator();
    while(hasNext()) {
        append(nextValue());
    }
    endArray();
    endObject();
    endJson();
}



bool BME280Sensor::drawPage(Adafruit_SSD1306 * display) {
    float change3h, change6h;
    unsigned char ch3h, ch6h;
    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text

#if OLED_HEIGHT == 32
#else
    display->setCursor(0,12);              
    switch(subPage) {
        case 0:
            change3h = changeSince(10800000L);
            change6h = changeSince(21600000L);
            ch3h = (change3h>0)?24:(change3h<0)?25:'-';
            ch6h = (change6h>0)?24:(change6h<0)?25:'-';
            display->printf("%6.1fmbar\n",pressure_event.pressure);
            display->setCursor(0,36);              
            display->cp437(true);         // Use full 256 char 'Code Page 437' font
            display->printf("3%c%2.0f 6%c%2.0f\n",ch3h,change3h,ch6h,change6h);
            display->cp437(false);         // Use full 256 char 'Code Page 437' font
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->print(temp_event.temperature,1);display->println(" C");
            display->setCursor(0,36);              
            display->print(humidity_event.relative_humidity,1);display->println(" %RH");
            display->display();
            subPage = 2;
            return false;
        case 2:
            if (  drawHistory(display) ) {
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

