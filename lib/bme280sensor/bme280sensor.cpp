

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
    bme_temp->getEvent(&temp_event);
    bme_pressure->getEvent(&pressure_event);
    bme_humidity->getEvent(&humidity_event);
    lastModified = millis();  
}

void BME280Sensor::outputJson(AsyncResponseStream *outputStream) {
    read();
    startJson(outputStream);
    append("t",millis());
    startObject("bmp280");
    append("lastModified", lastModified);
    append("temp",temp_event.temperature);
    append("pressure",pressure_event.pressure);
    append("humidity",humidity_event.relative_humidity);
    endObject();
    endJson();
}


