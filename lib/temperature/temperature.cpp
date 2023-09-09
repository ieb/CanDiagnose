#include "temperature.h"
#include "config.h"

/**
 * @brief NMEA2000 output for the sensors on this device, with mappings from the config file.
 * 
 * nmea2000.temp.n.instance = temperatureinstance, 255 to disable.
 * nmea2000.temp.n.source = tN2kTempSource see below
 * nmea2000.battery.service.instance = instance to use for service battery DC Status
 * nmea2000.battery.engine.instance = instance to use for engine battery DC Status
 * nmea2000.battery.service.tempsensor = temperature sensor index for service battery
 * nmea2000.battery.engine.tempsensor = temperature sensor index for engine battery
 * nmea2000.pressure.period = period to emit pressure and humidiy messages
 * nmea2000.temperature.period = period to emit temperature messages
 * nmea2000.dcstatus.period = period to emit battery messages
 * 
 * 
 * enum tN2kTempSource {
                            N2kts_SeaTemperature=0,
                            N2kts_OutsideTemperature=1,
                            N2kts_InsideTemperature=2,
                            N2kts_EngineRoomTemperature=3,
                            N2kts_MainCabinTemperature=4,
                            N2kts_LiveWellTemperature=5,
                            N2kts_BaitWellTemperature=6,
                            N2kts_RefridgerationTemperature=7,
                            N2kts_HeatingSystemTemperature=8,
                            N2kts_DewPointTemperature=9,
                            N2kts_ApparentWindChillTemperature=10,
                            N2kts_TheoreticalWindChillTemperature=11,
                            N2kts_HeatIndexTemperature=12,
                            N2kts_FreezerTemperature=13,
                            N2kts_ExhaustGasTemperature=14
                          };
 */


void Temperature::begin(const char * configurationFile) {
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

    String v = "";
    for (int i = 0; i < MAX_TEMPERATURE_SENSORS; i++) {
        String tempId = "nmea2000.temp.";
        tempId = tempId + i;
        if ( ConfigurationFile::get(configurationFile, tempId+".instnace", v)) {
            temperatureInstance[i] = v.toInt();
        } else {
            temperatureInstance[i] = i;
        }
        if ( ConfigurationFile::get(configurationFile, tempId+".source", v)) {
            temperatureSource[i] = v.toInt();
        } else {
            temperatureSource[i] = N2kts_MainCabinTemperature;
        }
    }
    if (ConfigurationFile::get(configurationFile, "nmea2000.temp.period", v)) {
        temperaturePeriod = v.toInt();
    } else {
        temperaturePeriod = 10000;
    }
   
}


void Temperature::read() {
    unsigned long now = millis();
    if ( (now-lastRead) >  readPeriod )  {
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

void Temperature::output() {
    unsigned long now = millis();

    tN2kMsg N2kMsg;
    if ( (now - lastTemperature) >  temperaturePeriod )  {
        lastTemperature = now; 
        for (int i = 0; i < MAX_TEMPERATURE_SENSORS; i++) {
            if ( temperatureInstance[i] != 255 && temperatureSensor[i].connected ) {
                SetN2kTemperature(N2kMsg, temperatureSid, temperatureInstance[i], (tN2kTempSource)(temperatureSource[i]), CToKelvin(temperatureSensor[i].temperature));
                NMEA2000->SendMsg(N2kMsg);
            }
        }
        temperatureSid++;
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


void Temperature::outputCsv(AsyncResponseStream *outputStream) {
    /*
    6
    #temperatures,id,lastmodified,temperatures...
t,83652827

temperatures,83663614,273.15,
*/
    startBlock(outputStream);
    startRecord("temperatures");
    appendField(lastRead);
    for (int i=0; i<MAX_TEMPERATURE_SENSORS; i++) {
        appendField(temperatureSensor[i].temperature);
    }
    endRecord();
    endBlock();
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
