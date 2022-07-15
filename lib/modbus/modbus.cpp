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
        serviceBattery.setUnit(2);
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
    if ( now > lastDCStatus + dcstatusPeriod) {
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
    if ( now > lastRead + readPeriod )  {
        lastRead = now;
        serviceBattery.read();
        engineBattery.read();
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
void Modbus::outputCsv(AsyncResponseStream *outputStream) {
    startBlock(outputStream);
    startRecord("battery");
    appendField(0);
    appendField(serviceBattery.lastModified);
    appendField(serviceBattery.voltage);
    appendField(serviceBattery.current);
    appendField(serviceBattery.temperature);
    endRecord();
    startRecord("battery");
    appendField(1);
    appendField(engineBattery.lastModified);
    appendField(engineBattery.voltage);
    appendField(engineBattery.current);
    appendField(engineBattery.temperature);
    endRecord();
    endBlock();
}




bool Modbus::drawPage(Adafruit_SSD1306 * display) {
    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
    switch(subPage) {
        case 0:
            display->setTextSize(2);   // 18x24  128/24=7 chars 5,3 
            if (serviceBattery.isEnabled() ) {
                display->printf("%5.2f V Sv\n", serviceBattery.voltage); 
                display->printf("%5.1f A\n", serviceBattery.current); 
            } else {
                display->printf("--.-- V Sv\n"); 
                display->printf("---.- A\n"); 

            }
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->setTextSize(2);
            if (engineBattery.isEnabled() ) {
                display->printf("%5.2f V Ev\n", engineBattery.voltage); 
                display->printf("%5.1f A\n", engineBattery.current); 
            } else {
                display->printf("--.-- V Ev\n"); 
                display->printf("---.- A\n"); 

            }
            display->display();
            subPage = 2;
            return false;
        case 2:
            if ( drawHistory(display,true,10.0,15.0) ) {
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
            display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
            display->setCursor(0,4);  
            if ( engineBattery.isEnabled() ) {
                display->printf("%5.2f V Ev\n",engineBattery.voltage);
            } else {
                display->printf("--.-- V Ev\n");
            }
            display->setCursor(0,24); // 4+16+4
            if ( serviceBattery.isEnabled() ) {
                display->printf("%5.2f V Sv\n", serviceBattery.voltage);
                display->setCursor(0,44); // 24+16+4
                display->printf("%5.1f A Sa\n", serviceBattery.current); 
            } else {
                display->printf("--.-- V Sv\n");
                display->setCursor(0,44); // 24+16+4
                display->printf("---.- A Sa\n"); 
            }
            display->display();
            subPage = 1;
            return false;
        case 1:
            if ( drawHistory(display,true,10.0,15.0) ) {
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


