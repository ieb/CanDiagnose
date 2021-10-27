

#include "datadisplay.h"
#include <N2kMessagesEnumToStr.h>


DataDisplay::DataDisplay(tNMEA2000 *_pNMEA2000, Stream *outputStream) : 
        tNMEA2000::tMsgHandler(0,_pNMEA2000),
        OutputStream{outputStream} {
}

void DataDisplay::HandleMsg(const tN2kMsg &N2kMsg) {
  if ( showData ) {
      switch(N2kMsg.PGN) {
        case 60928L: AddressClaim(N2kMsg); break;
        case 126992L: SystemTime(N2kMsg); break;
        case 127245L: Rudder(N2kMsg); break;
        case 127250L: Heading(N2kMsg); break;
        case 127257L: Attitude(N2kMsg); break;
        case 127488L: EngineRapid(N2kMsg); break;
        case 127489L: EngineDynamicParameters(N2kMsg); break;
        case 127493L: TransmissionParameters(N2kMsg); break;
        case 127497L: TripFuelConsumption(N2kMsg); break;
        case 127501L: BinaryStatus(N2kMsg); break;
        case 127505L: FluidLevel(N2kMsg); break;
        case 127506L: DCStatus(N2kMsg); break;
        case 127508L: DCBatteryStatus(N2kMsg); break;
        case 127513L: BatteryConfigurationStatus(N2kMsg); break;
        case 128259L: Speed(N2kMsg); break;
        case 128267L: WaterDepth(N2kMsg); break;
        case 129026L: COGSOG(N2kMsg); break;
        case 129029L: GNSS(N2kMsg); break;
        case 129033L: LocalOffset(N2kMsg); break;
        case 129045L: UserDatumSettings(N2kMsg); break;
        case 129540L: GNSSSatsInView(N2kMsg); break;
        case 130310L: OutsideEnvironmental(N2kMsg); break;
        case 130312L: Temperature(N2kMsg); break;
        case 130313L: Humidity(N2kMsg); break;
        case 130314L: Pressure(N2kMsg); break;
        case 130316L: TemperatureExt(N2kMsg); break;
      }
  }
}


