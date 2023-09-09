#include "modbus.h"


/*
# Holding registers

|| Register || type || units per bit || R/W || description ||
| 0 | int16 | serial number | R | serial number, can be written once only |
| 1 | int16 | device address | R/W | modbus unit or device address |
| 2 | int16 | A | R/W | Shunt full scale current range, default 100A |
| 3 | int16 | mV | R/W | Shunt full scale voltage, default 75mV |
| 4 | int16 | 0.1C | R/W | Temperature probe 1 offset, default 0 |
| 5 | int16 | 0.001x | R/W | Temperature probe 1 scale, default 1000 eg 1x |
| 6 | int16 | 0.1C | R/W | Temperature probe 2 offset, default 0 |
| 7 | int16 | 0.001x | R/W | Temperature probe 2 scale, default 1000 eg 1x |


# Input registers

|| Register || type || units per bit || description ||
|  0   | int16 | mV | Battery voltage |
|  1   | int16 | 0.1 A | Shunt current |
|  2   | int16 | 0.01 C | Temperature probe 1 |  
|  3   | int16 | 0.01 C | Temperature probe 2 |  
|  4   | int16 | bitmap | Lower 8 bits are Serial documented [here](https://github.com/SpenceKonde/megaTinyCore/blob/master/megaavr/extras/Ref_Serial.md#serialgetstatus) bit 8 indicates and error reading voltage, bit 9 indicates and error reading current  |
|  5   | int16 | mV | MCU Supply voltage |
|  6   | int16 | 0.1 C | MCU Temperature |
|  1000  | int16 | counter | modbus frames recieved |
|  1001   | int16 | counter | modbus frames sent |
|  1002   | int16 | counter | modbus errors recieved |
|  1003   | int16 | counter | modbus frames ignored |
|  1004   | int16 | counter | modbus errors sent |
|  1005   | int16 | counter | modbus buffer overflow |

*/


void ModbusShunt::setUnit(uint8_t _unit) {
    Serial.print("Modbus ");
                Serial.print(name);
    unit = _unit;
}

bool ModbusShunt::isEnabled() {
    if (unit > 0 ) {
        if ( (millis()-lastModified) < 60000  ) {
            return true;
        }
    }
    return false;
}

void ModbusShunt::setDiagnostics(bool enabled) {
    diagnosticsEnabled = enabled;
}

/*
|| Register || type || units per bit || description ||
|  0   | int16 | mV | Battery voltage |
|  1   | int16 | 0.1 A | Shunt current |
|  2   | int16 | 0.01 C | Temperature probe 1 |  
|  3   | int16 | 0.01 C | Temperature probe 2 |  
*/

