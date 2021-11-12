
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




/**
 * Format of the outpur packets is
 * ><packetId> Start of Packet
 * #<fields in packet csv>
 * 
 * !<data csv>
 * <<packetid> End of Packet
 * 
 * The packet will always start with a > 
 * ? indicates the start of a dataset with a name
 * # indicates the fields
 * ! indicates the dataset
 * ? indicates the next dataset with a name
 * etc
 * < indicates the end of the dataset.
 */



void EngineDataOutput::outputText(Stream *outputStream) {
        outputStream->print(">0,"); 
        outputStream->println(millis()); 
        outputStream->println("?Engine");
        outputStream->println("#SID,LastModified,Hours,speed,CoolantTemp,AltenatorVoltage,Status1,Status2");

        for (int i=0; i<MAX_ENGINE_SOURCES; i++) {
            EngineData *engine =  &dataCollector->engine[i];
            outputStream->print("!");
            outputStream->print(i);
            outputStream->print(",");
            outputStream->print(engine->lastModified);
            outputStream->print(",");
            outputStream->print(engine->Hours);
            outputStream->print(",");
            outputStream->print(engine->speed);
            outputStream->print(",");
            outputStream->print(engine->CoolantTemp);
            outputStream->print(",");
            outputStream->print(engine->AltenatorVoltage);
            outputStream->print(",");
            outputStream->print(engine->Status1);
            outputStream->print(",");
            outputStream->println(engine->Status2);
        }
        outputStream->println("?Battery");
        outputStream->println("#SID,lastModified,Instance,Voltage,Current,Temperature");
        for (int i=0; i<MAX_BATTERY_SOURCES; i++) {
            DcBatteryData *battery =  &dataCollector->dcBattery[i];
            outputStream->print("!");
            outputStream->print(i);
            outputStream->print(",");
            outputStream->print(battery->lastModified);
            outputStream->print(",");
            outputStream->print(battery->instance);
            outputStream->print(",");
            outputStream->print(battery->voltage);
            outputStream->print(",");
            outputStream->print(battery->current);
            outputStream->print(",");
            outputStream->println(battery->temperature);
        }
        outputStream->println("?FluidLevels");
        outputStream->println("#SID,lastModified,level,capacity,fluidType");
        for (int i=0; i<MAX_FLUID_LEVEL_SOURCES; i++) {
            outputStream->print("!");
            FluidLevelData *fluidLevel =  &dataCollector->fluidLevel[i];
            outputStream->print(i);
            outputStream->print(",");
            outputStream->print(fluidLevel->lastModified);
            outputStream->print(",");
            outputStream->print(fluidLevel->level);
            outputStream->print(",");
            outputStream->print(fluidLevel->capacity);
            outputStream->print(",");
            outputStream->println(fluidLevel->fluidType);
        }
        outputStream->println("<0");
}
void EngineDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
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


