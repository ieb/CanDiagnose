#pragma once

/**
 * Collects data from modbus devices on RS485 over serial.
 */ 

#define MODBUS_MAX_COUNT 32
#define MODBUS_BUFFER_LENGTH 72 // 32*2+8

#include <Arduino.h>
#include <NMEA2000.h>
#include <N2kMessages.h>
#include "N2KCollector.h"
#include "httpserver.h"



#define MODBUS_OK 0  // response was ol
#define MODBUS_TIMEOUT 1  // timeout waiting to recieve
#define MODBUS_WRONGUNIT 2  // recieved a message from the wrong unit
#define MODBUS_WRONGFUNCTION 3 // recieved and unexpected function
#define MODBUS_EXCEPTION 4 // recieved an exveption
#define MODBUS_CRCFAIL 5 // CRC check failed
#define MODBUS_TOOMANY 6 // too many registers requested
#define MODBUS_SENT_OK 7    // returned from send calls if sent ok
#define MODBUS_RECIEVE 8 // recieving frame from unit
#define MODBUS_BUSY 9 // return from send calls when busy
#define MODBUS_IDLE 10 // modbus is idle
#define MODBUS_RECEIVE_OVERFLOW 11 // the recieve buffer overflowed reading


#define READING_NONE 0
#define READING_VOLTAGE 1
#define READING_STATS1 2
#define READING_STATS2 3


class ModbusCallback {
    public:
        virtual void onResponse(int state);

};


class ModbusMaster {

    public:
        ModbusMaster(HardwareSerial *io, uint8_t enPin): io{io}, enPin{enPin} {}; 
        void begin();
        void readStats();
/*        int8_t readInput(uint8_t unit, uint16_t regNo, int8_t count);
        int8_t readHolding(uint8_t unit, uint16_t regNo, int16_t count);
        int8_t writeHolding(uint8_t unit, uint16_t regNo, int16_t value); */
        int8_t readInputAsync(uint8_t unit, uint16_t regNo, int8_t count, ModbusCallback *callback);
        int8_t readHoldingAsync(uint8_t unit, uint16_t regNo, int16_t count, ModbusCallback *callback);
        int8_t writeHoldingAsync(uint8_t unit, uint16_t regNo, int16_t value, ModbusCallback *callback);
        void readResponseAsync();

        int16_t getInt16Response(uint8_t offset);
        uint16_t getUInt16Response(uint8_t offset);
        void setDiagnostics(bool enabled);

    private:
        HardwareSerial *io;
        uint8_t enPin;
        ModbusCallback *modbusCallback;
        int8_t state = MODBUS_IDLE;  // state of the modbus master
        uint8_t buffer[MODBUS_BUFFER_LENGTH];
        uint8_t frameLength;
        uint8_t numRegisters;
        uint8_t recieveFromUnit;
        uint8_t readFunction;
        uint16_t startRegister;
        int16_t registerCount;
        uint8_t lenToRead;
        unsigned long messageSent;
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
        void debugDumpFrame(uint8_t length);
        bool checkCrc(uint8_t p);
        bool checkRegisterOffset(uint8_t offset);

        uint16_t crc16(const uint8_t *array, uint16_t length);


};


class ModbusShunt: public ModbusCallback {
    public:
        ModbusShunt(const char *name, ModbusMaster &modbusMaster, unsigned long readPeriod=10000) : 
                name{name}, 
                modbusMaster{modbusMaster},
                readPeriod{readPeriod}
                {};
        void read();
        void readStats();
        void setUnit(uint8_t unit);
        bool isEnabled();
        void setDiagnostics(bool enabled);
        void onResponse(int state) override;


        double voltage;     
        double current;     
        double temperature; 
        double temperatureOpt; 
        unsigned long lastModified;    
    private:
        const char * name;
        ModbusMaster &modbusMaster;
        unsigned long readPeriod;
        uint8_t readCommand = READING_NONE;
        uint8_t unit = 0;
        unsigned long lastRead = 0;
        bool diagnosticsEnabled = false;

};

class Modbus : public JsonOutput, public CsvOutput, public History128over24  {
    public:
        Modbus(tNMEA2000 *NMEA2000, ModbusMaster &modbusMaster, unsigned long readPeriod=10000):  
            History128over24{0.0,1000.0,675000}, 
            NMEA2000{NMEA2000},
            modbusMaster{modbusMaster},
            readPeriod{readPeriod},
            serviceBattery{"Service Battery",modbusMaster, readPeriod},
            engineBattery{"Engine Battery",modbusMaster, readPeriod} {}
        void begin(const char * configurationFile="/config.txt");
        void read();
        void readStats();
        void output();
        //void addShunt(ModbusShunt &bm);
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
        void setDiagnostics(bool enabled);

        ModbusShunt * getEngineBattery() {
            return &engineBattery;
        };

        ModbusShunt * getServiceBattery() {
            return &serviceBattery;
        };


    private:
        tNMEA2000 *NMEA2000;
        ModbusMaster &modbusMaster;
        unsigned long readPeriod;
        ModbusShunt serviceBattery;
        ModbusShunt engineBattery;
        uint8_t serviceBatteryInstance;
        uint8_t engineBatteryInstance;
        unsigned long dcstatusPeriod;
        unsigned long lastDCStatus;
        unsigned long lastRead;
        bool diagnosticsEnabled = false;

};