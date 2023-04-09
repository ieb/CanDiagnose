#pragma once

/**
 * Collects and stores data from a N2K feed.
 */ 

#include <NMEA2000.h>
#include <N2kMessages.h>

#define MAX_HEADDING_SOURCES 5
#define MAX_ENGINE_SOURCES 2
#define MAX_COGSOG_SOURCES 2
#define MAX_GNSS_SOURCES 2
#define MAX_OUTSIDE_ENVIRONMENTAL_SOURCES 2
#define MAX_ENVIRONMENTAL_PARAMS_SOURCES 2
#define MAX_TEMPERATURE_SOURCES 5
#define MAX_HUMIDITY_SOURCES 1
#define MAX_PRESSURE_SOURCES 1
#define MAX_BATTERY_SOURCES 4
#define MAX_SPEED_SOURCES 3
#define MAX_WATER_DEPTH_SOURCES 2
#define MAX_RUDDER_SOURCES 3
#define MAX_ATTITUDE_SOURCES 2
#define MAX_FLUID_LEVEL_SOURCES 4
#define MAX_XTE_SOURCES 2
#define MAX_VARIATION_SOURCES 2
#define MAX_WIND_SOURCES 2
#define MAX_LOG_SOURCES 2
#define MAX_POSSITION_SOURCES 3
#define MAX_LEEWAY_SOURCES 2




class MessageStoreWithInstance  {
    public:
        byte instance = 255;
        unsigned char source = 255;
        unsigned long lastModified = 0;
        bool active() {
            return (source != 255) && (instance != 255);
        };
        bool use(unsigned char msgSource,  byte msgInstance, bool useOld) {
            unsigned long now = millis();
            if ( source == 255) {
                source = msgSource;
                instance = msgInstance;
                lastModified = millis();
                return true;
            } else if ( source == msgSource && instance == msgInstance ) {
                lastModified = millis();
                return true;
            } else if (useOld && now > lastModified+600000 ) {
                // no update for 10m so assume not transmitting any more
                source = msgSource;
                lastModified = now;
                return true; 
            }
            return false;
        }; 
};

class History128over24 {
    public:
        /**
         * offset and scale are the offset and scale applied before the value 
         * is stored as a uint16. This manipulation is undone on rendering the history
         * or outputting. Remember to make sure the value can be stored in an uint16.
         */
        History128over24(float offset, float scale, unsigned long periodMs=675000) : 
            offset{offset}, 
            scale{scale}, 
            historyPeriod{periodMs} { };
        void storeHistory(double v);
        float changeSince(unsigned long tms);
        void startIterator();
        bool hasNext();
        float nextValue();
        unsigned long getPeriodMs() { return historyPeriod; };
        unsigned long getTimeWindowMs() { return historyPeriod*128; };
        uint16_t getRawValueFor(float v) { return (uint16_t)(round((v-offset)*scale));};
        uint16_t getRawScaleFor(float v) { return (uint16_t)(round((v)*scale));};
        float getRealValue(uint16_t v) { return (float)(offset+(v/scale));};
        float getFirstRawValue() { return history[0];};
        uint16_t nextValueRaw();
    private:
        uint8_t index(unsigned long t) { return (t/historyPeriod)%128; };
        float offset;
        float scale;
        unsigned long historyPeriod =  675000;
        static const unsigned long historyLength =  128;
        int currentSlot=-1;
        double pmean;
        int hend;
        int iv;
        uint8_t iteratorState;
        uint16_t history[128];
};



class AttitudeData: public MessageStoreWithInstance {
    public:
        double roll;
        double pitch;
        double yaw;
};

class FluidLevelData: public MessageStoreWithInstance  {
    public:
        tN2kFluidType fluidType;
        double level;
        double capacity;
};


class RudderData: public MessageStoreWithInstance {
    public:
        tN2kRudderDirectionOrder directionOrder;
        double position;
        double angleOrder;
};


class WaterDepthData: public MessageStoreWithInstance {
    public:
        double offset;
        double depthBelowTransducer; 
};

class SpeedData: public MessageStoreWithInstance {
    public:
        double stw;
        double sog;
        tN2kSpeedWaterReferenceType swrt;
};


class DcBatteryData : public MessageStoreWithInstance {
    public:
        double voltage;
        double current;
        double temperature;
};


// max is 1030 * 10 == 10300 which fits into a unit16 for history. 

class PressureData: public MessageStoreWithInstance, public History128over24 {
    public:
        PressureData() : History128over24{0.0,10.0,675000} {};
        tN2kPressureSource sourceSensor;
        double actual;
};

class HumidityData: public MessageStoreWithInstance {
    public:
        unsigned char instance;
        tN2kHumiditySource sourceSensor;
        double actual;
        double set;
};

class TemperatureData : public MessageStoreWithInstance {
    public:
        tN2kTempSource sourceSensor;
        double actual;
        double set;
};


class OutsideEnvironmentData : public MessageStoreWithInstance {
    public:
        double waterTemperature;
        double outsideAmbientAirTemperature;
        double atmosphericPressure;
};


class EnvironmentalParamsData  : public MessageStoreWithInstance {
    public:
        tN2kTempSource temperatureSource;
        tN2kHumiditySource humiditySource;
        double temperature;
        double humidity;
        double atmosphericPressure;

};
class GnssData : public MessageStoreWithInstance {
    public:
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
};

