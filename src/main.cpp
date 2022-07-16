// Demo: NMEA2000 library. 
// This demo reads messages from NMEA 2000 bus and
// sends them translated to clear text to Serial.

// Note! If you use this on Arduino Mega, I prefer to also connect interrupt line
// for MCP2515 and define N2k_CAN_INT_PIN to related line. E.g. MessageSender
// sends so many messages that Mega can not handle them. If you only use
// received messages internally without slow operations, then youmay survive
// without interrupt.

#include <Arduino.h>
#include <Wire.h>


// Pins   
#define ESP32_CAN_RX_PIN GPIO_NUM_22
#define ESP32_CAN_TX_PIN GPIO_NUM_23
#define ONEWIRE_GPIO_PIN GPIO_NUM_21
#define SDA_PIN GPIO_NUM_18
#define SCL_PIN GPIO_NUM_5
#define DISPLAY_BUTTON GPIO_NUM_19

// RS584 on Serial2 TX
#define RS485_TX GPIO_NUM_17
#define RS485_RX GPIO_NUM_16
#define RS485_EN GPIO_NUM_4

// SPI Display impl TBD
#define SPI_DISPLAY_MOSI GPIO_NUM_32
#define SPI_DISPLAY_SCK GPIO_NUM_33
#define SPI_DISPLAY_CS GPIO_NUM_25
#define SPI_DISPLAY_DC GPIO_NUM_26
#define SPI_DISPLAY_RST GPIO_NUM_12
#define SPI_DISPLAY_BL GPIO_NUM_13

// Calibrations to take account of resistor tollerances, this is specific to the board being flashed.
// should probably be in a config file eventually.



#include <NMEA2000_esp32.h>

// The method recomended doesnt work well so explicitly create the NMEA2000 objects to be sure
// the pins are correct. I've had loads of problems with this depening 
// on how the libraries are created.

tNMEA2000 &NMEA2000=*(new tNMEA2000_esp32(ESP32_CAN_TX_PIN, ESP32_CAN_RX_PIN));


// #include <NMEA2000_CAN.h>

#include "listdevices.h"
#include "datadisplay.h"
#include "dataoutput.h"
#include "httpserver.h"
#include "temperature.h"
#include "display.h"
#include "logbook.h"
#include "modbus.h"



Stream *OutputStream = &Serial;
DataDisplay dataDisplay(OutputStream);
DataCollector dataCollector(OutputStream);
ListDevices listDevices(&NMEA2000, OutputStream);
EngineDataOutput engineDataOutput(dataCollector);
BoatDataOutput boatDataOutput(dataCollector);
NavigationDataOutput navigationDataOutput(dataCollector);
EnvironmentDataOutput environmentDataOutput(dataCollector);
TemperatureDataOutput temperatureDataOutput(dataCollector);
XteDataOutput xteDataOutput(dataCollector);
MagneticVariationDataOutput magneticVariationDataOutput(dataCollector);
WindSpeedDataOutput windSpeedDataOutput(dataCollector);
LogDataOutput logDataOutput(dataCollector);
LatLonDataOutput latLonDataOutput(dataCollector);
LeewayDataOutput leewayDataOutput(dataCollector);


Temperature temperature(&NMEA2000, ONEWIRE_GPIO_PIN);


ModbusMaster modbusMaster(&Serial2, RS485_EN);
Modbus modbus(&NMEA2000, modbusMaster);



LogBook logbook(dataCollector);


WebServer webServer(OutputStream);
OledDisplay display;




void HandleNMEA2000Msg(const tN2kMsg &N2kMsg) {
    listDevices.HandleMsg(N2kMsg);
    dataDisplay.HandleMsg(N2kMsg);
    dataCollector.HandleMsg(N2kMsg);
}

void showHelp() {
  OutputStream->println("Device analyzer started.");
  OutputStream->println("  - Analyzer will automatically print new list on list changes.");
  OutputStream->println("  - Send 'h' to show this message");
  OutputStream->println("  - Send 's' to show status");
  OutputStream->println("  - Send 'u' to print latest list of devices");
  OutputStream->println("  - Send 'o' to toggle output, can be high volume");
  OutputStream->println("  - Send 'd' to toggle packet dump, can be high volume");
  OutputStream->println("  - Send 'm' to toggle modbus diagnostics");
  OutputStream->println("  - Send 'b' to change brightness");
}


