#include "temperature.h"


void Temperature::begin() {
    Serial.print("Starting Temperatures with pin "); Serial.println(oneWirePin);
    oneWire.begin(oneWirePin);
    sensors.setOneWire(&oneWire);
    sensors.begin();
    sensors.setWaitForConversion(true);
    for(int i=0; i< MAX_TEMPERATURE_SENSORS; i++) {
        if(sensors.getAddress(temperatureSensor[i].address, i)) {
           temperatureSensor[i].id = i;
           sensors.setResolution(temperatureSensor[i].address, TEMPERATURE_PRECISION);
           printSensor(i);
        } else {
            Serial.print("Unable to find address for Device ");
            Serial.println(i);
            temperatureSensor[i].id = -1;
        }
    }   
}


void Temperature::read() {
    unsigned long now = millis();
    if ( now > lastRead + readPeriod )  {
        lastRead = now;  
        for(int i=0; i< MAX_TEMPERATURE_SENSORS; i++) {
            if(isAvailable(i)) {
                unsigned long now = millis();
                float t = sensors.getTempC(temperatureSensor[i].address);
                if  (t == DEVICE_DISCONNECTED_C ) {
                    temperatureSensor[i].connected = 0;
                } else {
                    temperatureSensor[i].connected = 1;
                    temperatureSensor[i].temperature = t;
                    temperatureSensor[i].lastModified = millis();
                }
                temperatureSensor[i].readTime = millis()-now;
            }
        }
    }
}



void Temperature::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    startArray("temperatures");
    for (int i=0; i<MAX_TEMPERATURE_SENSORS; i++) {
        if (isAvailable(i)) {
            outputSensorJson(outputStream,i);
        }
    }
    endArray();
    endJson();
}

void Temperature::printSensor(int si) {
    Serial.print("Device: ");
    Serial.print(temperatureSensor[si].id);
    Serial.print(" Address: ");
    for (uint8_t i = 0; i < 8; i++) {
        // zero pad the address if necessary
        if (temperatureSensor[si].address[i] < 16) Serial.print("0");
        Serial.print(temperatureSensor[si].address[i], HEX);
    }    
    Serial.print(" resolution:");
    Serial.print(sensors.getResolution(temperatureSensor[si].address));
    Serial.println(" bits");
}

bool Temperature::isAvailable(int si) {
    return (temperatureSensor[si].id != -1);
}


void Temperature::outputSensorJson(AsyncResponseStream *outputStream, int si) {
//    char buffer[17];
    startObject();
    append("id",temperatureSensor[si].id);
    append("lastModified",temperatureSensor[si].lastModified);
    append("readTime",temperatureSensor[si].readTime);
    append("connected",temperatureSensor[si].connected);
    outputStream->print(",\"address\":\"");
    for (uint8_t i = 0; i < 8; i++) {
        // zero pad the address if necessary
        if (temperatureSensor[si].address[i] < 16) outputStream->print("0");
        outputStream->print(temperatureSensor[si].address[i], HEX);
    }
    outputStream->print("\"");
//    sprintf(buffer,"%02x%02x%02x%02x%02x%02x%02x%02x",sensorAddress[0],sensorAddress[1],sensorAddress[2],sensorAddress[3],sensorAddress[4],sensorAddress[5],sensorAddress[6],sensorAddress[7]);
//    append("address",buffer);
    append("temperature",temperatureSensor[si].temperature);
    endObject();
}