class CogSogData: public MessageStoreWithInstance {
    public:
        tN2kHeadingReference reference;
        double cog;
        double sog;
};

class HeadingData : public MessageStoreWithInstance {
    public:
        double heading;
        double deviation;
        double variation;
        tN2kHeadingReference reference;
};

class EngineData : public MessageStoreWithInstance {
    public:
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
};

class XteData : public MessageStoreWithInstance  {
    public:
        double xte;
};

class VariationData : public MessageStoreWithInstance {
    public:
        uint16_t daysSince1970;
        double variation;
};

class WindData : public MessageStoreWithInstance {
    public:
        double windSpeed;
        double windAngle;
        tN2kWindReference windReference;
};

class LogData : public MessageStoreWithInstance {
    public:
        uint16_t daysSince1970;
        double secondsSinceMidnight;
        uint32_t log;
        uint32_t tripLog;
};

class PossitionData : public MessageStoreWithInstance {
    public:
        double latitude;
        double longitude;
};


class LeewayData: public MessageStoreWithInstance {
    public:
        double leeway;
};


/*
 * This contains performance data derived 
 * from other values. There is only 1 instance of this.
 */
class N2KCollector;

class PerformanceData {
public:
    PerformanceData() {};
    unsigned long lastModified = 0;
    void update(N2KCollector *n2kCollector);
    WindData trueWind;
    float leeway, 
        polarSpeed, 
        polarSpeedRatio, 
        polarVmg, 
        vmg, 
        targetTwa, 
        targetVmg, 
        targetStw, 
        polarVmgRatio,
        windDirectionTrue, 
        windDirectionMagnetic, 
        oppositeTrackHeadingTrue, 
        oppositeTrackHeadingMagnetic, 
        oppositeTrackTrue,
        oppositeTrackMagnetic;
private:
    float calcPolarSpeed(float twsv, float twav, bool nmea2000Units=true);
    void updateTWSIdx(float tws);
    void updateTWAIdx(float twa);
    void findIndexes(uint8_t v, const uint8_t *a, uint8_t al, int8_t *idx  );
    float interpolateForY(float x, float xl, float xh, float yl, float yh);
    float correctBearing(float b);
    uint8_t lastTWSV = 0;
    uint8_t lastTWAV = 0;
    int8_t twaidx[2] = {0,0};
    int8_t twsidx[2] = {0,0};
};




class N2KCollector {
    public:
        N2KCollector(Stream *outputStream) : outputStream{outputStream} {};
        void HandleMsg(const tN2kMsg &N2kMsg);
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
        EnvironmentalParamsData environmentalParams[MAX_ENVIRONMENTAL_PARAMS_SOURCES]; // 4

        TemperatureData temperature[MAX_TEMPERATURE_SOURCES];   // 5
        TemperatureData temperatureExt[MAX_TEMPERATURE_SOURCES]; // 5

        XteData xte[MAX_XTE_SOURCES];
        VariationData variation[MAX_VARIATION_SOURCES];
        WindData wind[MAX_WIND_SOURCES];
        LogData log[MAX_LOG_SOURCES];
        PossitionData possition[MAX_POSSITION_SOURCES];
        LeewayData leeway[MAX_LEEWAY_SOURCES];
        PerformanceData performance;
        LogData *getLog();

        WindData * getWindInstance(byte instance, tN2kWindReference reference=N2kWind_Apparent);
        EngineData * getEngineInstance(byte instance=0);
        FluidLevelData * getFluidLevelInstance(byte instance);
        PossitionData * getPossition();
        GnssData * getGnss();
        CogSogData * getCogSog();
        bool getLatLong(double &latitude, double &longitude, int16_t &age);
        bool getPressure( double &pressure, int16_t &age);

        PressureData * getAtmosphericPressure();
        SpeedData * getSpeed();
        HeadingData * getHeading();
        VariationData * getVariation();
        AttitudeData * getAttitude();
        WindData * getAparentWind();
        WindData * getTrueWind() {
            return &(performance.trueWind);
        };
        DcBatteryData * getBatteryInstance(byte instance);
        FluidLevelData * getFuelLevel();
        WaterDepthData * getWaterDepth();



    private:
        void EngineRapid(const tN2kMsg &N2kMsg);
        void EngineDynamicParameters(const tN2kMsg &N2kMsg);
        void FluidLevel(const tN2kMsg &N2kMsg);
        void OutsideEnvironmental(const tN2kMsg &N2kMsg);
        void EnvironmentalParameters(const tN2kMsg &N2kMsg);
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
        void Xte(const tN2kMsg &N2kMsg);
        void MagneticVariation(const tN2kMsg &N2kMsg);
        void WindSpeed(const tN2kMsg &N2kMsg);
        void Log(const tN2kMsg &N2kMsg);
        void LatLon(const tN2kMsg &N2kMsg);
        void Leeway(const tN2kMsg &N2kMsg);
        void saveFailed(const tN2kMsg &N2kMsg, byte instance=255);
        void parseFailed(const tN2kMsg &N2kMsg);

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

        Stream *outputStream;




        
};