void BoatDataOutput::outputText(Stream *outputStream) {
        outputStream->print(">1,");
        outputStream->println(millis()); 
        outputStream->println("?Heading");
        outputStream->println("#SID,lastModified,heading,deviation,variation,reference");
        for (int i=0; i<MAX_SPEED_SOURCES; i++) {
            HeadingData *heading =  &dataCollector->heading[i];
            outputStream->print("!");
            outputStream->print(i);
            outputStream->print(",");
            outputStream->print(heading->lastModified);
            outputStream->print(",");
            outputStream->print(heading->heading);
            outputStream->print(",");
            outputStream->print(heading->deviation);
            outputStream->print(",");
            outputStream->print(heading->variation);
            outputStream->print(",");
            outputStream->println(heading->reference);
        }
        outputStream->println("?Speed");
        outputStream->println("#SID,lastModified,sow,sog,swrt");
        for (int i=0; i<MAX_SPEED_SOURCES; i++) {
            SpeedData *speed =  &dataCollector->speed[i];
            outputStream->print("!");
            outputStream->print(i);
            outputStream->print(",");
            outputStream->print(speed->lastModified);
            outputStream->print(",");
            outputStream->print(speed->sow);
            outputStream->print(",");
            outputStream->print(speed->sog);
            outputStream->print(",");
            outputStream->println(speed->swrt);
        }



        outputStream->println("?WaterDepth");
        outputStream->println("#SID,lastModified,depthBelowTransducer,offset");
        for (int i=0; i<MAX_WATER_DEPTH_SOURCES; i++) {
            WaterDepthData *waterDepth =  &dataCollector->waterDepth[i];
            outputStream->print("!");
            outputStream->print(i);
            outputStream->print(",");
            outputStream->print(waterDepth->lastModified);
            outputStream->print(",");
            outputStream->print(waterDepth->depthBelowTransducer);
            outputStream->print(",");
            outputStream->println(waterDepth->offset);
        }

        outputStream->println("?Rudder");
        outputStream->println("#SID,lastModified,directionOrder,position,angleOrder");
        for (int i=0; i<MAX_RUDDER_SOURCES; i++) {
            RudderData *rudder =  &dataCollector->rudder[i];
            outputStream->print("!");
            outputStream->print(i);
            outputStream->print(",");
            outputStream->print(rudder->lastModified);
            outputStream->print(",");
            outputStream->print(rudder->directionOrder);
            outputStream->print(",");
            outputStream->print(rudder->position);
            outputStream->print(",");
            outputStream->println(rudder->angleOrder);
        }


        outputStream->println("?Attitude");
        outputStream->println("#SID,lastModified,roll,pitch,yaw");
        for (int i=0; i<MAX_ATTITUDE_SOURCES; i++) {
            AttitudeData *attitude =  &dataCollector->attitude[i];
            outputStream->print("!");
            outputStream->print(i);
            outputStream->print(",");
            outputStream->print(attitude->lastModified);
            outputStream->print(",");
            outputStream->print(attitude->roll);
            outputStream->print(",");
            outputStream->print(attitude->pitch);
            outputStream->print(",");
            outputStream->println(attitude->yaw);
        }


        outputStream->println("<1");
}

void BoatDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
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


void NavigationDataOutput::outputText(Stream *outputStream) {
    outputStream->print(">2,");
    outputStream->println(millis()); 
    outputStream->println("?CogSog");
    outputStream->println("#SID,lastModified,cog,sog,reference");
    for (int i=0; i<MAX_COGSOG_SOURCES; i++) {
        CogSogData *cogSog =  &dataCollector->cogSog[i];
        outputStream->print("!");
        outputStream->print(i);
        outputStream->print(",");
        outputStream->print(cogSog->lastModified);
        outputStream->print(",");
        outputStream->print(cogSog->cog);
        outputStream->print(",");
        outputStream->print(cogSog->sog);
        outputStream->print(",");
        outputStream->println(cogSog->reference);
    }
    outputStream->println("?Gnss");
    outputStream->println("#SID,lastModified,daysSince1970,secondsSinceMidnight,latitude,longitude,altitude,type,method,nSatelites,HDOP,PDOP,geoidalSeparation,nReferenceStations,referenceStationType,ageOfCorrection");
    for (int i=0; i<MAX_GNSS_SOURCES; i++) {
        GnssData *gnss =  &dataCollector->gnss[i];
        outputStream->print("!");
        outputStream->print(i);
        outputStream->print(",");
        outputStream->print(gnss->lastModified);
        outputStream->print(",");
        outputStream->print(gnss->daysSince1970);
        outputStream->print(",");
        outputStream->print(gnss->secondsSinceMidnight);
        outputStream->print(",");
        outputStream->print(gnss->latitude);
        outputStream->print(",");
        outputStream->print(gnss->longitude);
        outputStream->print(",");
        outputStream->print(gnss->altitude);
        outputStream->print(",");
        outputStream->print(gnss->type);
        outputStream->print(",");
        outputStream->print(gnss->method);
        outputStream->print(",");
        outputStream->print(gnss->nSatellites);
        outputStream->print(",");
        outputStream->print(gnss->HDOP);
        outputStream->print(",");
        outputStream->print(gnss->PDOP);
        outputStream->print(",");
        outputStream->print(gnss->geoidalSeparation);
        outputStream->print(",");
        outputStream->print(gnss->nReferenceStations);
        outputStream->print(",");
        outputStream->print(gnss->referenceStationType);
        outputStream->print(",");
        outputStream->println(gnss->ageOfCorrection);
    }
    outputStream->println("<2");
}

void NavigationDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
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


void EnvironmentDataOutput::outputText(Stream *outputStream) {
    outputStream->print(">3,");
    outputStream->println(millis()); 
    outputStream->println("?OutsideEnvironment");
    outputStream->println("#SID,lastModified,waterTemperature,ActuaoutsideAmbientAirTemperaturel,atmosphericPressure");
    for (int i=0; i<MAX_OUTSIDE_ENVIRONMENTAL_SOURCES; i++) {
        OutsideEnvironmentData *outsideEnvironmental =  &dataCollector->outsideEnvironmental[i];
        outputStream->print("!");
        outputStream->print(i);
        outputStream->print(",");
        outputStream->print(outsideEnvironmental->lastModified);
        outputStream->print(",");
        outputStream->print(outsideEnvironmental->waterTemperature);
        outputStream->print(",");
        outputStream->print(outsideEnvironmental->outsideAmbientAirTemperature);
        outputStream->print(",");
        outputStream->println(outsideEnvironmental->atmosphericPressure);
    }

    outputStream->println("?Humidity");
    outputStream->println("#SID,lastModified,instance,source,actual,set");
    for (int i=0; i<MAX_HUMIDITY_SOURCES; i++) {
        HumidityData *humidity =  &dataCollector->humidity[i];
        outputStream->print("!");
        outputStream->print(i);
        outputStream->print(",");
        outputStream->print(humidity->lastModified);
        outputStream->print(",");
        outputStream->print(humidity->instance);
        outputStream->print(",");
        outputStream->print(humidity->source);
        outputStream->print(",");
        outputStream->print(humidity->actual);
        outputStream->print(",");
        outputStream->println(humidity->set);
    }

    outputStream->println("?Pressure");
    outputStream->println("#SID,lastModified,instance,source,actual");
    for (int i=0; i<MAX_PRESSURE_SOURCES; i++) {
        PressureData *pressure =  &dataCollector->pressure[i];
        outputStream->print("!");
        outputStream->print(i);
        outputStream->print(",");
        outputStream->print(pressure->lastModified);
        outputStream->print(",");
        outputStream->print(pressure->instance);
        outputStream->print(",");
        outputStream->print(pressure->source);
        outputStream->print(",");
        outputStream->println(pressure->actual);
    }
    outputStream->println("<3");
}

void EnvironmentDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
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


void TemperatureDataOutput::outputText(Stream *outputStream) {
    outputStream->print(">4,");
    outputStream->println(millis()); 
    outputStream->println("?Temperature");
    outputStream->println("#SID,lastModified,Instance,TempSource,Actual,Set");
    for (int i=0; i<MAX_TEMPERATURE_SOURCES; i++) {
        TemperatureData *temperature =  &dataCollector->temperature[i];
        outputStream->print("!");
        outputStream->print(i);
        outputStream->print(",");
        outputStream->print(temperature->lastModified);
        outputStream->print(",");
        outputStream->print(temperature->instance);
        outputStream->print(",");
        outputStream->print(temperature->source);
        outputStream->print(",");
        outputStream->print(temperature->actual);
        outputStream->print(",");
        outputStream->println(temperature->set);
    }
    outputStream->println("?TemperatureExt");
    outputStream->println("#SID,lastModified,Instance,TempSource,Actual,Set");
    for (int i=0; i<MAX_TEMPERATURE_SOURCES; i++) {
        TemperatureData *temperatureExt =  &dataCollector->temperatureExt[i];
        outputStream->print("!");
        outputStream->print(i);
        outputStream->print(",");
        outputStream->print(temperatureExt->lastModified);
        outputStream->print(",");
        outputStream->print(temperatureExt->instance);
        outputStream->print(",");
        outputStream->print(temperatureExt->source);
        outputStream->print(",");
        outputStream->print(temperatureExt->actual);
        outputStream->print(",");
        outputStream->println(temperatureExt->set);
    }

    outputStream->println("<4");
}

void TemperatureDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
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

