
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
        case 130311L: EnvironmentalParameters(N2kMsg); break;
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

void DataCollector::saveFailed(const tN2kMsg &N2kMsg, byte instance) {
    outputStream->print("Failed to save PGN: "); 
    outputStream->print(N2kMsg.PGN);
    if ( instance != 255 ) {
        outputStream->print(" instance ");
        outputStream->print(instance);
    }
    outputStream->print(" from "); 
    outputStream->println(N2kMsg.Source);
}
void DataCollector::parseFailed(const tN2kMsg &N2kMsg) {
    outputStream->print("Failed to parse PGN: "); 
    outputStream->print(N2kMsg.PGN);
    outputStream->print(" from "); 
    outputStream->println(N2kMsg.Source);
}



void DataCollector::Attitude(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double Yaw;
    double Pitch;
    double Roll;
    
    if (ParseN2kAttitude(N2kMsg,SID,Yaw,Pitch,Roll) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_ATTITUDE_SOURCES; i++) {
                if ( attitude[i].use(N2kMsg.Source, i, reuse) ) {
                    attitude[i].yaw = Yaw;
                    attitude[i].pitch = Pitch;
                    attitude[i].roll = Roll;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

//*****************************************************************************
void DataCollector::FluidLevel(const tN2kMsg &N2kMsg) {
    unsigned char Instance;
    tN2kFluidType FluidType;
    double Level=0;
    double Capacity=0;

    if (ParseN2kFluidLevel(N2kMsg,Instance,FluidType,Level,Capacity) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_FLUID_LEVEL_SOURCES; i++) {
                if ( fluidLevel[i].use(N2kMsg.Source, Instance, reuse) ) {
                    fluidLevel[i].fluidType = FluidType;
                    fluidLevel[i].level = Level;
                    fluidLevel[i].capacity = Capacity;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}



void DataCollector::EngineRapid(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    double EngineSpeed;
    double EngineBoostPressure;
    int8_t EngineTiltTrim;
    
    if (ParseN2kEngineParamRapid(N2kMsg,EngineInstance,EngineSpeed,EngineBoostPressure,EngineTiltTrim) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for ( int i = 0; i < MAX_ENGINE_SOURCES; i++) {
                if ( engine[i].use(N2kMsg.Source, EngineInstance, reuse) ) {
                    engine[i].speed = EngineSpeed;
                    engine[i].boostPressure = EngineBoostPressure;
                    engine[i].tiltTrim = EngineTiltTrim;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
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
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_ENGINE_SOURCES; i++ ) {
                if ( engine[i].use(N2kMsg.Source, EngineInstance, reuse)) {
                    engine[i].OilPress = EngineOilPress;
                    engine[i].OilTemp = EngineOilTemp;
                    engine[i].CoolantTemp = EngineCoolantTemp;
                    engine[i].AltenatorVoltage = AltenatorVoltage;
                    engine[i].FuelRate = FuelRate;
                    engine[i].Hours = EngineHours;
                    engine[i].CoolantPress = EngineCoolantPress;
                    engine[i].FuelPress = EngineFuelPress; 
                    engine[i].Load = EngineLoad;
                    engine[i].Torque = EngineTorque;
                    engine[i].Status1 = Status1.Status;
                    engine[i].Status2 = Status2.Status;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

void DataCollector::Heading(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double Heading;
    double Deviation;
    double Variation;
    
    if (ParseN2kHeading(N2kMsg,SID,Heading,Deviation,Variation,HeadingReference) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i< MAX_HEADDING_SOURCES; i++) {
                if ( heading[i].use(N2kMsg.Source, i, reuse)) {
                    heading[i].heading = Heading;
                    heading[i].deviation = Deviation;
                    heading[i].variation = Variation;
                    heading[i].reference = HeadingReference;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

//*****************************************************************************
void DataCollector::COGSOG(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double COG;
    double SOG;
    
    if (ParseN2kCOGSOGRapid(N2kMsg,SID,HeadingReference,COG,SOG) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for ( int i = 0; i < MAX_COGSOG_SOURCES; i++) {
                if ( cogSog[i].use(N2kMsg.Source, i, reuse)) {
                    cogSog[i].cog = COG;
                    cogSog[i].sog = SOG;
                    cogSog[i].reference = HeadingReference;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
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
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for ( int i = 0; i < MAX_GNSS_SOURCES; i++) {
                if ( gnss[i].use(N2kMsg.Source, i, reuse)) {
                    gnss[i].daysSince1970 = DaysSince1970;
                    gnss[i].secondsSinceMidnight = SecondsSinceMidnight;
                    gnss[i].latitude = Latitude;
                    gnss[i].longitude = Longitude;
                    gnss[i].altitude = Altitude;
                    gnss[i].type = GNSStype;
                    gnss[i].method = GNSSmethod;
                    gnss[i].nSatellites = nSatellites;
                    gnss[i].HDOP = HDOP;
                    gnss[i].PDOP = PDOP;
                    gnss[i].geoidalSeparation = GeoidalSeparation;
                    gnss[i].nReferenceStations = nReferenceStations;
                    gnss[i].referenceStationType = ReferenceStationType;
                    gnss[i].referenceStationID = ReferenceSationID;
                    gnss[i].ageOfCorrection = AgeOfCorrection;
                    return;
                }
            }

            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

void DataCollector::EnvironmentalParameters(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double Temperature;
    double Humidity;
    double AtmosphericPressure;
    tN2kTempSource TempSource;
    tN2kHumiditySource HumiditySource;
    

    if ( ParseN2kEnvironmentalParameters(N2kMsg, SID, TempSource,Temperature,
                    HumiditySource, Humidity, AtmosphericPressure) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_ENVIRONMENTAL_PARAMS_SOURCES; i++) {
                if ( environmentalParams[i].use(N2kMsg.Source, i, reuse) ) {
                    environmentalParams[i].temperatureSource = TempSource;
                    environmentalParams[i].humiditySource = HumiditySource;
                    environmentalParams[i].temperature = Temperature;
                    environmentalParams[i].humidity = Humidity;
                    environmentalParams[i].atmosphericPressure = AtmosphericPressure;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
    

}

void DataCollector::OutsideEnvironmental(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double WaterTemperature;
    double OutsideAmbientAirTemperature;
    double AtmosphericPressure;
    
    if (ParseN2kOutsideEnvironmentalParameters(N2kMsg,SID,WaterTemperature,OutsideAmbientAirTemperature,AtmosphericPressure) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_OUTSIDE_ENVIRONMENTAL_SOURCES; i++) {
                if ( outsideEnvironmental[i].use(N2kMsg.Source, i, reuse) ) {
                    outsideEnvironmental[i].waterTemperature = WaterTemperature;
                    outsideEnvironmental[i].outsideAmbientAirTemperature = OutsideAmbientAirTemperature;
                    outsideEnvironmental[i].atmosphericPressure = AtmosphericPressure;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
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
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_TEMPERATURE_SOURCES; i++ ) {
                if ( temperature[i].use(N2kMsg.Source, TempInstance, reuse)) {
                    temperature[i].sourceSensor = TempSource;
                    temperature[i].actual = ActualTemperature;
                    temperature[i].set = SetTemperature;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

//*****************************************************************************
void DataCollector::Humidity(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char Instance;
    tN2kHumiditySource HumiditySource;
    double ActualHumidity,SetHumidity;
    
    if ( ParseN2kHumidity(N2kMsg,SID,Instance,HumiditySource,ActualHumidity,SetHumidity) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for ( int i = 0; i < MAX_HUMIDITY_SOURCES; i++ ) {
                if ( humidity[i].use(N2kMsg.Source, Instance, reuse)) {
                    humidity[i].sourceSensor = HumiditySource;
                    humidity[i].actual = ActualHumidity;
                    humidity[i].set = SetHumidity;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

//*****************************************************************************
void DataCollector::Pressure(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char Instance;
    tN2kPressureSource PressureSource;
    double ActualPressure;
    
    if ( ParseN2kPressure(N2kMsg,SID,Instance,PressureSource,ActualPressure) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_PRESSURE_SOURCES; i++) {
                if (pressure[i].use(N2kMsg.Source, Instance, reuse)) {
                    pressure[i].sourceSensor = PressureSource;
                    pressure[i].actual = ActualPressure;
                    pressure[i].storeHistory(ActualPressure);
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
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
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for ( int i = 0; i < MAX_TEMPERATURE_SOURCES; i++) {
                if ( temperatureExt[i].use(N2kMsg.Source, TempInstance, reuse)) {
                    temperatureExt[i].sourceSensor = TempSource;
                    temperatureExt[i].actual = ActualTemperature;
                    temperatureExt[i].set = SetTemperature;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

void DataCollector::DCBatteryStatus(const tN2kMsg &N2kMsg) {
    byte BatteryInstance;
    double BatteryVoltage;
    double BatteryCurrent;
    double BatteryTemperature;
    unsigned char SID;

    if (ParseN2kDCBatStatus(N2kMsg, BatteryInstance,BatteryVoltage,BatteryCurrent,BatteryTemperature,SID)) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for ( int i = 0; i < MAX_BATTERY_SOURCES; i++) {
                if ( dcBattery[i].use(N2kMsg.Source, BatteryInstance, reuse)) {
                    dcBattery[i].voltage = BatteryVoltage;
                    dcBattery[i].current = BatteryCurrent;
                    dcBattery[i].temperature = BatteryTemperature;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}


//*****************************************************************************
void DataCollector::Speed(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double STW;
    double SOG;
    tN2kSpeedWaterReferenceType SWRT;

    if (ParseN2kBoatSpeed(N2kMsg,SID,STW,SOG,SWRT) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_SPEED_SOURCES; i++) {
                if ( speed[i].use(N2kMsg.Source, i, reuse)) {
                    speed[i].stw = STW;
                    speed[i].sog = SOG;
                    speed[i].swrt = SWRT;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

//*****************************************************************************
void DataCollector::WaterDepth(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double DepthBelowTransducer;
    double Offset;

    if (ParseN2kWaterDepth(N2kMsg,SID,DepthBelowTransducer,Offset) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_WATER_DEPTH_SOURCES; i++) {
                if ( waterDepth[i].use(N2kMsg.Source, i, reuse)) {
                    if ( N2kIsNA(Offset) ) {
                        waterDepth[i].offset = 0;
                    } else {
                        waterDepth[i].offset = Offset;
                    }
                    waterDepth[i].depthBelowTransducer = DepthBelowTransducer;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

void DataCollector::Rudder(const tN2kMsg &N2kMsg) {
    unsigned char Instance;
    tN2kRudderDirectionOrder RudderDirectionOrder;
    double RudderPosition;
    double AngleOrder;
    
    if (ParseN2kRudder(N2kMsg,RudderPosition,Instance,RudderDirectionOrder,AngleOrder) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_RUDDER_SOURCES; i++) {
                if ( rudder[i].use(N2kMsg.Source, Instance, reuse)) {
                    rudder[i].position = RudderPosition;
                    rudder[i].directionOrder = RudderDirectionOrder;
                    rudder[i].angleOrder = AngleOrder;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

void DataCollector::Xte(const tN2kMsg &N2kMsg) {
    // 129283L
    unsigned char Instance;
    tN2kXTEMode xteMode;
    bool navTerminated;
    double xte;

    
    if (ParseN2kXTE(N2kMsg,Instance, xteMode, navTerminated, xte) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_XTE_SOURCES; i++) {
                if ( this->xte[i].use(N2kMsg.Source, Instance, reuse)) {
                    this->xte[i].xte = xte;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
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
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for ( int i = 0; i < MAX_VARIATION_SOURCES; i++) {
                if (this->variation[i].use(N2kMsg.Source, i, reuse)) {
                    this->variation[i].daysSince1970 = daysSince1970;
                    this->variation[i].variation = variation;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
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
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_WIND_SOURCES; i++) {
                if ( wind[i].use(N2kMsg.Source,Instance,reuse)) {
                    wind[i].windSpeed = windSpeed;
                    wind[i].windAngle = (windAngle>PI)?windAngle-2*PI:windAngle;
                    wind[i].windReference = windReference;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
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
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for ( int i = 0; i < MAX_LOG_SOURCES; i++) {
                if ( this->log[i].use(N2kMsg.Source, i, reuse)) {
                    this->log[i].daysSince1970 = daysSince1970;
                    this->log[i].secondsSinceMidnight = secondsSinceMidnight;
                    this->log[i].log = log;
                    this->log[i].tripLog = tripLog;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}

void DataCollector::LatLon(const tN2kMsg &N2kMsg) {
    // 129025L
    double latitude;
    double longitude;
    
 
    if (ParseN2kPositionRapid(N2kMsg,latitude, longitude) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_POSSITION_SOURCES; i++) {
                if ( possition[i].use(N2kMsg.Source, i, reuse)) {
                    possition[i].latitude = latitude;
                    possition[i].longitude = longitude;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}


bool DataCollector::getLatLong(double &latitude, double &longitude, int16_t &age) {
    PossitionData *possition = getPossition();
    GnssData *gnss = getGnss();
    unsigned long now = millis();

    if ( possition != NULL && gnss != NULL ) {
        if ( (now-possition->lastModified) < (now-gnss->lastModified) ) {
            latitude = possition->latitude;
            longitude = possition->longitude;
            age = (now - possition->lastModified)/1000;
            return true;
        } else {
            latitude = gnss->latitude;
            longitude = gnss->longitude;
            age = (now - gnss->lastModified)/1000;
            return true;
        }
    } else if (  possition != NULL ) {
        latitude = possition->latitude;
        longitude = possition->longitude;
        age = (now - possition->lastModified)/1000;
        return true;
    } else if ( gnss != NULL ) {
        latitude = gnss->latitude;
        longitude = gnss->longitude;
        age = (now - gnss->lastModified)/1000;
        return true;
    }
    return false;
}



void DataCollector::Leeway(const tN2kMsg &N2kMsg) {
    // 128000L
    unsigned char Instance;
    double leeway;


    if (ParseN2kLeeway(N2kMsg, Instance, leeway) ) {
        bool reuse = false;
        for (int u = 0; u < 2; u++) {
            for (int i = 0; i < MAX_LEEWAY_SOURCES; i++) {
                if ( this->leeway[i].use(N2kMsg.Source, Instance, reuse)) {
                    this->leeway[i].leeway = leeway;
                    return;
                }
            }
            reuse = true;
        }
        saveFailed(N2kMsg);
    } else {
        parseFailed(N2kMsg);
    }
}


LogData * DataCollector::getLog() {
    unsigned long lm = 0;
    LogData *selected = NULL;
    for (int i = 0; i < MAX_LOG_SOURCES; i++) {
        if (log[i].source != 255 && log[i].lastModified > lm ) {
            lm = log[i].lastModified;
            selected = &log[i];
        }
    }
    return selected;
};

WindData * DataCollector::getWindInstance(byte instance, tN2kWindReference reference) {
    unsigned long lm = 0;
    WindData *selected = NULL;
    for (int i = -0; i < MAX_WIND_SOURCES; i++) {
        if ( wind[i].source != 255 && wind[i].instance == instance && wind[i].windReference == reference && wind[i].lastModified > lm ) {
            lm = wind[i].lastModified;
            selected = &wind[i];
            return &wind[i];
        }
    }
    return selected;
};


EngineData * DataCollector::getEngineInstance(byte instance) {
    unsigned long lm = 0;
    EngineData *selected = NULL;
    for (int i = 0; i < MAX_ENGINE_SOURCES; i++) {
        if ( engine[i].source != 255 && engine[i].instance == instance && engine[i].lastModified > lm ) {
            lm = engine[i].lastModified;
            selected = &engine[i];
        }
    }
    return selected;
};
FluidLevelData * DataCollector::getFluidLevelInstance(byte instance) {
    unsigned long lm = 0;
    FluidLevelData *selected = NULL;
    for (int i = -0; i < MAX_FLUID_LEVEL_SOURCES; i++) {
        if ( fluidLevel[i].source != 255 && fluidLevel[i].instance == instance &&  fluidLevel[i].lastModified > lm) {
            lm = fluidLevel[i].lastModified;
            selected = &fluidLevel[i];
        }
    }
    return selected;
};

PossitionData * DataCollector::getPossition() {
    unsigned long lm = 0;
    PossitionData *selected = NULL;
    for (int i = 0; i < MAX_POSSITION_SOURCES; i++) {
        if (possition[i].source != 255  && possition[i].lastModified > lm ) {
            lm = possition[i].lastModified;
            selected = &possition[i];
        }
    }
    return selected;
};

GnssData * DataCollector::getGnss() {
    unsigned long lm = 0;
    GnssData *selected = NULL;
    for (int i = 0; i < MAX_GNSS_SOURCES; i++) {
        if (gnss[i].source != 255 && gnss[i].lastModified > lm ) {
            lm = gnss[i].lastModified;
            selected = &gnss[i];
        }
    }
    return selected;
};

CogSogData * DataCollector::getCogSog() {
    unsigned long lm = 0;
    CogSogData *selected = NULL;
    for (int i = 0; i < MAX_COGSOG_SOURCES; i++) {
        if (cogSog[i].source != 255 && cogSog[i].lastModified > lm ) {
            lm = cogSog[i].lastModified;
            selected = &cogSog[i];
        }
    }
    return selected;
};


void EngineDataOutput::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",true);
    startArray("engine");
    for (int i=0; i<MAX_ENGINE_SOURCES; i++) {
        EngineData *engine =  &dataCollector.engine[i];
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
        DcBatteryData *battery =  &dataCollector.dcBattery[i];
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
        FluidLevelData *fluidLevel =  &dataCollector.fluidLevel[i];
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
    for (int i=0; i<MAX_SPEED_SOURCES; i++) {
        HeadingData *heading =  &dataCollector.heading[i];
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
        SpeedData *speed =  &dataCollector.speed[i];
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
        WaterDepthData *waterDepth =  &dataCollector.waterDepth[i];
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
        RudderData *rudder =  &dataCollector.rudder[i];
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
        AttitudeData *attitude =  &dataCollector.attitude[i];
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
        CogSogData *cogSog =  &dataCollector.cogSog[i];
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
        GnssData *gnss =  &dataCollector.gnss[i];
        startObject();
        append("id",i);
        append("source",gnss->source);
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
        OutsideEnvironmentData *outsideEnvironmental =  &dataCollector.outsideEnvironmental[i];
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
        EnvironmentalParamsData *environmentParams =  &dataCollector.environmentalParams[i];
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
        HumidityData *humidity =  &dataCollector.humidity[i];
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
        PressureData *pressure =  &dataCollector.pressure[i];
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
        TemperatureData *temperature =  &dataCollector.temperature[i];
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
        TemperatureData *temperatureExt =  &dataCollector.temperatureExt[i];
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
        XteData *xte =  &dataCollector.xte[i];
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
        VariationData *variation = &dataCollector.variation[i];
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
        WindData *wind = &dataCollector.wind[i];
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
        LogData *log = &dataCollector.log[i];
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
        PossitionData *possition = &dataCollector.possition[i];
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
        LeewayData *leeway = &dataCollector.leeway[i];
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



bool EngineDataOutput::drawPage(Adafruit_SSD1306 * display) {
    
    EngineData *engine = dataCollector.getEngineInstance(0);
    FluidLevelData *fluidLevel = dataCollector.getFluidLevelInstance(0);

    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    switch(subPage) {
        case 0:
            display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
            display->setCursor(0,0);
            if ( engine == NULL) {
                display->printf(" ---- RPM\n"); // RPM
                display->printf("  --- C\n"); // Coolant temperatire
                display->printf("--.-- V a\n"); // Alternator Voltage
                display->printf("---.- h\n"); // Engine Hours
            }  else {
                //  1234 RPM
                //   123 C
                // 14.23
                display->printf(" %4.0f RPM\n",engine->speed); // RPM
                display->printf("  %3.0f C\n", engine->CoolantTemp-273.0); // Coolant temperatire
                display->printf("%5.2f V\n", engine->AltenatorVoltage); // Alternator Voltage
                display->printf("%5.1f h\n", engine->Hours); // Engine Hours
            }
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
            display->setCursor(0,0);
            if ( fluidLevel == NULL) {
                display->printf("Fuel\n--- %%\n"); // Fuel %
            } else {
                display->printf("Fuel\n%3.0f %%\n",fluidLevel->level); // Fuel %
            }
            display->display();
            subPage = 0;
            return true;
        default:
            subPage = 0;
            return false;
    }
#endif

}

bool NavigationDataOutput::drawPage(Adafruit_SSD1306 * display) {
    CogSogData *cogSog =  dataCollector.getCogSog();



    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
    if ( cogSog == NULL ) {
        display->setCursor(0,12);              
        display->printf("COG: -\n"); 
        display->setCursor(0,36);             
        display->printf("SOG: -\n"); 
    } else {
        display->setCursor(0,12);              
        display->printf("COG:%3.0f\n", RadToDeg(cogSog->cog)); 
        display->setCursor(0,36);             
        display->printf("SOG:%5.1f\n", msToKnots(cogSog->sog)); 
    }
    display->display();
    return true;
#endif

}


bool WindSpeedDataOutput::drawPage(Adafruit_SSD1306 * display) {
    WindData *wind = dataCollector.getWindInstance(0);


    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
    if ( wind == NULL ) {
        display->setCursor(0,12);              
        display->printf("AWS: -\n"); // AWS
        display->setCursor(0,36);             
        display->printf("AWA: -\n"); // AWA
    } else {
        display->setCursor(0,12);              
        display->printf("AWS:%5.1f\n", msToKnots(wind->windSpeed)); // AWS
        display->setCursor(0,36);             
        if ( wind->windAngle < 0 ) {
            display->printf("AWA:%3.0fP\n", RadToDeg(wind->windAngle)); // AWA
        }  else {
            display->printf("AWA:%3.0fS\n", RadToDeg(wind->windAngle)); // Wind
        }
    }
    display->display();
    return true;
#endif

}

bool EnvironmentDataOutput::drawPage(Adafruit_SSD1306 * display) {
    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
    PressureData *pressure = &dataCollector.pressure[0];
    TemperatureData *temperatureExt = &dataCollector.temperatureExt[0];
    HumidityData *humidity = &dataCollector.humidity[0];

#if OLED_HEIGHT == 32
    switch(subPage) {
        case 0:
            if ( pressure->source != 255 && pressure->lastModified > 0 ) {
                display->setTextSize(2);   // 18x24  128/24=7 chars 5,3 
                display->printf("%6.1f mB\n", PascalTomBar(pressure->actual));
                display->printf("%4.1f C\n", KelvinToC(temperatureExt->actual)); 
                display->printf("%5.1f %%RH\n", humidity->actual ); 
                display->display();
                subPage = 1;
                return false;
            } else {
                display->setTextSize(2);   // 18x24  128/24=7 chars 5,3 
                display->printf("----.- mB\n");
                display->printf("--.- C\n"); 
                display->printf("---.- %%RH\n" ); 
                display->display();
                subPage = 0;
                return true;
            }
        case 1:
            if ( pressure->drawHistory(display,false,970.0,1030.0) ) {
                display->display();
                subPage = 0;
                return true;
            } else {
                display->display();
                return false;
            }
        default:
            subPage = 0;
            return false;
    }
#else
    switch(subPage) {
        case 0:

            display->setTextSize(2);   // 18x24  128/24=7 chars 5,3 
            display->setCursor(0,4);  
            display->printf("%6.1f mB\n", PascalTomBar(pressure->actual));
            display->printf("%4.1f C\n", KelvinToC(temperatureExt->actual)); 
            display->printf("%5.1f %%RH\n", humidity->actual ); 
            display->display();
            subPage = 1;
            return false;
        case 1:
            if ( pressure->drawHistory(display,false,970.0,1030.0) ) {
                display->display();
                subPage = 0;
                return true;
            } else {
                display->display();
                return false;
            }
        default:
            subPage = 0;
            return false;

    }
#endif
}


bool LogDataOutput::drawPage(Adafruit_SSD1306 * display) {
    LogData *log = dataCollector.getLog();
    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
    display->setCursor(0,12);   
    if ( log == NULL) {
        display->printf("L:----.-Nm\n"); // Log
        display->setCursor(0,36);             
        display->printf("T:----.-Nm\n"); // Trip
    } else {
        display->printf("L:%6.1fNm\n", log->log/1852.0); // Log
        display->setCursor(0,36);             
        display->printf("T:%6.1fNm\n", log->tripLog/1852.0); // Trip
    }
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
    double latitude,longitude;
    int16_t age;
    if (!dataCollector.getLatLong(latitude, longitude, age)) {
        display->printf(" --%c--.--N\n", 248); 
        display->printf("---%c--.--E\n", 248); 
    } else {
        int deg = (int) latitude;
        double min = (double) 60.0*(latitude-deg);
        if ( deg >= 0 && min >= 0) {
            display->printf(" %02d%c%05.2fN\n", deg, 248, min); 
        } else {
            display->printf(" %02d%c%05.2fS\n", -deg, 248, -min); 
        }
        deg = (int) longitude;
        min = (double) 60.0*(longitude-deg);
        display->setCursor(0,36);             
        if ( deg >= 0 && min >= 0) {
            display->printf("%03d%c%05.2fE\n", deg,248,min); 
        } else {
            display->printf("%03d%c%05.2fW\n", -deg,248,-min); 
        }
        display->setTextSize(1);   // 12x16, 4 rows, 10.6 chars
        if ( age < 600 ) {
            display->printf("%ds\n", age); 
        } else if (age < 7200 ) {
            display->printf("%dm\n", age/60); 
        } else {
            display->printf("%dn\n", age/3600); 
        }
    }
    display->cp437(false);         
    display->display();
    return true;
#endif

}