void setup() {
  Serial.begin(115200); 
  Serial2.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX); // RS485 Modbus
  modbusMaster.begin();

  if (!Wire.begin(SDA_PIN, SCL_PIN) ) {
    Serial.println("I2C failed to start");
  } else {
    Serial.println("I2C scanning ");
    for (uint8_t addr = 0; addr < 255; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Serial.print(" ");
            Serial.print(addr,HEX);
        } else {
            Serial.print(" --");
        }
    }
    Serial.println("Done ");
  }

  display.begin();




  pinMode(DISPLAY_BUTTON, INPUT_PULLUP);
  temperature.begin();
  modbus.begin();
 
  webServer.addJsonOutputHandler(0,&listDevices);
  webServer.addJsonOutputHandler(1,&engineDataOutput);
  webServer.addJsonOutputHandler(2,&boatDataOutput);
  webServer.addJsonOutputHandler(3,&navigationDataOutput);
  webServer.addJsonOutputHandler(4,&environmentDataOutput);
  webServer.addJsonOutputHandler(5,&temperatureDataOutput);
  webServer.addJsonOutputHandler(6,&temperature);  
  webServer.addJsonOutputHandler(8,&modbus);
  webServer.addJsonOutputHandler(9,&xteDataOutput);  
  webServer.addJsonOutputHandler(10,&magneticVariationDataOutput);  
  webServer.addJsonOutputHandler(11,&logDataOutput);  
  webServer.addJsonOutputHandler(12,&latLonDataOutput);  
  webServer.addJsonOutputHandler(13,&leewayDataOutput);  

  webServer.addCsvOutputHandler(0,&listDevices);
  webServer.addCsvOutputHandler(1,&engineDataOutput);
  webServer.addCsvOutputHandler(2,&boatDataOutput);
  webServer.addCsvOutputHandler(3,&navigationDataOutput);
  webServer.addCsvOutputHandler(4,&environmentDataOutput);
  webServer.addCsvOutputHandler(5,&temperatureDataOutput);
  webServer.addCsvOutputHandler(6,&temperature);  
  webServer.addCsvOutputHandler(6,&modbus);  
  webServer.addCsvOutputHandler(9,&xteDataOutput);  
  webServer.addCsvOutputHandler(10,&magneticVariationDataOutput);  
  webServer.addCsvOutputHandler(11,&logDataOutput);  
  webServer.addCsvOutputHandler(12,&latLonDataOutput);  
  webServer.addCsvOutputHandler(13,&leewayDataOutput);  
  webServer.begin();

  display.addDisplayPage(&windSpeedDataOutput);
  display.addDisplayPage(&latLonDataOutput);
  display.addDisplayPage(&logDataOutput);
  display.addDisplayPage(&modbus);  
  display.addDisplayPage(&engineDataOutput);
  display.addDisplayPage(&webServer);

  // Set Product information
  NMEA2000.SetProductInformation("00000003", // Manufacturer's Model SerialIO code
                                 100, // Manufacturer's product code
                                 "N2k bus device analyzer",  // Manufacturer's Model ID
                                 "1.0.0.10 (2017-07-29)",  // Manufacturer's Software version code
                                 "1.0.0.0 (2017-07-12)" // Manufacturer's Model version
                                );

  // Set device information
  NMEA2000.SetDeviceInformation(5, // Unique number. Use e.g. SerialIO number.
                                130, // Device function=Display. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                120, // Device class=Display. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );


//  NMEA2000.SetN2kCANReceiveFrameBufSize(50);
  // Do not forward bus messages at all
  NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);
  NMEA2000.SetForwardStream(OutputStream);
  // Set false below, if you do not want to see messages parsed to HEX withing library
  NMEA2000.EnableForward(false);
  NMEA2000.SetN2kCANReceiveFrameBufSize(150);
  NMEA2000.SetN2kCANMsgBufSize(8);

  NMEA2000.SetMsgHandler(HandleNMEA2000Msg);

  NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode, 50);
  NMEA2000.Open();

  OutputStream->print("Running...");
  showHelp();
  
}

//*****************************************************************************

void showStatus() {
  modbus.readStats();
}

//*****************************************************************************
//NMEA 2000 message handler - should  be in a class so it can be attached.

//=================================================
// Diagnosis Functions and control

//*****************************************************************************

//*****************************************************************************
void CheckCommand() {
  static bool enableForward = false;
  static bool modbusDiagnose = false;
  if (OutputStream->available()) {
    char chr = OutputStream->read();
    switch ( chr ) {
      case 'h': showHelp(); break;
      case 'u': listDevices.list(true); break;
      case 's':
        showStatus();
        break;
      case 'o': 
        dataDisplay.showData = !dataDisplay.showData;
        if (  dataDisplay.showData ) {
          Serial.println("Data Output Enabled");   
        } else {
          Serial.println("Data Output Disabled");   
        }
        break;
      case 'd': 
        enableForward = !enableForward;
        if (  enableForward ) {
          Serial.println("NMEA2000 Packet Output Enabled");   
        } else {
          Serial.println("NMEA2000 Packet Output Disabled");   
        }
        NMEA2000.EnableForward(enableForward); 
        break;
      case 'm':
        modbusDiagnose = !modbusDiagnose;
        if (  modbusDiagnose ) {
          Serial.println("Modbus Diagnostics enabled");   
        } else {
          Serial.println("Modbus Diagnostics disabled");   
        }
        modbus.setDiagnostics(modbusDiagnose);
        break;

      case 'b': display.dim(); break;
    }
  }
}

void onPress() {
  display.nextPage();
}
void onLongRelease() {
  display.endDim();
}
void onLongPress() {
  display.startDim();
}

long checkPress() {
  static bool  pressed = false;
  static uint8_t bits = 0;
  static unsigned long lastCalled = 0, startPress = 0;
  unsigned long now = millis();


  // called every 100ms
  if ( now > lastCalled+10) {
    lastCalled = now;

    int dr = digitalRead(DISPLAY_BUTTON);
    if (  dr == HIGH ) {
      bits = bits<<1 & 0xfe;
    } else {
      bits = bits<<1 | 1;
    }
    // count the bits

    // Serial.println(bits,BIN);

    if (pressed && ((bits&0b00000111) == 0)  ) {
      // released for the last 150ms
      pressed = false;
      if ( now > (startPress+3000) ) {
        onLongRelease();
      } else {
        onPress();
      }
    } else if ( !pressed && ((bits&0b00000111) == 0b00000111) ) {
      // touched for the last 300ms
      pressed = true;
      startPress = now;
    } 
    if ( pressed && (now > (startPress+3000))) {
      onLongPress();
    }
  }
  return startPress; 
}



//*****************************************************************************
void loop() { 
  NMEA2000.ParseMessages();
  listDevices.list();
  temperature.read();
  temperature.output();
  modbus.read();
  modbus.output();

  logbook.log();
//  logbook.demoMode();
  CheckCommand();
  if ( checkPress() + 60000 > millis() ) {
    display.wake();
  } else {
    display.sleep();
  }
  display.update();
}
