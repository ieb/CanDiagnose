#ifndef DATAOUTPUT_H
#define DATAOUTPUT_H

#include <NMEA2000.h>
#include <N2kMessages.h>

#define MAX_HEADDING_SOURCES 5
#define MAX_ENGINE_SOURCES 2
#define MAX_COGSOG_SOURCES 2
#define MAX_GNSS_SOURCES 2
#define MAX_OUTSIDE_ENVIRONMENTAL_SOURCES 2
#define MAX_TEMPERATURE_SOURCES 5
#define MAX_HUMIDITY_SOURCES 1
#define MAX_PRESSURE_SOURCES 1
#define MAX_BATTERY_SOURCES 4
#define MAX_SPEED_SOURCES 3
#define MAX_WATER_DEPTH_SOURCES 2
#define MAX_RUDDER_SOURCES 1
#define MAX_ATTITUDE_SOURCES 2
#define MAX_FLUID_LEVEL_SOURCES 2


typedef struct AttitudeData {
    unsigned long lastModified=0;
    double roll;
    double pitch;
    double yaw;
} AttitudeData;

typedef struct FluidLevelData {
    unsigned long lastModified=0;
    tN2kFluidType fluidType;
    double level;
    double capacity;
} FluidLevelData;


typedef struct RudderData {
    unsigned long lastModified=0;
    tN2kRudderDirectionOrder directionOrder;
    double position;
    double angleOrder;
} RudderData;


typedef struct WaterDepthData {
    unsigned long lastModified=0;
  double offset;
  double depthBelowTransducer; 
} WaterDepthData;

typedef struct SpeedData {
    unsigned long lastModified=0;
    double sow;
    double sog;
    tN2kSpeedWaterReferenceType swrt;
} SpeedData;


typedef struct DcBatteryData {
    unsigned long lastModified=0;
    byte instance;
    double voltage;
    double current;
    double temperature;
} DcBatteryData;




typedef struct PressureData {
    unsigned long lastModified=0;
    unsigned char instance;
    tN2kPressureSource source;
    double actual;
} PressureData;

typedef struct HumidityData {
    unsigned long lastModified=0;
    unsigned char instance;
    tN2kHumiditySource source;
    double actual;
    double set;
} HumidityData;

typedef struct TemperatureData {
    unsigned long lastModified=0;
    unsigned char instance;
    tN2kTempSource source;
    double actual;
    double set;
} TemperatureData;


typedef struct OutsideEnvironmentData {
    unsigned long lastModified=0;
  double waterTemperature;
  double outsideAmbientAirTemperature;
  double atmosphericPressure;
} OutsideEnvironmentData;

typedef struct GnssData {
    unsigned long lastModified=0;
    uint16_t daysSince1970;
    double secondsSinceMidnight; 
    double latitude;
    double longitude;
    double altitude; 
    tN2kGNSStype type;
    tN2kGNSSmethod method;
    unsigned char nSatellites;
    double HDOP;
    double PDOP;
    double geoidalSeparation;
    unsigned char nReferenceStations;
    tN2kGNSStype referenceStationType;
    uint16_t referenceStationID;
    double ageOfCorrection;
} GnssData;

typedef struct CogSogData {
    unsigned long lastModified=0;
    tN2kHeadingReference reference;
    double cog;
    double sog;
} CogSogData;

typedef struct HeadingData {
    unsigned long lastModified=0;
    double heading;
    double deviation;
    double variation;
    tN2kHeadingReference reference;
} HeadingData;

typedef struct EngineData {
    unsigned long lastModified=0;
        double speed;
        double boostPressure;
        double tiltTrim;
        double OilPress;
        double OilTemp;
        double CoolantTemp;
        double AltenatorVoltage;
        double FuelRate;
        double Hours;
        double CoolantPress;
        double FuelPress; 
        int8_t Load;
        int8_t Torque;
        uint16_t Status1;
        uint16_t Status2;
} EngineData;



class DataOutput{
    public:
        DataOutput(Stream *outputStream);
        void HandleMsg(const tN2kMsg &N2kMsg);
        void outputPackets(int packetId);
        bool showData= false;
    private:
        void EngineRapid(const tN2kMsg &N2kMsg);
        void EngineDynamicParameters(const tN2kMsg &N2kMsg);
        void FluidLevel(const tN2kMsg &N2kMsg);
        void OutsideEnvironmental(const tN2kMsg &N2kMsg);
        void Temperature(const tN2kMsg &N2kMsg);
        void TemperatureExt(const tN2kMsg &N2kMsg);
        void COGSOG(const tN2kMsg &N2kMsg);
        void GNSS(const tN2kMsg &N2kMsg);
        void Attitude(const tN2kMsg &N2kMsg);
        void Heading(const tN2kMsg &N2kMsg);
        void Humidity(const tN2kMsg &N2kMsg);
        void Pressure(const tN2kMsg &N2kMsg);
        void WaterDepth(const tN2kMsg &N2kMsg);
        void Rudder(const tN2kMsg &N2kMsg);
        void Speed(const tN2kMsg &N2kMsg);
        void DCStatus(const tN2kMsg &N2kMsg);
        void DCBatteryStatus(const tN2kMsg &N2kMsg);
/*        void SystemTime(const tN2kMsg &N2kMsg);
        void TransmissionParameters(const tN2kMsg &N2kMsg);
        void TripFuelConsumption(const tN2kMsg &N2kMsg);
        void BinaryStatus(const tN2kMsg &N2kMsg);
        void BatteryConfigurationStatus(const tN2kMsg &N2kMsg);
        void LocalOffset(const tN2kMsg &N2kMsg);
        void UserDatumSettings(const tN2kMsg &N2kMsg);
        void GNSSSatsInView(const tN2kMsg &N2kMsg);
        void AddressClaim(const tN2kMsg &N2kMsg);
        void BinaryStatusFull(const tN2kMsg &N2kMsg);


        void printLLNumber(Stream *OutputStream, unsigned long long n, uint8_t base=10);
*/

        Stream *OutputStream;

        EngineData engine[MAX_ENGINE_SOURCES]; // 1
        DcBatteryData dcBattery[MAX_BATTERY_SOURCES]; // 1
        FluidLevelData fluidLevel[MAX_FLUID_LEVEL_SOURCES]; // 1

        HeadingData heading[MAX_HEADDING_SOURCES]; //2
        SpeedData speed[MAX_SPEED_SOURCES]; //2
        WaterDepthData waterDepth[MAX_WATER_DEPTH_SOURCES]; //2
        RudderData rudder[MAX_RUDDER_SOURCES]; //2
        AttitudeData attitude[MAX_ATTITUDE_SOURCES]; // 1

        CogSogData cogSog[MAX_COGSOG_SOURCES]; // 3
        GnssData gnss[MAX_GNSS_SOURCES]; // 3

        OutsideEnvironmentData outsideEnvironmental[MAX_OUTSIDE_ENVIRONMENTAL_SOURCES]; // 4
        HumidityData humidity[MAX_HUMIDITY_SOURCES]; // 4
        PressureData pressure[MAX_PRESSURE_SOURCES]; // 4

        TemperatureData temperature[MAX_TEMPERATURE_SOURCES];   // 5
        TemperatureData temperatureExt[MAX_TEMPERATURE_SOURCES]; // 5

        
};

#endif