#include "modbus.h"
#include "config.h"


void Modbus::begin(const char * configurationFile) {
    String v = "";
    if (ConfigurationFile::get(configurationFile, "nmea2000.battery.service.instance", v)) {
        serviceBatteryInstance = v.toInt();
    } else {
        serviceBatteryInstance = 0;
    }
    if (ConfigurationFile::get(configurationFile, "nmea2000.battery.engine.instance", v)) {
        engineBatteryInstance = v.toInt();
    } else {
        engineBatteryInstance = 1;
    }
    if (ConfigurationFile::get(configurationFile, "nmea2000.dcstatus.period", v)) {
        dcstatusPeriod = v.toInt();
    } else {
        dcstatusPeriod = 10000;
    }
    if (ConfigurationFile::get(configurationFile, "modbus.units.servicebattery", v)) {
        serviceBattery.setUnit(v.toInt());
    } else {
        serviceBattery.setUnit(4);
    }
    if (ConfigurationFile::get(configurationFile, "modbus.units.enginebattery", v)) {
        engineBattery.setUnit(v.toInt());
    }
}

void Modbus::setDiagnostics(bool enabled) {
    diagnosticsEnabled = enabled;
    serviceBattery.setDiagnostics(enabled);
    engineBattery.setDiagnostics(enabled);
    modbusMaster.setDiagnostics(enabled);
}

void Modbus::readStats() {
    modbusMaster.readStats();
    serviceBattery.readStats();
    engineBattery.readStats();
}


void Modbus::output() {
    unsigned long now = millis();

    tN2kMsg N2kMsg;
    if ( (now-lastDCStatus) >  dcstatusPeriod) {
        lastDCStatus = now;
        // needs to come over RS485, if no update for 60s dont send.
        if ( serviceBattery.isEnabled() ) {
            SetN2kDCBatStatus(N2kMsg, serviceBatteryInstance, serviceBattery.voltage, serviceBattery.current,CToKelvin(serviceBattery.temperature));
            NMEA2000->SendMsg(N2kMsg);
        }

        if ( serviceBattery.isEnabled()) {
            SetN2kDCBatStatus(N2kMsg, engineBatteryInstance, engineBattery.voltage, engineBattery.current,CToKelvin(engineBattery.temperature));
            NMEA2000->SendMsg(N2kMsg);
        }
    }       
}
void Modbus::read() {
    unsigned long now = millis();
    // this is async so looks a bit odd.
    // readResponse reads the response if one is available and accumulates it
    // not blocking, 
    modbusMaster.readResponseAsync();
    // send a request get get the data for the service battery.
    // until its read more reads are noops so its ok to call repeatedly
    serviceBattery.read();
    engineBattery.read();
    if ( (now-lastRead) >  readPeriod )  {
        // Update the history periodically.
        // this may get the value from the last read, but thats ok.
        lastRead = now;
        storeHistory(serviceBattery.voltage);
    }

}


void Modbus::outputJson(AsyncResponseStream *outputStream) {
    startJson(outputStream);
    append("t",millis());
    append("nmea2000",false);
    startArray("battery");
    startObject();
    append("id",0);
    append("lastModified",serviceBattery.lastModified);
    append("voltage",serviceBattery.voltage);
    append("current",serviceBattery.current);
    append("temperature",serviceBattery.temperature);
    endObject();
    startObject();
    append("id",1);
    append("lastModified",engineBattery.lastModified);
    append("voltage",engineBattery.voltage);
    append("current",engineBattery.current);
    append("temperature",engineBattery.temperature);
    endObject();
    endArray();
    endJson();
}


//# battery,id,t,lastModified,voltage,current,temperature
// see also csvoutput.cpp where NMEA2000 batteries are output
// NMEA2000 and modbus are registered at different IDs so the 
// id fields here will not clash.
void Modbus::outputCsv(AsyncResponseStream *outputStream) {
    startBlock(outputStream);
    startRecord("battery");
    appendField(1);
    appendField(serviceBattery.lastModified);
    appendField(serviceBattery.voltage);
    appendField(serviceBattery.current);
    appendField(serviceBattery.temperature);
    endRecord();
    startRecord("battery");
    appendField(2);
    appendField(engineBattery.lastModified);
    appendField(engineBattery.voltage);
    appendField(engineBattery.current);
    appendField(engineBattery.temperature);
    endRecord();
    endBlock();
}






