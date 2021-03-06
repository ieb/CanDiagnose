#pragma once

#define MAX_COUNT 32

#include <Arduino.h>
#include <NMEA2000.h>
#include <N2kMessages.h>
#include "httpserver.h"
#include "display.h"


#define MODBUS_OK 0
#define MODBUS_TIMEOUT 1
#define MODBUS_WRONGUNIT 2
#define MODBUS_WRONGFUNCTION 3
#define MODBUS_EXCEPTION 4
#define MODBUS_CRCFAIL 5
#define MODBUS_TOOMANY 6
#define MODBUS_FAIL 7


class ModbusMaster {

    public:
        ModbusMaster(HardwareSerial *io, uint8_t enPin): io{io}, enPin{enPin} {}; 
        void begin();
        void readStats();
        int8_t readInput(uint8_t unit, uint16_t regNo, int8_t count);
        int8_t readHolding(uint8_t unit, uint16_t regNo, int16_t count);
        int8_t writeHolding(uint8_t unit, uint16_t regNo, int16_t value);
        int16_t getInt16Response(uint8_t offset);
        uint16_t getUInt16Response(uint8_t offset);
        void setDiagnostics(bool enabled);

    private:
        HardwareSerial *io;
        uint8_t enPin;
        uint8_t buffer[MAX_COUNT*2+8];
        uint8_t frameLength;
        uint8_t numRegisters;
        bool diagnosticsEnabled = false;
        uint16_t sent = 0;
        uint16_t recieved = 0;
        uint16_t errors = 0;

        void consumeTraffic();
        void recieveEnable();
        void transmitEnable();
        int8_t readResponse(uint8_t unit, uint8_t function, int16_t len=-1);
        void send();
        void dumpFrame(uint8_t length);
        bool checkCrc(uint8_t p);
        bool checkRegisterOffset(uint8_t offset);

        uint16_t crc16(const uint8_t *array, uint16_t length);


};


class BatteryMonitor {
    public:
        BatteryMonitor(const char *name, ModbusMaster &modbusMaster) : name{name}, modbusMaster{modbusMaster} {};
        void read();
        void readStats();
        void setUnit(uint8_t unit);
        bool isEnabled();
        void setDiagnostics(bool enabled);

        double voltage;     
        double current;     
        double temperature; 
        unsigned long lastModified;    
    private:
        const char * name;
        ModbusMaster &modbusMaster;
        uint8_t unit = 0;
        unsigned long lastRead = 0;
        bool diagnosticsEnabled = false;

};

class Modbus : public JsonOutput, public CsvOutput, public DisplayPage, public History128over24  {
    public:
        Modbus(tNMEA2000 *NMEA2000, ModbusMaster &modbusMaster, unsigned long readPeriod=10000):  
            History128over24{0.0,1000.0,675000}, 
            NMEA2000{NMEA2000},
            modbusMaster{modbusMaster},
            readPeriod{readPeriod},
            serviceBattery{"Service Battery",modbusMaster},
            engineBattery{"Engine Battery",modbusMaster} {}
        void begin(const char * configurationFile="/config.txt");
        void read();
        void readStats();
        void output();
        void addBattery(BatteryMonitor &bm);
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
        bool drawPage(Adafruit_SSD1306 * display);
        void setDiagnostics(bool enabled);

    private:
        tNMEA2000 *NMEA2000;
        ModbusMaster &modbusMaster;
        unsigned long readPeriod;
        BatteryMonitor serviceBattery;
        BatteryMonitor engineBattery;
        uint8_t serviceBatteryInstance;
        uint8_t engineBatteryInstance;
        unsigned long dcstatusPeriod;
        unsigned long lastDCStatus;
        unsigned long lastRead;
        bool diagnosticsEnabled = false;

};