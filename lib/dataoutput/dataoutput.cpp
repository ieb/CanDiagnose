
#include "dataoutput.h"


void DataCollector::HandleMsg(const tN2kMsg &N2kMsg) {
      switch(N2kMsg.PGN) {
//        case 60928L: AddressClaim(N2kMsg); break;
//        case 126992L: SystemTime(N2kMsg); break;
        case 127245L: Rudder(N2kMsg); break;
        case 127250L: Heading(N2kMsg); break;
        case 127257L: Attitude(N2kMsg); break;
        case 127488L: EngineRapid(N2kMsg); break;
        case 127489L: EngineDynamicParameters(N2kMsg); break;
//        case 127493L: TransmissionParameters(N2kMsg); break;
//        case 127497L: TripFuelConsumption(N2kMsg); break;
//        case 127501L: BinaryStatus(N2kMsg); break;
        case 127505L: FluidLevel(N2kMsg); break;
//        case 127506L: DCStatus(N2kMsg); break;
        case 127508L: DCBatteryStatus(N2kMsg); break;
//        case 127513L: BatteryConfigurationStatus(N2kMsg); break;
        case 128259L: Speed(N2kMsg); break;
        case 128267L: WaterDepth(N2kMsg); break;
        case 129026L: COGSOG(N2kMsg); break;
        case 129029L: GNSS(N2kMsg); break;
 //       case 129033L: LocalOffset(N2kMsg); break;
 //       case 129045L: UserDatumSettings(N2kMsg); break;
 //       case 129540L: GNSSSatsInView(N2kMsg); break;
        case 130310L: OutsideEnvironmental(N2kMsg); break;
        case 130312L: Temperature(N2kMsg); break;
        case 130313L: Humidity(N2kMsg); break;
        case 130314L: Pressure(N2kMsg); break;
        case 130316L: TemperatureExt(N2kMsg); break;

        case 129283L: Xte(N2kMsg); break;
        case 127258L: MagneticVariation(N2kMsg); break;
        case 130306L: WindSpeed(N2kMsg); break;
        case 128275L: Log(N2kMsg); break;
        case 129025L: LatLon(N2kMsg); break;
        case 128000L: Leeway(N2kMsg); break;

      }
}