//*****************************************************************************
void DataDisplay::SystemTime(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    uint16_t SystemDate;
    double SystemTime;
    tN2kTimeSource TimeSource;
    
    if (ParseN2kSystemTime(N2kMsg,SID,SystemDate,SystemTime,TimeSource) ) {
                      OutputStream->println("System time:");
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  SID: ",SID,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  days since 1.1.1970: ",SystemDate,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  seconds since midnight: ",SystemTime,0,true);
                        OutputStream->print("  time source: "); PrintN2kEnumType(TimeSource,OutputStream);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::Rudder(const tN2kMsg &N2kMsg) {
    unsigned char Instance;
    tN2kRudderDirectionOrder RudderDirectionOrder;
    double RudderPosition;
    double AngleOrder;
    
    if (ParseN2kRudder(N2kMsg,RudderPosition,Instance,RudderDirectionOrder,AngleOrder) ) {
      PrintLabelValWithConversionCheckUnDef(OutputStream, "Rudder: ",Instance,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  position (deg): ",RudderPosition,&RadToDeg,true);
                        OutputStream->print("  direction order: "); PrintN2kEnumType(RudderDirectionOrder,OutputStream);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  angle order (deg): ",AngleOrder,&RadToDeg,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::EngineRapid(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    double EngineSpeed;
    double EngineBoostPressure;
    int8_t EngineTiltTrim;
    
    if (ParseN2kEngineParamRapid(N2kMsg,EngineInstance,EngineSpeed,EngineBoostPressure,EngineTiltTrim) ) {
      PrintLabelValWithConversionCheckUnDef(OutputStream, "Engine rapid params: ",EngineInstance,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  RPM: ",EngineSpeed,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  boost pressure (Pa): ",EngineBoostPressure,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  tilt trim: ",EngineTiltTrim,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::EngineDynamicParameters(const tN2kMsg &N2kMsg) {
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
      PrintLabelValWithConversionCheckUnDef(OutputStream, "Engine dynamic params: ",EngineInstance,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  oil pressure (Pa): ",EngineOilPress,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  oil temp (C): ",EngineOilTemp,&KelvinToC,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  coolant temp (C): ",EngineCoolantTemp,&KelvinToC,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  altenator voltage (V): ",AltenatorVoltage,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  fuel rate (l/h): ",FuelRate,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  engine hours (h): ",EngineHours,&SecondsToh,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  coolant pressure (Pa): ",EngineCoolantPress,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  fuel pressure (Pa): ",EngineFuelPress,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  engine load (%): ",EngineLoad,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  engine torque (%): ",EngineTorque,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::TransmissionParameters(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    tN2kTransmissionGear TransmissionGear;
    double OilPressure;
    double OilTemperature;
    unsigned char DiscreteStatus1;
    
    if (ParseN2kTransmissionParameters(N2kMsg,EngineInstance, TransmissionGear, OilPressure, OilTemperature, DiscreteStatus1) ) {
      PrintLabelValWithConversionCheckUnDef(OutputStream, "Transmission params: ",EngineInstance,0,true);
                        OutputStream->print("  gear: "); PrintN2kEnumType(TransmissionGear,OutputStream);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  oil pressure (Pa): ",OilPressure,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  oil temperature (C): ",OilTemperature,&KelvinToC,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  discrete status: ",DiscreteStatus1,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::TripFuelConsumption(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    double TripFuelUsed;
    double FuelRateAverage; 
    double FuelRateEconomy; 
    double InstantaneousFuelEconomy; 
    
    if (ParseN2kEngineTripParameters(N2kMsg,EngineInstance, TripFuelUsed, FuelRateAverage, FuelRateEconomy, InstantaneousFuelEconomy) ) {
      PrintLabelValWithConversionCheckUnDef(OutputStream, "Trip fuel consumption: ",EngineInstance,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  fuel used (l): ",TripFuelUsed,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  average fuel rate (l/h): ",FuelRateAverage,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  economy fuel rate (l/h): ",FuelRateEconomy,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  instantaneous fuel economy (l/h): ",InstantaneousFuelEconomy,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::Heading(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double Heading;
    double Deviation;
    double Variation;
    
    if (ParseN2kHeading(N2kMsg,SID,Heading,Deviation,Variation,HeadingReference) ) {
                      OutputStream->println("Heading:");
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  SID: ",SID,0,true);
                        OutputStream->print("  reference: "); PrintN2kEnumType(HeadingReference,OutputStream);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  Heading (deg): ",Heading,&RadToDeg,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  Deviation (deg): ",Deviation,&RadToDeg,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  Variation (deg): ",Variation,&RadToDeg,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::COGSOG(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double COG;
    double SOG;
    
    if (ParseN2kCOGSOGRapid(N2kMsg,SID,HeadingReference,COG,SOG) ) {
                      OutputStream->println("COG/SOG:");
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  SID: ",SID,0,true);
                        OutputStream->print("  reference: "); PrintN2kEnumType(HeadingReference,OutputStream);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  COG (deg): ",COG,&RadToDeg,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  SOG (m/s): ",SOG,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::GNSS(const tN2kMsg &N2kMsg) {
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
                      OutputStream->println("GNSS info:");
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  SID: ",SID,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  days since 1.1.1970: ",DaysSince1970,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  seconds since midnight: ",SecondsSinceMidnight,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  latitude: ",Latitude,0,true,9);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  longitude: ",Longitude,0,true,9);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  altitude: (m): ",Altitude,0,true);
                        OutputStream->print("  GNSS type: "); PrintN2kEnumType(GNSStype,OutputStream);
                        OutputStream->print("  GNSS method: "); PrintN2kEnumType(GNSSmethod,OutputStream);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  satellite count: ",nSatellites,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  HDOP: ",HDOP,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  PDOP: ",PDOP,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  geoidal separation: ",GeoidalSeparation,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  reference stations: ",nReferenceStations,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::UserDatumSettings(const tN2kMsg &N2kMsg) {
  if (N2kMsg.PGN!=129045L) return;
  int Index=0;
  double val;

  OutputStream->println("User Datum Settings: ");
  val=N2kMsg.Get4ByteDouble(1e-2,Index);
  PrintLabelValWithConversionCheckUnDef(OutputStream, "  delta x (m): ",val,0,true);
  val=N2kMsg.Get4ByteDouble(1e-2,Index);
  PrintLabelValWithConversionCheckUnDef(OutputStream, "  delta y (m): ",val,0,true);
  val=N2kMsg.Get4ByteDouble(1e-2,Index);
  PrintLabelValWithConversionCheckUnDef(OutputStream, "  delta z (m): ",val,0,true);
  val=N2kMsg.GetFloat(Index);
  PrintLabelValWithConversionCheckUnDef(OutputStream, "  rotation in x (deg): ",val,&RadToDeg,true,5);
  val=N2kMsg.GetFloat(Index);
  PrintLabelValWithConversionCheckUnDef(OutputStream, "  rotation in y (deg): ",val,&RadToDeg,true,5);
  val=N2kMsg.GetFloat(Index);
  PrintLabelValWithConversionCheckUnDef(OutputStream, "  rotation in z (deg): ",val,&RadToDeg,true,5);
  val=N2kMsg.GetFloat(Index);
  PrintLabelValWithConversionCheckUnDef(OutputStream, "  scale: ",val,0,true,3);
}

//*****************************************************************************
void DataDisplay::GNSSSatsInView(const tN2kMsg &N2kMsg) {
  unsigned char SID;
  tN2kRangeResidualMode Mode;
  uint8_t NumberOfSVs;
  tSatelliteInfo SatelliteInfo;

  if (ParseN2kPGNSatellitesInView(N2kMsg,SID,Mode,NumberOfSVs) ) {
    OutputStream->println("Satellites in view: ");
                      OutputStream->print("  mode: "); OutputStream->println(Mode);
                      OutputStream->print("  number of satellites: ");  OutputStream->println(NumberOfSVs);
    for ( uint8_t i=0; i<NumberOfSVs && ParseN2kPGNSatellitesInView(N2kMsg,i,SatelliteInfo); i++) {
                        OutputStream->print("  Satellite PRN: ");  OutputStream->println(SatelliteInfo.PRN);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "    elevation: ",SatelliteInfo.Elevation,&RadToDeg,true,1);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "    azimuth:   ",SatelliteInfo.Azimuth,&RadToDeg,true,1);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "    SNR:       ",SatelliteInfo.SNR,0,true,1);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "    residuals: ",SatelliteInfo.RangeResiduals,0,true,1);
                        OutputStream->print("    status: "); OutputStream->println(SatelliteInfo.UsageStatus);
    }
  }
}

//*****************************************************************************
void DataDisplay::LocalOffset(const tN2kMsg &N2kMsg) {
    uint16_t SystemDate;
    double SystemTime;
    int16_t Offset;
    
    if (ParseN2kLocalOffset(N2kMsg,SystemDate,SystemTime,Offset) ) {
                      OutputStream->println("Date,time and local offset: ");
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  days since 1.1.1970: ",SystemDate,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  seconds since midnight: ",SystemTime,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  local offset (min): ",Offset,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::OutsideEnvironmental(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double WaterTemperature;
    double OutsideAmbientAirTemperature;
    double AtmosphericPressure;
    
    if (ParseN2kOutsideEnvironmentalParameters(N2kMsg,SID,WaterTemperature,OutsideAmbientAirTemperature,AtmosphericPressure) ) {
      PrintLabelValWithConversionCheckUnDef(OutputStream, "Water temp: ",WaterTemperature,&KelvinToC);
      PrintLabelValWithConversionCheckUnDef(OutputStream, ", outside ambient temp: ",OutsideAmbientAirTemperature,&KelvinToC);
      PrintLabelValWithConversionCheckUnDef(OutputStream, ", pressure: ",AtmosphericPressure,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::Temperature(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;
    
    if (ParseN2kTemperature(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature) ) {
                        OutputStream->print("Temperature source: "); PrintN2kEnumType(TempSource,OutputStream,false);
      PrintLabelValWithConversionCheckUnDef(OutputStream, ", actual temperature: ",ActualTemperature,&KelvinToC);
      PrintLabelValWithConversionCheckUnDef(OutputStream, ", set temperature: ",SetTemperature,&KelvinToC,true);
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::Humidity(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char Instance;
    tN2kHumiditySource HumiditySource;
    double ActualHumidity,SetHumidity;
    
    if ( ParseN2kHumidity(N2kMsg,SID,Instance,HumiditySource,ActualHumidity,SetHumidity) ) {
                        OutputStream->print("Humidity source: "); PrintN2kEnumType(HumiditySource,OutputStream,false);
      PrintLabelValWithConversionCheckUnDef(OutputStream, ", humidity: ",ActualHumidity,0,false);
      PrintLabelValWithConversionCheckUnDef(OutputStream, ", set humidity: ",SetHumidity,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::Pressure(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char Instance;
    tN2kPressureSource PressureSource;
    double ActualPressure;
    
    if ( ParseN2kPressure(N2kMsg,SID,Instance,PressureSource,ActualPressure) ) {
                        OutputStream->print("Pressure source: "); PrintN2kEnumType(PressureSource,OutputStream,false);
      PrintLabelValWithConversionCheckUnDef(OutputStream, ", pressure: ",ActualPressure,&PascalTomBar,true);
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::TemperatureExt(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;
    
    if (ParseN2kTemperatureExt(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature) ) {
                        OutputStream->print("Temperature source: "); PrintN2kEnumType(TempSource,OutputStream,false);
      PrintLabelValWithConversionCheckUnDef(OutputStream, ", actual temperature: ",ActualTemperature,&KelvinToC);
      PrintLabelValWithConversionCheckUnDef(OutputStream, ", set temperature: ",SetTemperature,&KelvinToC,true);
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::BatteryConfigurationStatus(const tN2kMsg &N2kMsg) {
    unsigned char BatInstance;
    tN2kBatType BatType;
    tN2kBatEqSupport SupportsEqual;
    tN2kBatNomVolt BatNominalVoltage;
    tN2kBatChem BatChemistry;
    double BatCapacity;
    int8_t BatTemperatureCoefficient;
    double PeukertExponent; 
    int8_t ChargeEfficiencyFactor;

    if (ParseN2kBatConf(N2kMsg,BatInstance,BatType,SupportsEqual,BatNominalVoltage,BatChemistry,BatCapacity,BatTemperatureCoefficient,PeukertExponent,ChargeEfficiencyFactor) ) {
      PrintLabelValWithConversionCheckUnDef(OutputStream, "Battery instance: ",BatInstance,0,true);
                        OutputStream->print("  - type: "); PrintN2kEnumType(BatType,OutputStream);
                        OutputStream->print("  - support equal.: "); PrintN2kEnumType(SupportsEqual,OutputStream);
                        OutputStream->print("  - nominal voltage: "); PrintN2kEnumType(BatNominalVoltage,OutputStream);
                        OutputStream->print("  - chemistry: "); PrintN2kEnumType(BatChemistry,OutputStream);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  - capacity (Ah): ",BatCapacity,&CoulombToAh,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  - temperature coefficient (%): ",BatTemperatureCoefficient,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  - peukert exponent: ",PeukertExponent,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  - charge efficiency factor (%): ",ChargeEfficiencyFactor,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void DataDisplay::DCStatus(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    unsigned char DCInstance;
    tN2kDCType DCType;
    unsigned char StateOfCharge;
    unsigned char StateOfHealth;
    double TimeRemaining;
    double RippleVoltage;
    double Capacity;

    if (ParseN2kDCStatus(N2kMsg,SID,DCInstance,DCType,StateOfCharge,StateOfHealth,TimeRemaining,RippleVoltage,Capacity) ) {
      OutputStream->print("DC instance: ");
      OutputStream->println(DCInstance);
      OutputStream->print("  - type: "); PrintN2kEnumType(DCType,OutputStream);
      OutputStream->print("  - state of charge (%): "); OutputStream->println(StateOfCharge);
      OutputStream->print("  - state of health (%): "); OutputStream->println(StateOfHealth);
      OutputStream->print("  - time remaining (h): "); OutputStream->println(TimeRemaining/60);
      OutputStream->print("  - ripple voltage: "); OutputStream->println(RippleVoltage);
      OutputStream->print("  - capacity: "); OutputStream->println(Capacity);
    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}

void DataDisplay::DCBatteryStatus(const tN2kMsg &N2kMsg) {
    byte BatteryInstance;
    double BatteryVoltage;
    double BatteryCurrent;
    double BatteryTemperature;
    unsigned char SID;

    if (ParseN2kDCBatStatus(N2kMsg, BatteryInstance,BatteryVoltage,BatteryCurrent,BatteryTemperature,SID)) {
      OutputStream->print("Battery instance: ");
      OutputStream->println(BatteryInstance);
      OutputStream->print("  - Voltage (V): "); OutputStream->println(BatteryVoltage);
      OutputStream->print("  - Current (A): "); OutputStream->println(BatteryCurrent);
      OutputStream->print("  - Temperature (K): "); OutputStream->println(BatteryTemperature);

    } else {
      OutputStream->print("Failed to parse PGN: ");  OutputStream->println(N2kMsg.PGN);
    }
}


//*****************************************************************************
void DataDisplay::Speed(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double SOW;
    double SOG;
    tN2kSpeedWaterReferenceType SWRT;

    if (ParseN2kBoatSpeed(N2kMsg,SID,SOW,SOG,SWRT) ) {
      OutputStream->print("Boat speed:");
      PrintLabelValWithConversionCheckUnDef(OutputStream, " SOW:",N2kIsNA(SOW)?SOW:msToKnots(SOW));
      PrintLabelValWithConversionCheckUnDef(OutputStream, ", SOG:",N2kIsNA(SOG)?SOG:msToKnots(SOG));
      OutputStream->print(", ");
      PrintN2kEnumType(SWRT,OutputStream,true);
    }
}

//*****************************************************************************
void DataDisplay::WaterDepth(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double DepthBelowTransducer;
    double Offset;

    if (ParseN2kWaterDepth(N2kMsg,SID,DepthBelowTransducer,Offset) ) {
      if ( N2kIsNA(Offset) || Offset == 0 ) {
        PrintLabelValWithConversionCheckUnDef(OutputStream, "Depth below transducer",DepthBelowTransducer);
        if ( N2kIsNA(Offset) ) {
          OutputStream->println(", offset not available");
        } else {
          OutputStream->println(", offset=0");
        }
      } else {
        if (Offset>0) {
          OutputStream->print("Water depth:");
        } else {
          OutputStream->print("Depth below keel:");
        }
        if ( !N2kIsNA(DepthBelowTransducer) ) { 
          OutputStream->println(DepthBelowTransducer+Offset); 
        } else {  OutputStream->println(" not available"); }
      }
    }
}

//*****************************************************************************
void DataDisplay::printLLNumber(Stream *OutputStream, unsigned long long n, uint8_t base)
{
  unsigned char buf[16 * sizeof(long)]; // Assumes 8-bit chars.
  unsigned long long i = 0;

  if (n == 0) {
    OutputStream->print('0');
    return;
  }

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    OutputStream->print((char) (buf[i - 1] < 10 ?
      '0' + buf[i - 1] :
      'A' + buf[i - 1] - 10));
}

//*****************************************************************************
void DataDisplay::BinaryStatusFull(const tN2kMsg &N2kMsg) {
    unsigned char BankInstance;
    tN2kBinaryStatus BankStatus;

    if (ParseN2kBinaryStatus(N2kMsg,BankInstance,BankStatus) ) {
      OutputStream->print("Binary status for bank "); OutputStream->print(BankInstance); OutputStream->println(":");
      OutputStream->print("  "); //printLLNumber(OutputStream,BankStatus,16);
      for (uint8_t i=1; i<=28; i++) {
        if (i>1) OutputStream->print(",");
        PrintN2kEnumType(N2kGetStatusOnBinaryStatus(BankStatus,i),OutputStream,false);
      }
      OutputStream->println();
    }
}

//*****************************************************************************
void DataDisplay::BinaryStatus(const tN2kMsg &N2kMsg) {
    unsigned char BankInstance;
    tN2kOnOff Status1,Status2,Status3,Status4;

    if (ParseN2kBinaryStatus(N2kMsg,BankInstance,Status1,Status2,Status3,Status4) ) {
      if (BankInstance>2) { // note that this is only for testing different methods. MessageSender.ini sends 4 status for instace 2
        BinaryStatusFull(N2kMsg);
      } else {
        OutputStream->print("Binary status for bank "); OutputStream->print(BankInstance); OutputStream->println(":");
        OutputStream->print("  Status1=");PrintN2kEnumType(Status1,OutputStream,false);
        OutputStream->print(", Status2=");PrintN2kEnumType(Status2,OutputStream,false);
        OutputStream->print(", Status3=");PrintN2kEnumType(Status3,OutputStream,false);
        OutputStream->print(", Status4=");PrintN2kEnumType(Status4,OutputStream,false);
        OutputStream->println();
      }
    }
}

//*****************************************************************************
void DataDisplay::FluidLevel(const tN2kMsg &N2kMsg) {
    unsigned char Instance;
    tN2kFluidType FluidType;
    double Level=0;
    double Capacity=0;

    if (ParseN2kFluidLevel(N2kMsg,Instance,FluidType,Level,Capacity) ) {
      switch (FluidType) {
        case N2kft_Fuel:
          OutputStream->print("Fuel level :");
          break;
        case N2kft_Water:
          OutputStream->print("Water level :");
          break;
        case N2kft_GrayWater:
          OutputStream->print("Gray water level :");
          break;
        case N2kft_LiveWell:
          OutputStream->print("Live well level :");
          break;
        case N2kft_Oil:
          OutputStream->print("Oil level :");
          break;
        case N2kft_BlackWater:
          OutputStream->print("Black water level :");
          break;
        case N2kft_FuelGasoline:
          OutputStream->print("Gasoline level :");
          break;
        case N2kft_Error:
          OutputStream->print("Error level :");
          break;
        case N2kft_Unavailable:
          OutputStream->print("Unknown level :");
          break;
      }
      OutputStream->print(Level); OutputStream->print("%"); 
      OutputStream->print(" ("); OutputStream->print(Capacity*Level/100); OutputStream->print("l)");
      OutputStream->print(" capacity :"); OutputStream->println(Capacity);
    }
}

//*****************************************************************************
void DataDisplay::Attitude(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double Yaw;
    double Pitch;
    double Roll;
    
    if (ParseN2kAttitude(N2kMsg,SID,Yaw,Pitch,Roll) ) {
                      OutputStream->println("Attitude:");
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  SID: ",SID,0,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  Yaw (deg): ",Yaw,&RadToDeg,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  Pitch (deg): ",Pitch,&RadToDeg,true);
      PrintLabelValWithConversionCheckUnDef(OutputStream, "  Roll (deg): ",Roll,&RadToDeg,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

void DataDisplay::AddressClaim(const tN2kMsg &N2kMsg) {
    //PGN 60928
    int index = 0;
    uint64_t name = N2kMsg.GetUInt64(index);
    OutputStream->print("Address Claim: address:");
    OutputStream->print(N2kMsg.Source);
    OutputStream->print(" name:");
    OutputStream->println(name);
    
}

