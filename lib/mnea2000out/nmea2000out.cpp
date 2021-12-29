#include "nmea2000out.h"
#include <NMEA2000.h>
#include <N2kMessages.h>
#include <N2kTypes.h>

/**
 * @brief NMEA2000 output for the sensors on this device, with mappings from the config file.
 * 
 * nmea2000.temp.n.instance = temperatureinstance, 255 to disable.
 * nmea2000.temp.n.source = tN2kTempSource see below
 * nmea2000.pressure.instance = instance to use for bme280 pressure sensor
 * nmea2000.humidity.instance = instance to use for bme280 humidity sensor
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

Nmea2000Output::Nmea2000Output( tNMEA2000 *NMEA2000, 
            ADCSensor &adc, 
            BME280Sensor &bme280Sensor, 
            Temperature &temperature): 
            NMEA2000{NMEA2000},
            adc{adc}, 
            bme280Sensor{bme280Sensor}, 
            temperature{temperature} {};

void Nmea2000Output::begin(const char * configurationFile) {
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
    if ( ConfigurationFile::get(configurationFile, "nmea2000.pressure.instance", v)) {
        pressureInstance = v.toInt();
    } else {
        pressureInstance = 0;
    }
    if ( ConfigurationFile::get(configurationFile, "nmea2000.humidity.instance", v)) {
        humidityInstance = v.toInt();
    } else {
        humidityInstance = 0;
    }
    if (ConfigurationFile::get(configurationFile, "nmea2000.battery.service.instance", v)) {
        serviceBatteryInstance = v.toInt();
    } else {
        serviceBatteryInstance = 0;
    }
    if (ConfigurationFile::get(configurationFile, "nmea2000.battery.service.tempsensor", v)) {
        serviceBatteryTemperatureSensor = v.toInt();
    } else {
        serviceBatteryTemperatureSensor = 255;
    }
    if (ConfigurationFile::get(configurationFile, "nmea2000.battery.engine.instance", v)) {
        engineBatteryInstance = v.toInt();
    } else {
        engineBatteryInstance = 1;
    }
    if (ConfigurationFile::get(configurationFile, "nmea2000.battery.engine.tempsensor", v)) {
        engineBatteryTemperatureSensor = v.toInt();
    } else {
        engineBatteryTemperatureSensor = 255;
    }
    if (ConfigurationFile::get(configurationFile, "nmea2000.temp.period", v)) {
        temperaturePeriod = v.toInt();
    } else {
        temperaturePeriod = 10000;
    }
    if (ConfigurationFile::get(configurationFile, "nmea2000.pressure.period", v)) {
        pressurePeriod = v.toInt();
    } else {
        pressurePeriod = 10000;
    }
    if (ConfigurationFile::get(configurationFile, "nmea2000.dcstatus.period", v)) {
        dcstatusPeriod = v.toInt();
    } else {
        dcstatusPeriod = 10000;
    }
}

void Nmea2000Output::output() {
    unsigned long now = millis();

    tN2kMsg N2kMsg;
    if ( now > lastTemperature + temperaturePeriod )  {
        lastTemperature = now; 
        for (int i = 0; i < MAX_TEMPERATURE_SENSORS; i++) {
            if ( temperatureInstance[i] != 255 && temperature.temperatureSensor[i].connected ) {
                SetN2kTemperature(N2kMsg, temperatureSid, temperatureInstance[i], (tN2kTempSource)(temperatureSource[i]), CToKelvin(temperature.temperatureSensor[i].temperature));
                NMEA2000->SendMsg(N2kMsg);
            }
        }
        temperatureSid++;
    } else if ( now > lastPressure + pressurePeriod ) {
        lastPressure = now;
        if ( pressureInstance != 255 ) {
            SetN2kPressure(N2kMsg, pressureSID, pressureInstance,N2kps_Atmospheric, bme280Sensor.getPressure());
            NMEA2000->SendMsg(N2kMsg);
        }
        if ( humidityInstance != 255 ) {
            SetN2kHumidity(N2kMsg, pressureSID, humidityInstance, N2khs_InsideHumidity, bme280Sensor.getHumidity());
            NMEA2000->SendMsg(N2kMsg);
        }
        pressureSID++;
    } else if ( now > lastDCStatus + dcstatusPeriod) {
        lastDCStatus = now;
        double batteryTemperature = N2kDoubleNA;
        if ( serviceBatteryTemperatureSensor != 255 && temperature.temperatureSensor[serviceBatteryTemperatureSensor].connected ) {
            batteryTemperature = CToKelvin(temperature.temperatureSensor[serviceBatteryTemperatureSensor].temperature);
        }
        SetN2kDCBatStatus(N2kMsg, serviceBatteryInstance, adc.getServiceVoltage(), adc.getServiceCurrent(),batteryTemperature);
        NMEA2000->SendMsg(N2kMsg);
        batteryTemperature = N2kDoubleNA;
        if ( engineBatteryTemperatureSensor != 255 && temperature.temperatureSensor[engineBatteryTemperatureSensor].connected ) {
            batteryTemperature = CToKelvin(temperature.temperatureSensor[engineBatteryTemperatureSensor].temperature);
        }
        SetN2kDCBatStatus(N2kMsg, engineBatteryInstance, adc.getEngineVoltage(), N2kDoubleNA,batteryTemperature);
        NMEA2000->SendMsg(N2kMsg);
    }
    
}
