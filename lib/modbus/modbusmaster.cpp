#include "modbus.h"



void ModbusMaster::begin() {
    pinMode(enPin,OUTPUT);
    recieveEnable();
}

void ModbusMaster::setDiagnostics(bool enabled) {
    diagnosticsEnabled = enabled;
}

void ModbusMaster::readStats() {
    Serial.print("ModbsMaster sent:");
    Serial.print(sent);
    Serial.print(" recieved:");
    Serial.print(recieved);
    Serial.print(" errors:");
    Serial.println(errors);
}

int8_t ModbusMaster::readInput(uint8_t unit, uint16_t regNo, int8_t count) {
    if ( count > MAX_COUNT ) {
        return MODBUS_TOOMANY;
    }
    unsigned long tstart = micros();
    // clear pending data from the serial buffer.
    consumeTraffic();
    frameLength = 0;
    numRegisters = 0;

    buffer[frameLength++] = unit;
    buffer[frameLength++] = 0x04;
    buffer[frameLength++] = 0xff&(regNo>>8);
    buffer[frameLength++] = 0xff&regNo;
    buffer[frameLength++] = 0xff&(count>>8);
    buffer[frameLength++] = 0xff&count;
    send();

    // expecting to recieve 5+count*2 bytes in the response
    // first read 2 bytes to 
    int8_t ret = readResponse(unit, 0x04);
    if ( diagnosticsEnabled ) {
        Serial.print("Modbus read input unit:");
        Serial.print(unit);
        Serial.print(" reg:");
        Serial.print(regNo);
        Serial.print(" n:");
        Serial.print(count);
        Serial.print(" ret:");
        Serial.print(ret);
        Serial.print(" t:");
        Serial.print(micros()-tstart);
        Serial.println("us");

    }
    return ret;
}

int8_t ModbusMaster::readHolding(uint8_t unit, uint16_t regNo, int16_t count) {
    // clear pending data from the serial buffer.
    if ( count > MAX_COUNT ) {
        return MODBUS_TOOMANY;
    }
    unsigned long tstart = micros();
    consumeTraffic();
    frameLength = 0;
    numRegisters = 0;

    buffer[frameLength++] = unit;
    buffer[frameLength++] = 0x03;
    buffer[frameLength++] = 0xff&(regNo>>8);
    buffer[frameLength++] = 0xff&regNo;
    buffer[frameLength++] = 0xff&(count>>8);
    buffer[frameLength++] = 0xff&count;
    send();

    // expecting to recieve 5+count*2 bytes in the response
    // first read 2 bytes to 
    int8_t ret = readResponse(unit, 0x03);
    if ( diagnosticsEnabled ) {
        Serial.print("Modbus read holding unit:");
        Serial.print(unit);
        Serial.print(" reg:");
        Serial.print(regNo);
        Serial.print(" n:");
        Serial.print(count);
        Serial.print(" ret:");
        Serial.print(ret);
        Serial.print(" t:");
        Serial.print(micros()-tstart);
        Serial.println("us");
    }
    return ret;
}

int8_t ModbusMaster::writeHolding(uint8_t unit, uint16_t regNo, int16_t value) {
    // clear pending data from the serial buffer.
    consumeTraffic();
    frameLength = 0;
    numRegisters = 0;

    buffer[frameLength++] = unit;
    buffer[frameLength++] = 0x03;
    buffer[frameLength++] = 0xff&(regNo>>8);
    buffer[frameLength++] = 0xff&regNo;
    buffer[frameLength++] = 0xff&(value>>8);
    buffer[frameLength++] = 0xff&value;
    send();

    // expecting to recieve 5+count*2 bytes in the response
    // first read 2 bytes to 
    int8_t resp = readResponse(unit, 0x03, 8); 
    if(  resp == MODBUS_OK ) {
        if ( (buffer[0] == unit) && 
                (buffer[1] == 0x03) &&
                (buffer[2] == (0xff&(regNo>>8))) &&
                (buffer[3] == (0xff&regNo)) &&
                (buffer[4] == (0xff&(value>>8))) &&
                (buffer[5] == (0xff&value))) {
                return MODBUS_OK;
        } else {
            return MODBUS_FAIL;
        }
    } else {
        return resp;
    }
}

bool ModbusMaster::checkRegisterOffset(uint8_t offset) {
    if (offset > numRegisters ) {
        Serial.print("register ");
        Serial.print(offset);
        Serial.print(" unavailable:");
        Serial.println(offset);
        return false;
    } else if ((offset*2)+4 > frameLength) {
        Serial.print("frameLength too short required: ");
        Serial.print(((offset*2)+4));
        Serial.print(" unavailable:");
        Serial.println(frameLength);
        return false;
    } else {
        return true;
    }
}


uint16_t ModbusMaster::getUInt16Response(uint8_t offset) {
    if ( checkRegisterOffset(offset) ) {
        return (uint16_t) (0xff00&(buffer[(offset*2)+3]<<8)) | (0xff&(buffer[(offset*2)+4]));
    } else {
        return 0;
    }
}
int16_t ModbusMaster::getInt16Response(uint8_t offset) {
    if ( checkRegisterOffset(offset) ) {
        return (int16_t) (0xff00&(buffer[(offset*2)+3]<<8)) | (0xff&(buffer[(offset*2)+4]));
    } else {
        return 0;
    }
}

void ModbusMaster::consumeTraffic() {
    recieveEnable();
    if ( diagnosticsEnabled ) {
        if ( io->available() ) {
            Serial.print("Traffic:");
            while(io->available() ) {
                Serial.print(io->read(),HEX);
                Serial.print(' ');
            }
            Serial.println("");
        }
    } else {
        while(io->available() ) {
            io->read();
        }
    }

    transmitEnable();
}

