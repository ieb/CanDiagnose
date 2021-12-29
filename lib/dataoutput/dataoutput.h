#ifndef DATAOUTPUT_H
#define DATAOUTPUT_H

#include <NMEA2000.h>
#include <N2kMessages.h>
#include "httpserver.h"
#include "display.h"

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
#define MAX_FLUID_LEVEL_SOURCES 4
#define MAX_XTE_SOURCES 2
#define MAX_VARIATION_SOURCES 2
#define MAX_WIND_SOURCES 2
#define MAX_LOG_SOURCES 2
#define MAX_POSSITION_SOURCES 3
#define MAX_LEEWAY_SOURCES 2


class MessageStore {
    public:
        unsigned char source = 255;
        unsigned long lastModified = 0;
        bool use(unsigned char msgSource, bool useOld) {
            unsigned long now = millis();
            if (source == 255) {
                source = msgSource;
                lastModified = now;
                return true; 
            } else if (source == msgSource) {
                lastModified = now;
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

class MessageStoreWithInstance : public MessageStore {
    public:
        byte instance;
        bool use(unsigned char msgSource) {
            return false;
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


class AttitudeData: public MessageStore {
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


class WaterDepthData: public MessageStore {
    public:
        double offset;
        double depthBelowTransducer; 
};

class SpeedData: public MessageStore {
    public:
        double sow;
        double sog;
        tN2kSpeedWaterReferenceType swrt;
};


class DcBatteryData : public MessageStoreWithInstance {
    public:
        double voltage;
        double current;
        double temperature;
};



class PressureData: public MessageStoreWithInstance {
    public:
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


class OutsideEnvironmentData : public MessageStore {
    public:
        double waterTemperature;
        double outsideAmbientAirTemperature;
        double atmosphericPressure;
};

class GnssData : public MessageStore {
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

class CogSogData: public MessageStore {
    public:
        tN2kHeadingReference reference;
        double cog;
        double sog;
};

class HeadingData : public MessageStore {
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

class VariationData : public MessageStore {
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

class LogData : public MessageStore {
    public:
        uint16_t daysSince1970;
        double secondsSinceMidnight;
        uint32_t log;
        uint32_t tripLog;
};

class PossitionData : public MessageStore {
    public:
        double latitude;
        double longitude;
};


class LeewayData: public MessageStoreWithInstance {
    public:
        double leeway;
};






class DataCollector {
    public:
        DataCollector(Stream *outputStream) : outputStream{outputStream} {};
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

        TemperatureData temperature[MAX_TEMPERATURE_SOURCES];   // 5
        TemperatureData temperatureExt[MAX_TEMPERATURE_SOURCES]; // 5

        XteData xte[MAX_XTE_SOURCES];
        VariationData variation[MAX_VARIATION_SOURCES];
        WindData wind[MAX_WIND_SOURCES];
        LogData log[MAX_LOG_SOURCES];
        PossitionData possition[MAX_POSSITION_SOURCES];
        LeewayData leeway[MAX_LEEWAY_SOURCES];
        LogData *getLog();
        WindData * getWindInstance(byte instance, tN2kWindReference reference=N2kWind_Apparent);
        EngineData * getEngineInstance(byte instance);
        FluidLevelData * getFluidLevelInstance(byte instance);
        PossitionData * getPossition();
        GnssData * getGnss();
        bool getLatLong(double &latitude, double &longitude, int16_t &age);



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

class EngineDataOutput: public JsonOutput, public DisplayPage {
    public:
        EngineDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        bool drawPage(Adafruit_SSD1306 * display);

    private:
        DataCollector &dataCollector;

};

class BoatDataOutput: public JsonOutput {
    public:
        BoatDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
    private:
        DataCollector &dataCollector;
};
class NavigationDataOutput: public JsonOutput {
    public:
        NavigationDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        DataCollector &dataCollector;
};

class EnvironmentDataOutput: public JsonOutput {
    public:
        EnvironmentDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        DataCollector &dataCollector;
};
class TemperatureDataOutput: public JsonOutput {
    public:
        TemperatureDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        DataCollector &dataCollector;
};

class XteDataOutput: public JsonOutput {
    public:
        XteDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        DataCollector &dataCollector;
};

class MagneticVariationDataOutput: public JsonOutput {
    public:
        MagneticVariationDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        DataCollector &dataCollector;
};

class WindSpeedDataOutput: public JsonOutput, public DisplayPage {
    public:
        WindSpeedDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        bool drawPage(Adafruit_SSD1306 * display);

    private:
        Stream *outputStream;
        DataCollector &dataCollector;
};
class LogDataOutput: public JsonOutput, public DisplayPage {
    public:
        LogDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        bool drawPage(Adafruit_SSD1306 * display);
    private:
        Stream *outputStream;
        DataCollector &dataCollector;
};
class LatLonDataOutput: public JsonOutput, public DisplayPage {
    public:
        LatLonDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        bool drawPage(Adafruit_SSD1306 * display);
    private:
        Stream *outputStream;
        DataCollector &dataCollector;
};
class LeewayDataOutput: public JsonOutput {
    public:
        LeewayDataOutput(DataCollector &dataCollector): dataCollector{dataCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        DataCollector &dataCollector;
};

#endif
