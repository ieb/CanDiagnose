
#include "dataoutput.h"



void EngineDataOutput::outputCsv(AsyncResponseStream *outputStream) {
    startBlock(outputStream);
    for (int i=0; i<MAX_ENGINE_SOURCES; i++) {
        //# engine,id,t,lastModified,speed,coolantTemp,altenatorVoltage,status1,status2,hours
        EngineData *engine =  &dataCollector.engine[i];
        if ( engine->active() ) {
            startRecord("engine");
            appendField(i);
            appendField(engine->lastModified);
            appendField(engine->speed);
            appendField(engine->CoolantTemp);
            appendField(engine->AltenatorVoltage);
            appendField(engine->Status1);
            appendField(engine->Status2);
            appendField(engine->Hours);    
            endRecord();
        }
    }
    for (int i=0; i<MAX_BATTERY_SOURCES; i++) {
        //# battery,id,t,lastModified,voltage,current,temperature
        DcBatteryData *battery =  &dataCollector.dcBattery[i];
        if ( battery->active() ) {
            startRecord("battery");
            appendField(battery->instance);
            appendField(battery->lastModified);
            appendField(battery->voltage);
            appendField(battery->current);
            appendField(battery->temperature);
            endRecord();
        }
    }
    for (int i=0; i<MAX_FLUID_LEVEL_SOURCES; i++) {
        // # fluid,id,t,lastModified,level,capacity,type
        FluidLevelData *fluidLevel =  &dataCollector.fluidLevel[i];
        if ( fluidLevel->active() ) {
            startRecord("fluid");
            appendField(fluidLevel->instance);
            appendField(fluidLevel->lastModified);
            appendField(fluidLevel->level);
            appendField(fluidLevel->capacity);
            appendField(fluidLevel->fluidType);
            endRecord();
        }
    }
    endBlock();
}

void BoatDataOutput::outputCsv(AsyncResponseStream *outputStream) {
    /*
    2

    #type,id,t,lastmodified,...
#heading,id,t,lastmodified,heading,deviation,variation,type (0=true,1=magnetic)
#speed,id,t,lastmodified,stw,sog,referencetype (0=paddlewheel)
#waterDepth,id,t,lastmodified,depthBelowTransducer,offset
#rudder,id,t,lastmodified,directionOrder,position,angleOder
#attitude,id,t,lastmodified,pitch,roll,yaw
#wind,id,t,lastmodified,angle,speed,type  (0=true,  1=apparent)
t,83652827
heading,0,83663614,3.77,-0.15,0.2,1
speed,0,83663614,6.58,,0
waterDepth,0,83662795,5.5,0.2
rudder,0,83663613,1,1.09,-0.09
attitude,0,83662733,-0.25,0.14,-0.05
wind,0,83662733,-0.14,0.04,1
*/
   startBlock(outputStream);
     for (int i=0; i<MAX_SPEED_SOURCES; i++) {
        HeadingData *heading =  &dataCollector.heading[i];
        if ( heading->active() ) {
            startRecord("heading");
            appendField(i);
            appendField(heading->lastModified);
            appendField(heading->heading);
            appendField(heading->deviation);
            appendField(heading->variation);
            appendField(heading->reference);
            endRecord();
        }
    }
    for (int i=0; i<MAX_SPEED_SOURCES; i++) {
        SpeedData *speed =  &dataCollector.speed[i];
        if ( speed->active() ) {
            startRecord("speed");
            appendField(i);
            appendField(speed->lastModified);
            appendField(speed->stw);
            appendField(speed->sog);
            appendField(speed->swrt);
            endRecord();
        }
    }
    for (int i=0; i<MAX_WATER_DEPTH_SOURCES; i++) {
        WaterDepthData *waterDepth =  &dataCollector.waterDepth[i];
        if ( waterDepth->active() ) {
            startRecord("waterDepth");
            appendField(i);
            appendField(waterDepth->lastModified);
            appendField(waterDepth->depthBelowTransducer);
            appendField(waterDepth->offset);
            endRecord();            
        }
    }
    for (int i=0; i<MAX_RUDDER_SOURCES; i++) {
        RudderData *rudder =  &dataCollector.rudder[i];
        if ( rudder->active() ) {
            startRecord("rudder");
            appendField(rudder->instance);
            appendField(rudder->lastModified);
            appendField(rudder->directionOrder);
            appendField(rudder->position);
            appendField(rudder->angleOrder);
            endRecord();
        }
    }
    for (int i=0; i<MAX_ATTITUDE_SOURCES; i++) {
        AttitudeData *attitude =  &dataCollector.attitude[i];
        if ( attitude->active() ) {
            startRecord("attitude");
            appendField(i);
            appendField(attitude->lastModified);
            appendField(attitude->roll);
            appendField(attitude->pitch);
            appendField(attitude->yaw);
            endRecord();
        }
    }

    for (int i=0; i<MAX_WIND_SOURCES; i++) {
        WindData *wind =  &dataCollector.wind[i];
        if ( wind->active() ) {
            startRecord("wind");
            appendField(i);
            appendField(wind->lastModified);
            appendField(wind->windAngle);
            appendField(wind->windSpeed);
            appendField(wind->windReference);
            endRecord();
        }
    }
    endBlock();

}
void NavigationDataOutput::outputCsv(AsyncResponseStream *outputStream) {
/*
3
#type,id,t,lastmodified,...
#cogSog,id,t,lastmodified,cog,sog,reference
#gnss,id,t,lastmodified,daysSince1970,secondsSinceMidnight,latitude,longitude,altitude,type,method,nSatellites,HDOP,PDOP,geoidalSeparation,nReferenceStations,referenceStationType,ageOfCorrection
t,83652827

cogSog,0,83663614,2.12,0.2,0
gnss,0,83675791,18973,77160,60.44,22.24,10.5,0,1,12,0.8,0.5,15,1,0,2
*/
   startBlock(outputStream);
    for (int i=0; i<MAX_COGSOG_SOURCES; i++) {
        CogSogData *cogSog =  &dataCollector.cogSog[i];
        if ( cogSog->active() ) {
            startRecord("cogSog");
            appendField(i);
            appendField(cogSog->lastModified);
            appendField(cogSog->cog);
            appendField(cogSog->sog);
            appendField(cogSog->reference);
            endRecord();
        }
    }
    for (int i=0; i<MAX_GNSS_SOURCES; i++) {
        GnssData *gnss =  &dataCollector.gnss[i];
        if ( gnss->active() ) {
            startRecord("gnss");
            appendField(i);
            appendField(gnss->lastModified);
            appendField(gnss->daysSince1970);
            appendField(gnss->secondsSinceMidnight);
            appendField(gnss->latitude,8);
            appendField(gnss->longitude,8);
            appendField(gnss->altitude);
            appendField(gnss->type);
            appendField(gnss->method);
            appendField(gnss->nSatellites);
            appendField(gnss->HDOP);
            appendField(gnss->PDOP);
            appendField(gnss->geoidalSeparation);
            appendField(gnss->nReferenceStations);
            appendField(gnss->referenceStationType);
            appendField(gnss->ageOfCorrection);
            endRecord();
        }
    }
    endBlock();
}

