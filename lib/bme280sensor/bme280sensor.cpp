

#include "bme280sensor.h"


void BME280Sensor::begin() {
    if (!Wire.begin(sdaPin, sclPin) ) {
        Serial.println("I2C failed to start");
        return;
    }
    Serial.println("I2C scanning ");
    for (uint8_t addr = 0; addr < 255; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.print(" ");
            Serial.print(addr,HEX);
        } else {
            Serial.print(" --");
        }
    }
    Serial.println("Done ");

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
    if ( now > lastRead + readPeriod )  {
        lastRead = now;  
        bme_temp->getEvent(&temp_event);
        bme_pressure->getEvent(&pressure_event);
        bme_humidity->getEvent(&humidity_event);
    }
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
    endObject();
    endJson();
}

bool BME280Sensor::drawPage(Adafruit_SSD1306 * display) {
    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text

#if OLED_HEIGHT == 32
    display->setCursor(0,0);              // Start at top-left corner
    switch(subPage) {
        case 0:

            display->printf("% 6.1fmbar\n",pressure_event.pressure);
            display->cp437(true);         // Use full 256 char 'Code Page 437' font
            display->printf("6%c%2.0f 12%c%2.0f\n",24,5.0,25,14.2);
            display->cp437(false);         // Use full 256 char 'Code Page 437' font
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->print(temp_event.temperature,1);display->println(" C");
            display->print(humidity_event.relative_humidity,1);display->println(" %RH");
            display->display();
            subPage = 0;
            return true;
        default:
            subPage = 0;
            return false;
    }
#else
    display->setCursor(0,12);              
    switch(subPage) {
        case 0:

            display->printf("% 6.1fmbar\n",pressure_event.pressure);
            display->setCursor(0,36);              
            display->cp437(true);         // Use full 256 char 'Code Page 437' font
            display->printf("6%c%2.0f 12%c%2.0f\n",24,5.0,25,14.2);
            display->cp437(false);         // Use full 256 char 'Code Page 437' font
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->print(temp_event.temperature,1);display->println(" C");
            display->setCursor(0,36);              
            display->print(humidity_event.relative_humidity,1);display->println(" %RH");
            display->display();
            subPage = 0;
            return true;
        default:
            subPage = 0;
            return false;
    }
#endif
}