void DataCollector::Attitude(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double Yaw;
    double Pitch;
    double Roll;
    
    if (ParseN2kAttitude(N2kMsg,SID,Yaw,Pitch,Roll) ) {
        if ( SID >= 0 && SID < MAX_ATTITUDE_SOURCES ) {
            attitude[SID].lastModified = millis();
            attitude[SID].yaw = Yaw;
            attitude[SID].pitch = Pitch;
            attitude[SID].roll = Roll;
        }
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataCollector::FluidLevel(const tN2kMsg &N2kMsg) {
    unsigned char Instance;
    tN2kFluidType FluidType;
    double Level=0;
    double Capacity=0;

    if (ParseN2kFluidLevel(N2kMsg,Instance,FluidType,Level,Capacity) ) {
        if ( Instance >= 0 && Instance < MAX_FLUID_LEVEL_SOURCES ) {
            fluidLevel[Instance].lastModified = millis();
            fluidLevel[Instance].fluidType = FluidType;
            fluidLevel[Instance].level = Level;
            fluidLevel[Instance].capacity = Capacity;
        }
    }
}



void DataCollector::EngineRapid(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    double EngineSpeed;
    double EngineBoostPressure;
    int8_t EngineTiltTrim;
    
    if (ParseN2kEngineParamRapid(N2kMsg,EngineInstance,EngineSpeed,EngineBoostPressure,EngineTiltTrim) ) {
        if ( EngineInstance >= 0 && EngineInstance < MAX_ENGINE_SOURCES ) {
            engine[EngineInstance].lastModified = millis();
            engine[EngineInstance].speed = EngineSpeed;
            engine[EngineInstance].boostPressure = EngineBoostPressure;
            engine[EngineInstance].tiltTrim = EngineTiltTrim;
        }
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}
void DataCollector::EngineDynamicParameters(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    double EngineOilPress;
    double EngineOilTemp;
    double EngineCoolantTemp;
    double AltenatorVoltage;
    double FuelRate;
    double EngineHours;
    double EngineCoolantPress;
    double EngineFuelPress; 
    int8_t EngineLoad;
    int8_t EngineTorque;
    tN2kEngineDiscreteStatus1 Status1;
    tN2kEngineDiscreteStatus2 Status2;
    
    if (ParseN2kEngineDynamicParam(N2kMsg,EngineInstance,EngineOilPress,EngineOilTemp,EngineCoolantTemp,
                                 AltenatorVoltage,FuelRate,EngineHours,
                                 EngineCoolantPress,EngineFuelPress,
                                 EngineLoad,EngineTorque,Status1,Status2) ) {
        if ( EngineInstance >= 0 && EngineInstance < MAX_ENGINE_SOURCES ) {
            engine[EngineInstance].lastModified = millis();
            engine[EngineInstance].OilPress = EngineOilPress;
            engine[EngineInstance].OilTemp = EngineOilTemp;
            engine[EngineInstance].CoolantTemp = EngineCoolantTemp;
            engine[EngineInstance].AltenatorVoltage = AltenatorVoltage;
            engine[EngineInstance].FuelRate = FuelRate;
            engine[EngineInstance].Hours = EngineHours;
            engine[EngineInstance].CoolantPress = EngineCoolantPress;
            engine[EngineInstance].FuelPress = EngineFuelPress; 
            engine[EngineInstance].Load = EngineLoad;
            engine[EngineInstance].Torque = EngineTorque;
            engine[EngineInstance].Status1 = Status1.Status;
            engine[EngineInstance].Status2 = Status2.Status;
        }
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

void DataCollector::Heading(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double Heading;
    double Deviation;
    double Variation;
    
    if (ParseN2kHeading(N2kMsg,SID,Heading,Deviation,Variation,HeadingReference) ) {
        if ( SID >= 0 && SID < MAX_HEADDING_SOURCES ) {
            heading[SID].lastModified = millis();
            heading[SID].heading = Heading;
            heading[SID].deviation = Deviation;
            heading[SID].variation = Variation;
            heading[SID].reference = HeadingReference;
        }
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataCollector::COGSOG(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double COG;
    double SOG;
    
    if (ParseN2kCOGSOGRapid(N2kMsg,SID,HeadingReference,COG,SOG) ) {
        if ( SID >= 0 && SID < MAX_COGSOG_SOURCES ) {
            cogSog[SID].lastModified = millis();
            cogSog[SID].cog = COG;
            cogSog[SID].sog = SOG;
            cogSog[SID].reference = HeadingReference;
        }
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataCollector::GNSS(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    uint16_t DaysSince1970;
    double SecondsSinceMidnight; 
    double Latitude;
    double Longitude;
    double Altitude; 
    tN2kGNSStype GNSStype;
    tN2kGNSSmethod GNSSmethod;
    unsigned char nSatellites;
    double HDOP;
    double PDOP;
    double GeoidalSeparation;
    unsigned char nReferenceStations;
    tN2kGNSStype ReferenceStationType;
    uint16_t ReferenceSationID;
    double AgeOfCorrection;

    if (ParseN2kGNSS(N2kMsg,SID,DaysSince1970,SecondsSinceMidnight,
                  Latitude,Longitude,Altitude,
                  GNSStype,GNSSmethod,
                  nSatellites,HDOP,PDOP,GeoidalSeparation,
                  nReferenceStations,ReferenceStationType,ReferenceSationID,
                  AgeOfCorrection) ) {
        if ( SID >= 0 && SID < MAX_GNSS_SOURCES ) {
            gnss[SID].lastModified = millis();
            gnss[SID].daysSince1970 = DaysSince1970;
            gnss[SID].secondsSinceMidnight = SecondsSinceMidnight;
            gnss[SID].latitude = Latitude;
            gnss[SID].longitude = Longitude;
            gnss[SID].altitude = Altitude;
            gnss[SID].type = GNSStype;
            gnss[SID].method = GNSSmethod;
            gnss[SID].nSatellites = nSatellites;
            gnss[SID].HDOP = HDOP;
            gnss[SID].PDOP = PDOP;
            gnss[SID].geoidalSeparation = GeoidalSeparation;
            gnss[SID].nReferenceStations = nReferenceStations;
            gnss[SID].referenceStationType = ReferenceStationType;
            gnss[SID].referenceStationID = ReferenceSationID;
            gnss[SID].ageOfCorrection = AgeOfCorrection;
        }
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

void DataCollector::OutsideEnvironmental(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double WaterTemperature;
    double OutsideAmbientAirTemperature;
    double AtmosphericPressure;
    
    if (ParseN2kOutsideEnvironmentalParameters(N2kMsg,SID,WaterTemperature,OutsideAmbientAirTemperature,AtmosphericPressure) ) {
        if ( SID >= 0 && SID < MAX_OUTSIDE_ENVIRONMENTAL_SOURCES ) {
            outsideEnvironmental[SID].lastModified = millis();
            outsideEnvironmental[SID].waterTemperature = WaterTemperature;
            outsideEnvironmental[SID].outsideAmbientAirTemperature = OutsideAmbientAirTemperature;
            outsideEnvironmental[SID].atmosphericPressure = AtmosphericPressure;
        }

    } else {
      outputStream->print("Failed to parse PGN: ");  outputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataCollector::Temperature(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;
    
    if (ParseN2kTemperature(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature) ) {
        if ( SID >= 0 && SID < MAX_TEMPERATURE_SOURCES ) {
            temperature[SID].lastModified = millis();
            temperature[SID].instance = TempInstance;
            temperature[SID].source = TempSource;
            temperature[SID].actual = ActualTemperature;
            temperature[SID].set = SetTemperature;
        }
    } else {
      outputStream->print("Failed to parse PGN: ");  outputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataCollector::Humidity(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char Instance;
    tN2kHumiditySource HumiditySource;
    double ActualHumidity,SetHumidity;
    
    if ( ParseN2kHumidity(N2kMsg,SID,Instance,HumiditySource,ActualHumidity,SetHumidity) ) {
        if ( SID >= 0 && SID < MAX_HUMIDITY_SOURCES ) {
            humidity[SID].lastModified = millis();
            humidity[SID].instance = Instance;
            humidity[SID].source = HumiditySource;
            humidity[SID].actual = ActualHumidity;
            humidity[SID].set = SetHumidity;
        }
    } else {
      outputStream->print("Failed to parse PGN: ");  outputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataCollector::Pressure(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char Instance;
    tN2kPressureSource PressureSource;
    double ActualPressure;
    
    if ( ParseN2kPressure(N2kMsg,SID,Instance,PressureSource,ActualPressure) ) {
        if ( SID >= 0 && SID < MAX_PRESSURE_SOURCES ) {
            pressure[SID].lastModified = millis();
            pressure[SID].instance = Instance;
            pressure[SID].source = PressureSource;
            pressure[SID].actual = ActualPressure;
        }
    } else {
      outputStream->print("Failed to parse PGN: ");  outputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataCollector::TemperatureExt(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;
    
    if (ParseN2kTemperatureExt(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature) ) {
        if ( SID >= 0 && SID < MAX_TEMPERATURE_SOURCES ) {
            temperatureExt[SID].lastModified = millis();
            temperatureExt[SID].instance = TempInstance;
            temperatureExt[SID].source = TempSource;
            temperatureExt[SID].actual = ActualTemperature;
            temperatureExt[SID].set = SetTemperature;
        }
    } else {
      outputStream->print("Failed to parse PGN: ");  outputStream->println(N2kMsg.PGN);
    }
}

void DataCollector::DCBatteryStatus(const tN2kMsg &N2kMsg) {
    byte BatteryInstance;
    double BatteryVoltage;
    double BatteryCurrent;
    double BatteryTemperature;
    unsigned char SID;

    if (ParseN2kDCBatStatus(N2kMsg, BatteryInstance,BatteryVoltage,BatteryCurrent,BatteryTemperature,SID)) {
        if ( SID >= 0 && SID < MAX_BATTERY_SOURCES ) {
            dcBattery[SID].lastModified = millis();
            dcBattery[SID].instance = BatteryInstance;
            dcBattery[SID].voltage = BatteryVoltage;
            dcBattery[SID].current = BatteryCurrent;
            dcBattery[SID].temperature = BatteryTemperature;
        }

    } else {
      outputStream->print("Failed to parse PGN: ");  outputStream->println(N2kMsg.PGN);
    }
}


//*****************************************************************************
void DataCollector::Speed(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double SOW;
    double SOG;
    tN2kSpeedWaterReferenceType SWRT;

    if (ParseN2kBoatSpeed(N2kMsg,SID,SOW,SOG,SWRT) ) {
        if ( SID >= 0 && SID < MAX_SPEED_SOURCES ) {
            speed[SID].lastModified = millis();
            speed[SID].sow = SOW;
            speed[SID].sog = SOG;
            speed[SID].swrt = SWRT;
        }
    }
}

//*****************************************************************************
void DataCollector::WaterDepth(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double DepthBelowTransducer;
    double Offset;

    if (ParseN2kWaterDepth(N2kMsg,SID,DepthBelowTransducer,Offset) ) {
        if ( SID >= 0 && SID < MAX_WATER_DEPTH_SOURCES ) {
            waterDepth[SID].lastModified = millis();
            if ( N2kIsNA(Offset) ) {
                waterDepth[SID].offset = 0;
            } else {
                waterDepth[SID].offset = Offset;
            }
            waterDepth[SID].depthBelowTransducer = DepthBelowTransducer;
        }
    }
}

void DataCollector::Rudder(const tN2kMsg &N2kMsg) {
    unsigned char Instance;
    tN2kRudderDirectionOrder RudderDirectionOrder;
    double RudderPosition;
    double AngleOrder;
    
    if (ParseN2kRudder(N2kMsg,RudderPosition,Instance,RudderDirectionOrder,AngleOrder) ) {
        if ( Instance >= 0 && Instance < MAX_RUDDER_SOURCES ) {
            rudder[Instance].lastModified = millis();
            rudder[Instance].position = RudderPosition;
            rudder[Instance].directionOrder = RudderDirectionOrder;
            rudder[Instance].angleOrder = AngleOrder;
        }
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

void DataCollector::Xte(const tN2kMsg &N2kMsg) {
    // 129283L
    unsigned char Instance;
    tN2kXTEMode xteMode;
    bool navTerminated;
    double xte;

    
    if (ParseN2kXTE(N2kMsg,Instance, xteMode, navTerminated, xte) ) {
        this->xte.lastModified = millis();
        this->xte.xte = xte;
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

void DataCollector::MagneticVariation(const tN2kMsg &N2kMsg) {
    // 127258L
    unsigned char Instance;
    tN2kMagneticVariation source;
    uint16_t daysSince1970;
    double variation;
    // captured data
    // 228787 : Pri:6 PGN:127258 Source:3 Dest:255 Len:8 Data:FF,FF,FF,FF,FF,7F,FF,FF
    // SID FF
    // Source 0F
    // days FF,FF
    // variaton FF,7F
    // ignore instance.
    

    if (ParseN2kMagneticVariation(N2kMsg,Instance,source,daysSince1970, variation) ) {
        this->variation.lastModified = millis();
        this->variation.daysSince1970 = daysSince1970;
        this->variation.variation = variation;
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

void DataCollector::WindSpeed(const tN2kMsg &N2kMsg) {
    // 130306L
    unsigned char Instance;
    double windSpeed;
    double windAngle;
    tN2kWindReference windReference;
    // captured data shows an iTC5 outputs only apparent wind, nothing else on the bus
    // 239097 : Pri:2 PGN:130306 Source:105 Dest:255 Len:8 Data:0,9A,0,20,DA,FA,FF,FF
    // SID = 0
    // windspeed 9A00
    // wind Andle 20DA
    // windReference FA  == 0b11111010 == 2 aparent

    if (ParseN2kWindSpeed(N2kMsg,Instance,windSpeed, windAngle, windReference) ) {
        wind.lastModified = millis();
        wind.windSpeed = windSpeed;
        wind.windAngle = windAngle;
        wind.windReference = windReference;
    } else {
        outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

void DataCollector::Log(const tN2kMsg &N2kMsg) {
    // 128275L
    uint16_t daysSince1970;
    double secondsSinceMidnight;
    uint32_t log;
    uint32_t tripLog;

// 236928 : Pri:6 PGN:128275 Source:105 Dest:255 Len:14 Data:FF,FF,FF,FF,FF,FF,FC,85,9,0,FC,85,9,0

    if (ParseN2kDistanceLog(N2kMsg,daysSince1970,secondsSinceMidnight,log,tripLog) ) {
        this->log.lastModified = millis();
        this->log.daysSince1970 = daysSince1970;
        this->log.secondsSinceMidnight = secondsSinceMidnight;
        this->log.log = log;
        this->log.tripLog = tripLog;
    } else {
        outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

void DataCollector::LatLon(const tN2kMsg &N2kMsg) {
    // 129025L
    double latitude;
    double longitude;
    
 
    if (ParseN2kPositionRapid(N2kMsg,latitude, longitude) ) {
        possition.lastModified = millis();
        possition.latitude = latitude;
        possition.longitude = longitude;
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}

void DataCollector::Leeway(const tN2kMsg &N2kMsg) {
    // 128000L
    unsigned char Instance;
    double leeway;


    if (ParseN2kLeeway(N2kMsg, Instance, leeway) ) {
        this->leeway.lastModified = millis();
        this->leeway.leeway = leeway;
    } else {
      outputStream->print("Failed to parse PGN: "); outputStream->println(N2kMsg.PGN);
    }
}




void EngineDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("engine");
    for (int i=0; i<MAX_ENGINE_SOURCES; i++) {
        EngineData *engine =  &dataCollector->engine[i];
        startObject();
        append("id",i);
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
        DcBatteryData *battery =  &dataCollector->dcBattery[i];
        startObject();
        append("id",i);
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
        FluidLevelData *fluidLevel =  &dataCollector->fluidLevel[i];
        startObject();
        append("id",i);
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
    for (int i=0; i<MAX_SPEED_SOURCES; i++) {
        HeadingData *heading =  &dataCollector->heading[i];
        startObject();
        append("id",i);
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
        SpeedData *speed =  &dataCollector->speed[i];
        startObject();
        append("id",i);
        append("lastModified",speed->lastModified);
        append("sow",speed->sow);
        append("sog",speed->sog);
        append("swrt",speed->swrt);
        endObject();
    }
    endArray();
    startArray("waterDepth");
    for (int i=0; i<MAX_WATER_DEPTH_SOURCES; i++) {
        WaterDepthData *waterDepth =  &dataCollector->waterDepth[i];
        startObject();
        append("id",i);
        append("lastModified",waterDepth->lastModified);
        append("depthBelowTransducer",waterDepth->depthBelowTransducer);
        append("offset",waterDepth->offset);
        endObject();
    }
    endArray();
    startArray("rudder");
    for (int i=0; i<MAX_RUDDER_SOURCES; i++) {
        RudderData *rudder =  &dataCollector->rudder[i];
        startObject();
        append("id",i);
        append("lastModified",rudder->lastModified);
        append("directionOrder",rudder->directionOrder);
        append("position",rudder->position);
        append("angleOrder",rudder->angleOrder);
        endObject();
    }
    endArray();
    startArray("attitude");
    for (int i=0; i<MAX_ATTITUDE_SOURCES; i++) {
        AttitudeData *attitude =  &dataCollector->attitude[i];
        startObject();
        append("id",i);
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
        CogSogData *cogSog =  &dataCollector->cogSog[i];
        startObject();
        append("id",i);
        append("lastModified",cogSog->lastModified);
        append("cog",cogSog->cog);
        append("sog",cogSog->sog);
        append("reference",cogSog->reference);
        endObject();
    }
    endArray();
    startArray("gnss");
    for (int i=0; i<MAX_GNSS_SOURCES; i++) {
        GnssData *gnss =  &dataCollector->gnss[i];
        startObject();
        append("id",i);
        append("lastModified",gnss->lastModified);
        append("daysSince1970",gnss->daysSince1970);
        append("secondsSinceMidnight",gnss->secondsSinceMidnight);
        append("latitude",gnss->latitude);
        append("longitude",gnss->longitude);
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
        OutsideEnvironmentData *outsideEnvironmental =  &dataCollector->outsideEnvironmental[i];
        startObject();
        append("id",i);
        append("lastModified",outsideEnvironmental->lastModified);
        append("waterTemperature",outsideEnvironmental->waterTemperature);
        append("outsideAmbientAirTemperature",outsideEnvironmental->outsideAmbientAirTemperature);
        append("atmosphericPressure",outsideEnvironmental->atmosphericPressure);
        endObject();
    }
    endArray();
    startArray("humidity");
    for (int i=0; i<MAX_HUMIDITY_SOURCES; i++) {
        HumidityData *humidity =  &dataCollector->humidity[i];
        startObject();
        append("id",i);
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
        PressureData *pressure =  &dataCollector->pressure[i];
        startObject();
        append("id",i);
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
        TemperatureData *temperature =  &dataCollector->temperature[i];
        startObject();
        append("id",i);
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
        TemperatureData *temperatureExt =  &dataCollector->temperatureExt[i];
        startObject();
        append("id",i);
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
    startObject("xte");
    append("lastModified",dataCollector->xte.lastModified);
    append("xte",dataCollector->xte.xte);
    endObject();
    endJson();
}


void MagneticVariationDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startObject("variation");
    append("lastModified",dataCollector->variation.lastModified);
    append("daysSince1970",dataCollector->variation.daysSince1970);
    append("variation",dataCollector->variation.variation);
    endObject();
    endJson();
}

void WindSpeedDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startObject("wind");
    append("lastModified",dataCollector->wind.lastModified);
    append("windSpeed",dataCollector->wind.windSpeed);
    append("windAngle",dataCollector->wind.windAngle);
    append("windReference",dataCollector->wind.windReference);
    endObject();
    endJson();
}

void LogDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startObject("log");
    append("lastModified",dataCollector->log.lastModified);
    append("daysSince1970",dataCollector->log.daysSince1970);
    append("secondsSinceMidnight",dataCollector->log.secondsSinceMidnight);
    append("log",dataCollector->log.log);
    append("tripLog",dataCollector->log.tripLog);
    endObject();
    endJson();
}

void LatLonDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startObject("log");
    append("lastModified",dataCollector->possition.lastModified);
    append("latitude",dataCollector->possition.latitude);
    append("longitude",dataCollector->possition.longitude);
    endObject();
    endJson();
}


void LeewayDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startObject("log");
    append("lastModified",dataCollector->leeway.lastModified);
    append("leeway",dataCollector->leeway.leeway);
    endObject();
    endJson();
}



bool EngineDataOutput::drawPage(Adafruit_SSD1306 * display) {
    EngineData *engine =  &dataCollector->engine[0];
    FluidLevelData *fluidLevel =  &dataCollector->fluidLevel[0];

    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    switch(subPage) {
        case 0:
            display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
            display->setCursor(0,0);;  
            display->printf("%4.0f RPM\n",engine->speed); // RPM
            display->printf("%3.0f C\n", engine->CoolantTemp-273.0); // Coolant temperatire
            display->printf("%5.2f V a\n", engine->AltenatorVoltage); // Alternator Voltage
            display->printf("%5.1f h\n", engine->Hours); // Engine Hours
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
            display->setCursor(0,0);;  
            display->printf("Fuel\n%3.0f %%\n",fluidLevel->level); // Fuel %
            display->display();
            subPage = 0;
            return true;
        default:
            subPage = 0;
            return false;
    }
#endif

}

bool WindSpeedDataOutput::drawPage(Adafruit_SSD1306 * display) {

    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
    display->setCursor(0,12);              
    display->printf("AWS:%5.1f\n", msToKnots(dataCollector->wind.windSpeed)); // AWS
    display->setCursor(0,36);             
    if ( dataCollector->wind.windAngle < 0 ) {
        display->printf("AWA:%3.0fP\n", RadToDeg(dataCollector->wind.windAngle)); // AWA
    }  else {
        display->printf("AWA:%3.0fS\n", RadToDeg(dataCollector->wind.windAngle)); // Wind
    }
    display->display();
    return true;
#endif

}

bool LogDataOutput::drawPage(Adafruit_SSD1306 * display) {

    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
    display->setCursor(0,12);              
    display->printf("L:%5.1f Nm\n", dataCollector->log.log/1852.0); // Log
    display->setCursor(0,36);             
    display->printf("T:%5.1f Nm\n", dataCollector->log.tripLog/1852.0); // Trip
    display->display();
    return true;
#endif

}


bool LatLonDataOutput::drawPage(Adafruit_SSD1306 * display) {

    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
    display->setCursor(0,12);
    display->cp437(true);         // Use full 256 char 'Code Page 437' font
    int deg = (int) dataCollector->possition.latitude;
    double min = (double) 60.0*(dataCollector->possition.latitude-deg);
    if ( deg >= 0 ) {
        display->printf(" %02d%c%05.2fN\n", deg, 248, min); 
    } else {
        display->printf(" %02d%c%05.2fS\n", -deg, 248, -min); 
    }
    deg = (int) dataCollector->possition.latitude;
    min = (double) 60.0*(dataCollector->possition.latitude-deg);
    display->setCursor(0,36);             
    if ( deg >= 0 ) {
        display->printf("%03d%c%05.2fE\n", deg,248,min); 
    } else {
        display->printf("%03d%c%05.2fW\n", -deg,248,-min); 
    }
    display->cp437(false);         
    display->display();
    return true;
#endif

}