void EnvironmentDataOutput::outputCsv(AsyncResponseStream *outputStream) {
    /*
    4
    #outsideEnvironment,id,lastmodified,waterTemperature,outsideAmbientAirTemperature,atmosphericPressure
    #humidity,id,lastmodified,actual,set
    #pressure,id,lastmodified,actual
    t,83652827

    outsideEnvironment,0,83663614,289.65,299.45,101600
    humidity,0,83675791,41.8,
    pressure,0,83675791,102400
    */
    startBlock(outputStream);
    for (int i=0; i<MAX_OUTSIDE_ENVIRONMENTAL_SOURCES; i++) {
        OutsideEnvironmentData *outsideEnvironmental =  &dataCollector.outsideEnvironmental[i];
        if ( outsideEnvironmental->active() ) {
            startRecord("outsideEnvironment");
            appendField(i);
            appendField(outsideEnvironmental->lastModified);
            appendField(outsideEnvironmental->waterTemperature);
            appendField(outsideEnvironmental->outsideAmbientAirTemperature);
            appendField(outsideEnvironmental->atmosphericPressure);
            endRecord();
        }
    }
    for (int i=0; i<MAX_ENVIRONMENTAL_PARAMS_SOURCES; i++) {
        EnvironmentalParamsData *environmentalParams =  &dataCollector.environmentalParams[i];
        if ( environmentalParams->active() ) {
            startRecord("enviromental");
            appendField(i);
            appendField(environmentalParams->lastModified);
            appendField(environmentalParams->temperature);
            appendField(environmentalParams->humidity);
            appendField(environmentalParams->atmosphericPressure);
            appendField(environmentalParams->temperatureSource);
            appendField(environmentalParams->humiditySource);
            endRecord();
        }
    }
    for (int i=0; i<MAX_HUMIDITY_SOURCES; i++) {
        HumidityData *humidity =  &dataCollector.humidity[i];
        if ( humidity->active() ) {
            startRecord("humidity");
            appendField(humidity->instance);
            appendField(humidity->lastModified);
            appendField(humidity->actual);
            appendField(humidity->set);
            endRecord();
        }
    }
    for (int i=0; i<MAX_PRESSURE_SOURCES; i++) {
        PressureData *pressure =  &dataCollector.pressure[i];
        if ( pressure->active() ) {
            startRecord("pressure");
            appendField(pressure->instance);
            appendField(pressure->lastModified);
            appendField(pressure->actual);
            endRecord();
        }
    }
    endBlock();

}
void TemperatureDataOutput::outputCsv(AsyncResponseStream *outputStream) {
    /*

    5
    #temperature,id,lastmodified,actual,set
#temperatureExt,id,lastmodified,actual,set
t,83652827

temperature,0,83663614,273.15,
temperatureExt,0,83663614,273.15,


*/
    startBlock(outputStream);
    for (int i=0; i<MAX_TEMPERATURE_SOURCES; i++) {
        TemperatureData *temperature =  &dataCollector.temperature[i];
        if ( temperature->active() ) {
            startRecord("temperature");
            appendField(temperature->instance);
            appendField(temperature->lastModified);
            appendField(temperature->actual);
            appendField(temperature->set);
            endRecord();
        }
    }
    for (int i=0; i<MAX_TEMPERATURE_SOURCES; i++) {
        TemperatureData *temperatureExt =  &dataCollector.temperatureExt[i];
        if ( temperatureExt->active() ) {
            startRecord("temperatureExt");
            appendField(temperatureExt->instance);
            appendField(temperatureExt->lastModified);
            appendField(temperatureExt->actual);
            appendField(temperatureExt->set);
            endRecord();
        }
    }
    endBlock();

}



