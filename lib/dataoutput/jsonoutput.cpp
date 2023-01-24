
#include "dataoutput.h"



void EngineDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("engine");
    for (int i=0; i<MAX_ENGINE_SOURCES; i++) {
        EngineData *engine =  &n2kCollector.engine[i];
        startObject();
        append("id",i);
        append("source",engine->source);
        append("instance",engine->instance);
        append("lastModified",engine->lastModified);
        append("hours",engine->Hours);
        append("speed",engine->speed);
        append("coolantTemp",engine->CoolantTemp);
        append("altenatorVoltage",engine->AltenatorVoltage);
        append("status1",engine->Status1);
        append("status2",engine->Status2);
        endObject();
    }
    endArray();
    startArray("battery");
    for (int i=0; i<MAX_BATTERY_SOURCES; i++) {
        DcBatteryData *battery =  &n2kCollector.dcBattery[i];
        startObject();
        append("id",i);
        append("source",battery->source);
        append("instance",battery->instance);
        append("lastModified",battery->lastModified);
        append("instance",battery->instance);
        append("voltage",battery->voltage);
        append("current",battery->current);
        append("temperature",battery->temperature);
        endObject();   
    }
    endArray();
    startArray("fluidLevel");
    for (int i=0; i<MAX_FLUID_LEVEL_SOURCES; i++) {
        FluidLevelData *fluidLevel =  &n2kCollector.fluidLevel[i];
        startObject();
        append("id",i);
        append("source",fluidLevel->source);
        append("instance",fluidLevel->instance);
        append("lastModified",fluidLevel->lastModified);
        append("level",fluidLevel->level);
        append("capacity",fluidLevel->capacity);
        append("fluidType",fluidLevel->fluidType);
        endObject();
    }
    endArray();
    endJson();
}



void BoatDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("heading");
    for (int i=0; i<MAX_HEADDING_SOURCES; i++) {
        HeadingData *heading =  &n2kCollector.heading[i];
        startObject();
        append("id",i);
        append("source",heading->source);
        append("lastModified",heading->lastModified);
        append("heading",heading->heading);
        append("deviation",heading->deviation);
        append("variation",heading->variation);
        append("reference",heading->reference);
        endObject();
    }
    endArray();
    startArray("speed");
    for (int i=0; i<MAX_SPEED_SOURCES; i++) {
        SpeedData *speed =  &n2kCollector.speed[i];
        startObject();
        append("id",i);
        append("source",speed->source);
        append("lastModified",speed->lastModified);
        append("stw",speed->stw);
        append("sog",speed->sog);
        append("swrt",speed->swrt);
        endObject();
    }
    endArray();
    startArray("waterDepth");
    for (int i=0; i<MAX_WATER_DEPTH_SOURCES; i++) {
        WaterDepthData *waterDepth =  &n2kCollector.waterDepth[i];
        startObject();
        append("id",i);
        append("source",waterDepth->source);
        append("lastModified",waterDepth->lastModified);
        append("depthBelowTransducer",waterDepth->depthBelowTransducer);
        append("offset",waterDepth->offset);
        endObject();
    }
    endArray();
    startArray("rudder");
    for (int i=0; i<MAX_RUDDER_SOURCES; i++) {
        RudderData *rudder =  &n2kCollector.rudder[i];
        startObject();
        append("id",i);
        append("source",rudder->source);
        append("instance",rudder->instance);
        append("lastModified",rudder->lastModified);
        append("directionOrder",rudder->directionOrder);
        append("position",rudder->position);
        append("angleOrder",rudder->angleOrder);
        endObject();
    }
    endArray();
    startArray("attitude");
    for (int i=0; i<MAX_ATTITUDE_SOURCES; i++) {
        AttitudeData *attitude =  &n2kCollector.attitude[i];
        startObject();
        append("id",i);
        append("source",attitude->source);
        append("lastModified",attitude->lastModified);
        append("roll",attitude->roll);
        append("pitch",attitude->pitch);
        append("yaw",attitude->yaw);
        endObject();
    }
    endArray();
    endJson();
}





void NavigationDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("cogSog");
    for (int i=0; i<MAX_COGSOG_SOURCES; i++) {
        CogSogData *cogSog =  &n2kCollector.cogSog[i];
        startObject();
        append("id",i);
        append("source",cogSog->source);
        append("lastModified",cogSog->lastModified);
        append("cog",cogSog->cog);
        append("sog",cogSog->sog);
        append("reference",cogSog->reference);
        endObject();
    }
    endArray();
    startArray("gnss");
    for (int i=0; i<MAX_GNSS_SOURCES; i++) {
        GnssData *gnss =  &n2kCollector.gnss[i];
        startObject();
        append("id",i);
        append("source",gnss->source);
        append("lastModified",gnss->lastModified);
        append("daysSince1970",gnss->daysSince1970);
        append("secondsSinceMidnight",gnss->secondsSinceMidnight);
        append("latitude",gnss->latitude,8);
        append("longitude",gnss->longitude,8);
        append("altitude",gnss->altitude);
        append("type",gnss->type);
        append("method",gnss->method);
        append("nSatellites",gnss->nSatellites);
        append("HDOP",gnss->HDOP);
        append("PDOP",gnss->PDOP);
        append("geoidalSeparation",gnss->geoidalSeparation);
        append("nReferenceStations",gnss->nReferenceStations);
        append("referenceStationType",gnss->referenceStationType);
        append("ageOfCorrection",gnss->ageOfCorrection);
        endObject();
    }
    endArray();
    endJson();
}



void EnvironmentDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("outsideEnvironment");
    for (int i=0; i<MAX_OUTSIDE_ENVIRONMENTAL_SOURCES; i++) {
        OutsideEnvironmentData *outsideEnvironmental =  &n2kCollector.outsideEnvironmental[i];
        startObject();
        append("id",i);
        append("source",outsideEnvironmental->source);
        append("lastModified",outsideEnvironmental->lastModified);
        append("waterTemperature",outsideEnvironmental->waterTemperature);
        append("outsideAmbientAirTemperature",outsideEnvironmental->outsideAmbientAirTemperature);
        append("atmosphericPressure",outsideEnvironmental->atmosphericPressure);
        endObject();
    }
    endArray();
    startArray("environment");
    for (int i=0; i<MAX_ENVIRONMENTAL_PARAMS_SOURCES; i++) {
        EnvironmentalParamsData *environmentParams =  &n2kCollector.environmentalParams[i];
        startObject();
        append("id",i);
        append("source",environmentParams->source);
        append("lastModified",environmentParams->lastModified);
        append("temperature",environmentParams->temperature);
        append("humidity",environmentParams->humidity);
        append("atmosphericPressure",environmentParams->atmosphericPressure);
        append("temperatureSource",environmentParams->temperatureSource);
        append("humiditySource",environmentParams->humiditySource);
        endObject();
    }
    endArray();
    startArray("humidity");
    for (int i=0; i<MAX_HUMIDITY_SOURCES; i++) {
        HumidityData *humidity =  &n2kCollector.humidity[i];
        startObject();
        append("id",i);
        append("source",humidity->source);
        append("instance",humidity->instance);
        append("lastModified",humidity->lastModified);
        append("instance",humidity->instance);
        append("source",humidity->source);
        append("actual",humidity->actual);
        append("set",humidity->set);
        endObject();
    }
    endArray();
    startArray("pressure");
    for (int i=0; i<MAX_PRESSURE_SOURCES; i++) {
        PressureData *pressure =  &n2kCollector.pressure[i];
        startObject();
        append("id",i);
        append("source",pressure->source);
        append("instance",pressure->instance);
        append("lastModified",pressure->lastModified);
        append("instance",pressure->instance);
        append("source",pressure->source);
        append("actual",pressure->actual);
        endObject();
    }
    endArray();
    endJson();
}



void TemperatureDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("temperature");
    for (int i=0; i<MAX_TEMPERATURE_SOURCES; i++) {
        TemperatureData *temperature =  &n2kCollector.temperature[i];
        startObject();
        append("id",i);
        append("source",temperature->source);
        append("instance",temperature->instance);
        append("lastModified",temperature->lastModified);
        append("instance",temperature->instance);
        append("source",temperature->source);
        append("actual",temperature->actual);
        append("set",temperature->set);
        endObject();
    }
    endArray();
    startArray("temperatureExt");
    for (int i=0; i<MAX_TEMPERATURE_SOURCES; i++) {
        TemperatureData *temperatureExt =  &n2kCollector.temperatureExt[i];
        startObject();
        append("id",i);
        append("source",temperatureExt->source);
        append("instance",temperatureExt->instance);
        append("lastModified",temperatureExt->lastModified);
        append("instance",temperatureExt->instance);
        append("source",temperatureExt->source);
        append("actual",temperatureExt->actual);
        append("set",temperatureExt->set);
        endObject();
    }
    endArray();
    endJson();
}

void XteDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("xte");
    for ( int i = 0; i < MAX_XTE_SOURCES; i++) {
        XteData *xte =  &n2kCollector.xte[i];
        startObject();
        append("id",i);
        append("source",xte->source);
        append("instance",xte->instance);
        append("lastModified",xte->lastModified);
        append("xte",xte->xte);
        endObject();
    }
    endArray();
    endJson();
}


void MagneticVariationDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("variation");
    for ( int i = 0; i < MAX_VARIATION_SOURCES; i++) {
        VariationData *variation = &n2kCollector.variation[i];
        startObject();
        append("id",i);
        append("source",variation->source);
        append("lastModified",variation->lastModified);
        append("daysSince1970",variation->daysSince1970);
        append("variation",variation->variation);
        endObject();
    }
    endArray();
    endJson();
}

void WindSpeedDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("wind");
    for ( int i = 0; i < MAX_WIND_SOURCES; i++) {
        WindData *wind = &n2kCollector.wind[i];
        startObject();
        append("id",i);
        append("source",wind->source);
        append("instance",wind->instance);
        append("lastModified",wind->lastModified);
        append("windSpeed",wind->windSpeed);
        append("windAngle",wind->windAngle);
        append("windReference",wind->windReference);
        endObject();
    }
    endArray();
    endJson();
}

void LogDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("log");
    for ( int i = 0; i < MAX_LOG_SOURCES; i++) {
        LogData *log = &n2kCollector.log[i];
        startObject();
        append("id",i);
        append("source",log->source);
        append("lastModified",log->lastModified);
        append("daysSince1970",log->daysSince1970);
        append("secondsSinceMidnight",log->secondsSinceMidnight);
        append("log",log->log);
        append("tripLog",log->tripLog);
        endObject();
    }
    endArray();
    endJson();
}

void LatLonDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("possition");
    for ( int i = 0; i < MAX_LOG_SOURCES; i++) {
        PossitionData *possition = &n2kCollector.possition[i];
        startObject();
        append("id",i);
        append("source",possition->source);
        append("lastModified",possition->lastModified);
        append("latitude",possition->latitude);
        append("longitude",possition->longitude);
        endObject();
    }
    endArray();
    endJson();
}


void LeewayDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("leeway");
    for ( int i = 0; i < MAX_LOG_SOURCES; i++) {
        LeewayData *leeway = &n2kCollector.leeway[i];
        startObject();
        append("id",i);
        append("source",leeway->source);
        append("lastModified",leeway->lastModified);
        append("instance",leeway->instance);
        append("leeway",leeway->leeway);
        endObject();
    }
    endArray();
    endJson();
}
