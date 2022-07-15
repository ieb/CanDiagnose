#include "modbus.h"


void BatteryMonitor::setUnit(uint8_t _unit) {
    unit = _unit;
}

bool BatteryMonitor::isEnabled() {
    if (unit > 0 ) {
        if ( lastModified+60000 > millis() ) {
            return true;
        }
    }
    return false;
}

void BatteryMonitor::setDiagnostics(bool enabled) {
    diagnosticsEnabled = enabled;
}


void BatteryMonitor::read() {
    if ( unit > 0) {
        int8_t res = modbusMaster.readInput(unit,0,3);
        if ( res == MODBUS_OK ) {
            voltage = modbusMaster.getInt16Response(0)*0.01;
            current = modbusMaster.getInt16Response(1)*0.01;
            temperature = modbusMaster.getInt16Response(2)*0.01;
            if ( diagnosticsEnabled) {
                Serial.print("Modbus ");
                Serial.print(name);
                Serial.print(" voltage:");
                Serial.print(voltage);
                Serial.print("V current:");
                Serial.print(current);
                Serial.print("A temperature");
                Serial.print(temperature);
                Serial.println("C");
            }
            lastModified = millis();
        } else {
            Serial.println("Failed to query measurements");
        }
    }
}
/*
     | 1001   | Recieved        | uint16 | count of frames recieved                |
| 1002   | Sent            | uint16 | count off frames send                   |
| 1003   | Errors Recieved | uint16 | count off frames recieved with errors   |
| 1004   | Ignored         | uint16 | count off frames dropped                |
| 1005   | Errors Sent     | uint16 | count off error frames send             |
*/
void BatteryMonitor::readStats() {
    if ( unit > 0) {
        Serial.print("Modbus ");
        Serial.print(name);
        Serial.print(" unit:");
        Serial.println(unit);
        int8_t res = modbusMaster.readInput(unit,1001,5);
        if ( res == MODBUS_OK ) {
            Serial.print(" stats frames:{recieved:");
            Serial.print(modbusMaster.getUInt16Response(0));
            Serial.print(" sent:");
            Serial.print(modbusMaster.getUInt16Response(1));
            Serial.print(" errors recieved:");
            Serial.print(modbusMaster.getUInt16Response(2));
            Serial.print(" ignored:");
            Serial.print(modbusMaster.getUInt16Response(3));
            Serial.print(" errors sent:");
            Serial.print(modbusMaster.getUInt16Response(4));
            Serial.println("}");
        } else {
            Serial.println("Failed to query stats");
        }
        /*
        | 0      | Device Adddress               | int16 | 1-254 modbus address, default is 2               |
| 1      | Voltage offset adjustment     | int16 | Offset in 0.1mv steps eg 10 = 1mV                |
| 2      | Voltage scale adjustment      | int16 | scale in 1/10000th eg 10000 = 1x                 |
| 3      | Current offset adjustment     | int16 | offset in 0.1mA steps eg 10 = 1mA                |
| 4      | Current scale adjustment      | int16 | scale in 1/10000th eg 10000 = 1x                 |
| 5      | Temperature offset adjustment | int16 | offset in C in 1/1000th eg 1000 = +1C            |
| 6      | Temperature scale adjustment  | int16 | scale in C in 1/1000th eg 1000 = 1x              |
| 7      | Serial Number                 | int16 | Device serial number, once set cannot be changed |
*/
        res = modbusMaster.readHolding(unit,0,8);
        if ( res == MODBUS_OK ) {
            Serial.print(" config address:");
            Serial.print(modbusMaster.getInt16Response(0));
            Serial.print(" voltage offset:");
            Serial.print(0.1*modbusMaster.getInt16Response(1));
            Serial.print("mV scale:");
            Serial.print(0.0001*modbusMaster.getInt16Response(2));
            Serial.print(" current offset:");
            Serial.print(0.1*modbusMaster.getInt16Response(3));
            Serial.print("mA scale:");
            Serial.print(0.0001*modbusMaster.getInt16Response(4));
            Serial.print(" temperature offset:");
            Serial.print(0.1*modbusMaster.getInt16Response(5));
            Serial.print("C scale:");
            Serial.print(0.001*modbusMaster.getInt16Response(6));
            Serial.print(" serial number:");
            Serial.println(modbusMaster.getInt16Response(7));
        } else {
            Serial.println("Failed to query holding");
        }

    }
}