void ModbusMaster::recieveEnable() {
    digitalWrite(enPin,LOW);
}
void ModbusMaster::transmitEnable() {
    digitalWrite(enPin,HIGH);
}

void ModbusMaster::dumpFrame(uint8_t length) {
    for(uint8_t i = 0; i < length; i++) {
        if (buffer[i] < 16) {
            Serial.print(" 0x0");
        } else {
            Serial.print(" 0x");
        }
        Serial.print(buffer[i],HEX);
    }
    Serial.print("\n");    
}


void ModbusMaster::send() {
    uint16_t crcv = crc16(&buffer[0],frameLength);
    buffer[frameLength++] = 0xff&(crcv>>8);
    buffer[frameLength++] = 0xff&(crcv);
    io->write(buffer, frameLength);
    io->flush();
    sent++;
    if ( diagnosticsEnabled ) {
        Serial.print("send>");
        dumpFrame(frameLength);
    }
    frameLength = 0;
}

int8_t ModbusMaster::readResponse(uint8_t unit, uint8_t function, int16_t len) {
    recieveEnable();
    io->setTimeout(5000);
    int8_t ret = MODBUS_OK;
    if ( io->readBytes(&buffer[0],2) != 2) {
        // timeout
        Serial.println("Coms Timeout reading 0-1");
        ret =  MODBUS_TIMEOUT;
    } else {
    // 
        if ( buffer[0] != unit) {
            // wrong unit responded, probably > 1 master on the bus.
            Serial.print("Wrong unit responded, expected:");
            Serial.print(unit);
            Serial.print(" got:");
            Serial.println(buffer[0]);
            dumpFrame(8);
            ret = MODBUS_WRONGUNIT;
        } else if ( (buffer[1] & 0x7f) != function ) {
            // response had wrong function code.
            Serial.println("Wrong function response, expected:");
            Serial.print(function);
            Serial.print(" got:");
            Serial.println(buffer[1]);
            ret = MODBUS_WRONGFUNCTION;
        } else if ( (buffer[1] & 0x80) == 0x80 ) {
            // exception, read code and CRC
            if ( io->readBytes(&buffer[2],3) == 3 ) {
                checkCrc(3);
                Serial.print("Exception: 0x");
                Serial.println(buffer[0]);
            } else {
                Serial.println("Coms Timeout reading exception byte 2");
            }
            frameLength = 3;
            ret = MODBUS_EXCEPTION;
        } else {
            if ( len != -1) {
                // fixed length response
                // 2 read already, 
                if ( io->readBytes(&buffer[2], len-2) == len-2) {
                    frameLength = len;
                    if ( !checkCrc(len) ) {
                        Serial.println("CRC Fail fixed length ");
                        ret =  MODBUS_CRCFAIL;
                    }
                } else {
                    Serial.println("Coms Timeout 2-len");
                    ret = MODBUS_TIMEOUT;
                }
            } else {
                // read the count
                if ( io->readBytes(&buffer[2],1) != 1 ) {
                    Serial.println("Coms Timeout count");
                    ret = MODBUS_TIMEOUT;
                } else {
                    // read bytes and the CRC
                    if ( io->readBytes(&buffer[3],buffer[2]+2) != buffer[2]+2)  {
                        Serial.println("Coms Timeout 3-count");
                        ret = MODBUS_TIMEOUT;
                    } else {
                        frameLength = buffer[2]+3;
                        numRegisters = buffer[2]/2;
                        if ( !checkCrc(buffer[2]+3)) {
                            Serial.println("CRC fail count len");

                            ret = MODBUS_CRCFAIL;
                        }
                    }
                }
            }
        }
    }
    transmitEnable();
    if ( diagnosticsEnabled ) {
        Serial.print("recv<");
        dumpFrame(frameLength);
    }
    if ( ret != MODBUS_OK ) {
        errors++;
    } else {
        recieved++;
    }
    return ret;


}


bool ModbusMaster::checkCrc(uint8_t p) {
    uint16_t crcv = crc16(&buffer[0],p);
    uint16_t crcr = (0xff00&(buffer[p]<<8)) | (0xff&buffer[p+1]);
    if (crcv != crcr ) {
        Serial.println("Warning: CRC Failed");
        return false;
    }
    return true;
}
    /**
    * @brief crc for mdbus, polynomial = 0x8005, reverse in, reverse out, init 0xffff;
    * 
    * @param array 
    * @param length 
    * @return uint16_t 
    */
uint16_t ModbusMaster::crc16(const uint8_t *array, uint16_t length) {
    uint16_t crc = 0xffff;
    while (length--) {
        if ((length & 0xFF) == 0) yield();  // RTOS
        uint8_t data = *array++;
        data = (((data & 0xAA) >> 1) | ((data & 0x55) << 1));
        data = (((data & 0xCC) >> 2) | ((data & 0x33) << 2));
        data =          ((data >> 4) | (data << 4));
        crc ^= ((uint16_t)data) << 8;
        for (uint8_t i = 8; i; i--) {
        if (crc & (1 << 15)) {
            crc <<= 1;
            crc ^= 0x8005;
        } else {
            crc <<= 1;
        }
        }
    }
    crc = (((crc & 0XAAAA) >> 1) | ((crc & 0X5555) << 1));
    crc = (((crc & 0xCCCC) >> 2) | ((crc & 0X3333) << 2));
    crc = (((crc & 0xF0F0) >> 4) | ((crc & 0X0F0F) << 4));
    //  crc = (( crc >> 8) | (crc << 8));
    //  crc ^= endmask;
    return crc;
}