void ModbusShunt::read() {
    if ( unit > 0 && ((millis()-lastModified) > readPeriod) ) {
        // read can be called as many times as liked, but it wont read if its already readng
        // if its already running, then this is a noop and can be called as often as required.

        if ( modbusMaster.readInputAsync(unit,0,4, this) == MODBUS_SENT_OK ) {
            readCommand = READING_VOLTAGE;
        }
    }
}
void ModbusShunt::onResponse(int res) {
    if ( res == MODBUS_OK ) {
        switch(readCommand) {
            case READING_VOLTAGE:
                voltage = modbusMaster.getInt16Response(0)*0.001;
                current = modbusMaster.getInt16Response(1)*0.1;
                temperature = modbusMaster.getInt16Response(2)*0.01;
                temperatureOpt = modbusMaster.getInt16Response(3)*0.01;
                if ( diagnosticsEnabled) {
                    Serial.print("Modbus ");
                    Serial.print(name);
                    Serial.print(" voltage:");
                    Serial.print(voltage);
                    Serial.print("V current:");
                    Serial.print(current);
                    Serial.print("A temperature:");
                    Serial.print(temperature);
                    Serial.print("C temperature opt:");
                    Serial.print(temperatureOpt);
                    Serial.println("C");
                }
                lastModified = millis();
                readCommand = READING_NONE;
                break;
            case READING_STATS1:
/*
|| Register || type || units per bit || description ||
|  1000  | int16 | counter | modbus frames recieved |
|  1001   | int16 | counter | modbus frames sent |
|  1002   | int16 | counter | modbus errors recieved |
|  1003   | int16 | counter | modbus frames ignored |
|  1004   | int16 | counter | modbus errors sent |
|  1005   | int16 | counter | modbus buffer overflow |
*/
                if ( diagnosticsEnabled ) {
                    Serial.print(" stats frames:{recieved:");
                    Serial.print(modbusMaster.getInt16Response(0));
                    Serial.print(" sent:");
                    Serial.print(modbusMaster.getInt16Response(1));
                    Serial.print(" errors recieved:");
                    Serial.print(modbusMaster.getInt16Response(2));
                    Serial.print(" ignored:");
                    Serial.print(modbusMaster.getInt16Response(3));
                    Serial.print(" errors sent:");
                    Serial.print(modbusMaster.getInt16Response(4));
                    Serial.print(" buffer overflow:");
                    Serial.print(modbusMaster.getInt16Response(5));
                    Serial.println("}");
                }

                res = modbusMaster.readHoldingAsync(unit,0,8, this);
                if ( res == MODBUS_SENT_OK ) {
                    readCommand = READING_STATS2;
                } else if (res == MODBUS_BUSY ) {
                    if ( diagnosticsEnabled ) {
                        Serial.println("Failed to query stats2, async operation in progress");
                    }
                } else {
                    if ( diagnosticsEnabled ) {
                        Serial.println("Failed to query holding");
                    }
                }
                break;
            case READING_STATS2:
/*
|| Register || type || units per bit || R/W || description ||
| 0 | int16 | serial number | R | serial number, can be written once only |
| 1 | int16 | device address | R/W | modbus unit or device address |
| 2 | int16 | A | R/W | Shunt full scale current range, default 100A |
| 3 | int16 | mV | R/W | Shunt full scale voltage, default 75mV |
| 4 | int16 | 0.1C | R/W | Temperature probe 1 offset, default 0 |
| 5 | int16 | 0.001x | R/W | Temperature probe 1 scale, default 1000 eg 1x |
| 6 | int16 | 0.1C | R/W | Temperature probe 2 offset, default 0 |
| 7 | int16 | 0.001x | R/W | Temperature probe 2 scale, default 1000 eg 1x |

*/
                if ( diagnosticsEnabled ) {
                    Serial.print(" serial number:");
                    Serial.print(modbusMaster.getInt16Response(0));
                    Serial.print(" device address:");
                    Serial.print(modbusMaster.getInt16Response(1));
                    Serial.print(" Shunt fully scale:");
                    Serial.print(modbusMaster.getInt16Response(2));
                    Serial.print("A Shunt full scale v:");
                    Serial.print(modbusMaster.getInt16Response(3));
                    Serial.print("mV temperature offset:");
                    Serial.print(0.1*modbusMaster.getInt16Response(4));
                    Serial.print("C temperature scale:");
                    Serial.print(0.001*modbusMaster.getInt16Response(5));
                    Serial.print(" temperature opt offset:");
                    Serial.print(0.1*modbusMaster.getInt16Response(6));
                    Serial.print("C temperature opt scale:");
                    Serial.println(0.001*modbusMaster.getInt16Response(7));
                }
                readCommand = READING_NONE;
                break;
        }
    } else {
        //Serial.print("Modbus ");
        //Serial.print(name);
        //Serial.print(" unit:");
        //Serial.print(unit);
        //Serial.print(" res:");
        //Serial.print(res);
        //Serial.println(" Failed to query measurements");        
    }
}


void ModbusShunt::readStats() {
    if ( unit > 0) {
        if ( diagnosticsEnabled ) {
             /* code */
            Serial.print("Modbus ");
            Serial.print(name);
            Serial.print(" unit:");
            Serial.println(unit);

        }
        int8_t res = modbusMaster.readInputAsync(unit,1000,6, this);
        if ( res == MODBUS_SENT_OK ) {
            readCommand = READING_STATS1;
        } else if (res == MODBUS_BUSY ) {
            if ( diagnosticsEnabled ) {
                Serial.println("Failed to query stats, async operation in progress");
            }
        } else {
            if ( diagnosticsEnabled ) {
                Serial.println("Failed to query stats");
            }
        }
    }
}

