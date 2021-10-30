
#include "dataoutput.h"

DataOutput::DataOutput(Stream *outputStream) :
        OutputStream{outputStream} {
}

void DataOutput::HandleMsg(const tN2kMsg &N2kMsg) {
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

void DataOutput::Attitude(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataOutput::FluidLevel(const tN2kMsg &N2kMsg) {
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



void DataOutput::EngineRapid(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}
void DataOutput::EngineDynamicParameters(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

void DataOutput::Heading(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataOutput::COGSOG(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataOutput::GNSS(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

void DataOutput::OutsideEnvironmental(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataOutput::Temperature(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataOutput::Humidity(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataOutput::Pressure(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataOutput::TemperatureExt(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

void DataOutput::DCBatteryStatus(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}


//*****************************************************************************
void DataOutput::Speed(const tN2kMsg &N2kMsg) {
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
void DataOutput::WaterDepth(const tN2kMsg &N2kMsg) {
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

void DataOutput::Rudder(const tN2kMsg &N2kMsg) {
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
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
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
void DataOutput::outputPackets(int packetId) {
        OutputStream->print(">");
        OutputStream->print(packetId); 
        OutputStream->print(","); 
        OutputStream->println(millis()); 
        switch (packetId) {
            case 0:
            OutputStream->println("?Engine");
            OutputStream->println("#SID,LastModified,Hours,speed,CoolantTemp,AltenatorVoltage,Status1,Status2");
            for (int i=0; i<MAX_ENGINE_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(engine[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(engine[i].Hours);
                OutputStream->print(",");
                OutputStream->print(engine[i].speed);
                OutputStream->print(",");
                OutputStream->print(engine[i].CoolantTemp);
                OutputStream->print(",");
                OutputStream->print(engine[i].AltenatorVoltage);
                OutputStream->print(",");
                OutputStream->print(engine[i].Status1);
                OutputStream->print(",");
                OutputStream->println(engine[i].Status2);
            }
            OutputStream->println("?Battery");
            OutputStream->println("#SID,lastModified,Instance,Voltage,Current,Temperature");
            for (int i=0; i<MAX_BATTERY_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(dcBattery[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(dcBattery[i].instance);
                OutputStream->print(",");
                OutputStream->print(dcBattery[i].voltage);
                OutputStream->print(",");
                OutputStream->print(dcBattery[i].current);
                OutputStream->print(",");
                OutputStream->println(dcBattery[i].temperature);
            }
            OutputStream->println("?FluidLevels");
            OutputStream->println("#SID,lastModified,level,capacity,fluidType");
            for (int i=0; i<MAX_FLUID_LEVEL_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(fluidLevel[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(fluidLevel[i].level);
                OutputStream->print(",");
                OutputStream->print(fluidLevel[i].capacity);
                OutputStream->print(",");
                OutputStream->println(fluidLevel[i].fluidType);
            }
            break;


        case 1:
            OutputStream->println("?Heading");
            OutputStream->println("#SID,lastModified,heading,deviation,variation,reference");
            for (int i=0; i<MAX_SPEED_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(heading[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(heading[i].heading);
                OutputStream->print(",");
                OutputStream->print(heading[i].deviation);
                OutputStream->print(",");
                OutputStream->print(heading[i].variation);
                OutputStream->print(",");
                OutputStream->println(heading[i].reference);
            }
            OutputStream->println("?Speed");
            OutputStream->println("#SID,lastModified,sow,sog,swrt");
            for (int i=0; i<MAX_SPEED_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(speed[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(speed[i].sow);
                OutputStream->print(",");
                OutputStream->print(speed[i].sog);
                OutputStream->print(",");
                OutputStream->println(speed[i].swrt);
            }



            OutputStream->println("?WaterDepth");
            OutputStream->println("#SID,lastModified,depthBelowTransducer,offset");
            for (int i=0; i<MAX_WATER_DEPTH_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(waterDepth[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(waterDepth[i].depthBelowTransducer);
                OutputStream->print(",");
                OutputStream->println(waterDepth[i].offset);
            }

            OutputStream->println("?Rudder");
            OutputStream->println("#SID,lastModified,directionOrder,position,angleOrder");
            for (int i=0; i<MAX_RUDDER_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(rudder[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(rudder[i].directionOrder);
                OutputStream->print(",");
                OutputStream->print(rudder[i].position);
                OutputStream->print(",");
                OutputStream->println(rudder[i].angleOrder);
            }


            OutputStream->println("?Attitude");
            OutputStream->println("#SID,lastModified,roll,pitch,yaw");
            for (int i=0; i<MAX_ATTITUDE_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(attitude[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(attitude[i].roll);
                OutputStream->print(",");
                OutputStream->print(attitude[i].pitch);
                OutputStream->print(",");
                OutputStream->println(attitude[i].yaw);
            }
            break;

        case 2:

            OutputStream->println("?CogSog");
            OutputStream->println("#SID,lastModified,cog,sog,reference");
            for (int i=0; i<MAX_COGSOG_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(cogSog[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(cogSog[i].cog);
                OutputStream->print(",");
                OutputStream->print(cogSog[i].sog);
                OutputStream->print(",");
                OutputStream->println(cogSog[i].reference);
            }



            OutputStream->println("?Gnss");
            OutputStream->println("#SID,lastModified,daysSince1970,secondsSinceMidnight,latitude,longitude,altitude,type,method,nSatelites,HDOP,PDOP,geoidalSeparation,nReferenceStations,referenceStationType,ageOfCorrection");
            for (int i=0; i<MAX_GNSS_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(gnss[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(gnss[i].daysSince1970);
                OutputStream->print(",");
                OutputStream->print(gnss[i].secondsSinceMidnight);
                OutputStream->print(",");
                OutputStream->print(gnss[i].latitude);
                OutputStream->print(",");
                OutputStream->print(gnss[i].longitude);
                OutputStream->print(",");
                OutputStream->print(gnss[i].altitude);
                OutputStream->print(",");
                OutputStream->print(gnss[i].type);
                OutputStream->print(",");
                OutputStream->print(gnss[i].method);
                OutputStream->print(",");
                OutputStream->print(gnss[i].nSatellites);
                OutputStream->print(",");
                OutputStream->print(gnss[i].HDOP);
                OutputStream->print(",");
                OutputStream->print(gnss[i].PDOP);
                OutputStream->print(",");
                OutputStream->print(gnss[i].geoidalSeparation);
                OutputStream->print(",");
                OutputStream->print(gnss[i].nReferenceStations);
                OutputStream->print(",");
                OutputStream->print(gnss[i].referenceStationType);
                OutputStream->print(",");
                OutputStream->println(gnss[i].ageOfCorrection);
            }
            break;
        case 3:


            OutputStream->println("?OutsideEnvironment");
            OutputStream->println("#SID,lastModified,waterTemperature,ActuaoutsideAmbientAirTemperaturel,atmosphericPressure");
            for (int i=0; i<MAX_OUTSIDE_ENVIRONMENTAL_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(outsideEnvironmental[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(outsideEnvironmental[i].waterTemperature);
                OutputStream->print(",");
                OutputStream->print(outsideEnvironmental[i].outsideAmbientAirTemperature);
                OutputStream->print(",");
                OutputStream->println(outsideEnvironmental[i].atmosphericPressure);
            }

            OutputStream->println("?Humidity");
            OutputStream->println("#SID,lastModified,instance,source,actual,set");
            for (int i=0; i<MAX_OUTSIDE_ENVIRONMENTAL_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(humidity[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(humidity[i].instance);
                OutputStream->print(",");
                OutputStream->print(humidity[i].source);
                OutputStream->print(",");
                OutputStream->print(humidity[i].actual);
                OutputStream->print(",");
                OutputStream->println(humidity[i].set);
            }

            OutputStream->println("?Pressure");
            OutputStream->println("#SID,lastModified,instance,source,actual");
            for (int i=0; i<MAX_PRESSURE_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(pressure[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(pressure[i].instance);
                OutputStream->print(",");
                OutputStream->print(pressure[i].source);
                OutputStream->print(",");
                OutputStream->println(pressure[i].actual);
            }


           break;

        case 4:

            OutputStream->println("?Temperature");
            OutputStream->println("#SID,lastModified,Instance,TempSource,Actual,Set");
            for (int i=0; i<MAX_TEMPERATURE_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(temperature[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(temperature[i].instance);
                OutputStream->print(",");
                OutputStream->print(temperature[i].source);
                OutputStream->print(",");
                OutputStream->print(temperature[i].actual);
                OutputStream->print(",");
                OutputStream->println(temperature[i].set);
            }
            OutputStream->println("?TemperatureExt");
            OutputStream->println("#SID,lastModified,Instance,TempSource,Actual,Set");
            for (int i=0; i<MAX_TEMPERATURE_SOURCES; i++) {
                OutputStream->print("!");
                OutputStream->print(i);
                OutputStream->print(",");
                OutputStream->print(temperatureExt[i].lastModified);
                OutputStream->print(",");
                OutputStream->print(temperatureExt[i].instance);
                OutputStream->print(",");
                OutputStream->print(temperatureExt[i].source);
                OutputStream->print(",");
                OutputStream->print(temperatureExt[i].actual);
                OutputStream->print(",");
                OutputStream->println(temperatureExt[i].set);
            }
            break;
        }
        OutputStream->print("<");
        OutputStream->println(packetId); 

}