void XteDataOutput::outputCsv(AsyncResponseStream *outputStream) {
    /*
    9
    #type,id,lastmodified,...
    #xte,id,lastmodified,xte
    t,83652827

    xte,0,83663630,180.20

    */
    startBlock(outputStream);
    for ( int i = 0; i < MAX_XTE_SOURCES; i++) {
        XteData *xte =  &dataCollector.xte[i];
        if ( xte->active() ) {
            startRecord("xte");
            appendField(xte->instance);
            appendField(xte->lastModified);
            appendField(xte->xte);
            endRecord();
        }
    
    }
    endBlock();
}

void MagneticVariationDataOutput::outputCsv(AsyncResponseStream *outputStream) {
    /*
    10
    #variation,id,lastmodified,daysSince1970,variation
    t,83652827

    variation,0,83663614,18973,-0.05
    */
    startBlock(outputStream);
    for ( int i = 0; i < MAX_VARIATION_SOURCES; i++) {
        VariationData *variation = &dataCollector.variation[i];
        if ( variation->active() ) {
            startRecord("variation");
            appendField(i);
            appendField(variation->lastModified);
            appendField(variation->daysSince1970);
            appendField(variation->variation);
            endRecord();
        }
    }
    endBlock();
}

void WindSpeedDataOutput::outputCsv(AsyncResponseStream *outputStream) {
    startBlock(outputStream);
    for ( int i = 0; i < MAX_WIND_SOURCES; i++) {
        WindData *wind = &dataCollector.wind[i];
        if ( wind->active() ) {
            startRecord("wind");
            appendField(wind->instance);
            appendField(wind->lastModified);
            appendField(wind->windSpeed);
            appendField(wind->windAngle);
            appendField(wind->windReference);
            endRecord();
        }
    }
    endBlock();
}

void LogDataOutput::outputCsv(AsyncResponseStream *outputStream) {
/*
11
#log,id,lastmodified,daysSince1970,secondsSinceMidnight,log,tripLog
t,83652827

log,0,83663614,18973,77353.00,19520,18520

*/

    startBlock(outputStream);
    for ( int i = 0; i < MAX_LOG_SOURCES; i++) {
        LogData *log = &dataCollector.log[i];
        if ( log->active() ) {
            startRecord("log");
            appendField(i);
            appendField(log->lastModified);
            appendField(log->daysSince1970);
            appendField(log->secondsSinceMidnight);
            appendField(log->log);
            appendField(log->tripLog);
            endRecord();
        }
    }
    endBlock();
}

void LatLonDataOutput::outputCsv(AsyncResponseStream *outputStream) {
/*
12
#position,id,lastmodified,latitude,longitude
t,83654897
position,0,83663914,54.33,4.342
*/
    startBlock(outputStream);
    for ( int i = 0; i < MAX_LOG_SOURCES; i++) {
        PossitionData *possition = &dataCollector.possition[i];
        if ( possition->active() ) {
            startRecord("possition");
            appendField(i);
            appendField(possition->lastModified);
            appendField(possition->latitude,8);
            appendField(possition->longitude,8);
            endRecord();
        }
    }
    endBlock();
}

void LeewayDataOutput::outputCsv(AsyncResponseStream *outputStream) {
/*
13
#leeway,id,lastmodified,leeway
t,83663681
leeway,0,83663815,1.982312
*/
    startBlock(outputStream);
    for ( int i = 0; i < MAX_LOG_SOURCES; i++) {
        LeewayData *leeway = &dataCollector.leeway[i];
        if ( leeway->active() ) {
            startRecord("leeway");
            appendField(leeway->instance);
            appendField(leeway->lastModified);
            appendField(leeway->leeway);
            endRecord();
        }
    }
    endBlock();
}



