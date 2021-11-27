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


#define ESP32_CAN_TX_PIN GPIO_NUM_19
#define ESP32_CAN_RX_PIN GPIO_NUM_18
#define ONEWIRE_GPIO_PIN GPIO_NUM_21
#define SDA_PIN GPIO_NUM_23
#define SCL_PIN GPIO_NUM_22

// Calibrations to take account of resistor tollerances, this is specific to the board being flashed.
// should probably be in a config file eventually.



#include <NMEA2000_esp32.h>
#include <NMEA2000_CAN.h>

#include "listdevices.h"
#include "datadisplay.h"
#include "dataoutput.h"
#include "httpserver.h"
#include "temperature.h"
#include "bme280sensor.h"
#include "adc.h"
#include "display.h"



Stream *OutputStream = &Serial;
DataDisplay dataDisplay(OutputStream);
DataCollector dataCollector(OutputStream);
ListDevices listDevices(&NMEA2000, OutputStream);
EngineDataOutput engineDataOutput(&dataCollector);
BoatDataOutput boatDataOutput(&dataCollector);
NavigationDataOutput navigationDataOutput(&dataCollector);
EnvironmentDataOutput environmentDataOutput(&dataCollector);
TemperatureDataOutput temperatureDataOutput(&dataCollector);
Temperature temperature(ONEWIRE_GPIO_PIN);
BME280Sensor bme280Sensor(SDA_PIN, SCL_PIN);
ADCSensor adcSensor;

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
  OutputStream->println("  - Send 'u' to print latest list of devices");
  OutputStream->println("  - Send 'o' to toggle output, can be high volume");
  OutputStream->println("  - Send 'd' to toggle packet dump, can be high volume");
  OutputStream->println("  - Send '0' to '9' to preconfigured data packets");
  OutputStream->println("  - Send 'v' to print voltages");
  OutputStream->println("  - Send 'i' to Be prompted for IP settings");
  OutputStream->println("  - Send 'b' to change brightness");
  

}

void setup() {
  Serial.begin(115200); 
  temperature.begin();
  bme280Sensor.begin();
  adcSensor.begin();
  display.begin();

 
  webServer.addDataSet(0,&listDevices);
  webServer.addDataSet(1,&engineDataOutput);
  webServer.addDataSet(2,&boatDataOutput);
  webServer.addDataSet(3,&navigationDataOutput);
  webServer.addDataSet(4,&environmentDataOutput);
  webServer.addDataSet(5,&temperatureDataOutput);
  webServer.addDataSet(6,&temperature);  
  webServer.addDataSet(7,&bme280Sensor);  
  webServer.addDataSet(8,&adcSensor);  
  webServer.begin();

  display.addDisplayPage(&adcSensor);
  display.addDisplayPage(&bme280Sensor);


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



//*****************************************************************************
//NMEA 2000 message handler - should  be in a class so it can be attached.

//=================================================
// Diagnosis Functions and control

//*****************************************************************************

//*****************************************************************************
void CheckCommand() {
  static bool enableForward = false;
  if (OutputStream->available()) {
    char chr = OutputStream->read();
    switch ( chr ) {
      case '0': engineDataOutput.outputText(OutputStream); break;
      case '1': boatDataOutput.outputText(OutputStream); break;
      case '2': navigationDataOutput.outputText(OutputStream); break;
      case '3': environmentDataOutput.outputText(OutputStream); break;
      case '4': temperatureDataOutput.outputText(OutputStream); break;
      case 'h': showHelp(); break;
      case 'u': listDevices.list(true); break;
      case 'o': Serial.println("Output Toggle"); dataDisplay.showData = !dataDisplay.showData;  break;
      case 'd': Serial.println("Data Toggle");enableForward = !enableForward; NMEA2000.EnableForward(enableForward); break;
      case 'v': adcSensor.printVoltages(); break;
      case 'b': display.dim(); break;
    }
  }
}




//*****************************************************************************
void loop() { 
  NMEA2000.ParseMessages();
  listDevices.list();
  adcSensor.read();
  bme280Sensor.read();
  temperature.read();
  display.update();
  CheckCommand();
}
