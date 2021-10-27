// Demo: NMEA2000 library. 
// This demo reads messages from NMEA 2000 bus and
// sends them translated to clear text to Serial.

// Note! If you use this on Arduino Mega, I prefer to also connect interrupt line
// for MCP2515 and define N2k_CAN_INT_PIN to related line. E.g. MessageSender
// sends so many messages that Mega can not handle them. If you only use
// received messages internally without slow operations, then youmay survive
// without interrupt.

#include <Arduino.h>

#define ESP32_CAN_TX_PIN GPIO_NUM_19
#define ESP32_CAN_RX_PIN GPIO_NUM_18
#define N2k_CAN_INT_PIN 21


//#define N2k_SPI_CS_PIN 53    // Pin for SPI select for mcp_can
//#define N2k_CAN_INT_PIN 21   // Interrupt pin for mcp_can
//#define USE_MCP_CAN_CLOCK_SET 8  // Uncomment this, if your mcp_can shield has 8MHz chrystal
//#define ESP32_CAN_TX_PIN GPIO_NUM_16 // Uncomment this and set right CAN TX pin definition, if you use ESP32 and do not have TX on default IO 16
//#define ESP32_CAN_RX_PIN GPIO_NUM_17 // Uncomment this and set right CAN RX pin definition, if you use ESP32 and do not have RX on default IO 4
//#define NMEA2000_ARDUINO_DUE_CAN_BUS tNMEA2000_due::CANDevice1    // Uncomment this, if you want to use CAN bus 1 instead of 0 for Arduino DUE
#include <NMEA2000_esp32.h>
#include <NMEA2000_CAN.h>

#include "listdevices.h"
#include "datadisplay.h"


//#define BLUETOOTHCLASSIC 1
#ifdef BLUETOOTHCLASSIC
#include <BlueToothSerial.h>
BluetoothSerial SerialBT;
#endif


ListDevices *pListDevices;
DataDisplay *pDataDisplay;




Stream *OutputStream;

void HandleNMEA2000Msg(const tN2kMsg &N2kMsg) {
    pListDevices->HandleMsg(N2kMsg);
    pDataDisplay->HandleMsg(N2kMsg);
}


void setup() {
#ifdef BLUETOOTHCLASSIC
  SerialBT.begin("CanAnalyzer"); delay(500);
  OutputStream=&SerialBT;
#else
  Serial.begin(115200); delay(500);
  OutputStream=&Serial;
#endif
//   while (!Serial) 


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

  pListDevices = new ListDevices(&NMEA2000, OutputStream);
  pDataDisplay = new DataDisplay(&NMEA2000, OutputStream);
  NMEA2000.SetMsgHandler(HandleNMEA2000Msg);

  NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode, 50);
  NMEA2000.Open();
  OutputStream->print("Running...");
  OutputStream->println("Device analyzer started.");
  OutputStream->println("  - Analyzer will automatically print new list on list changes.");
  OutputStream->println("  - Send 'u' to print latest list");
  OutputStream->println("  - Send 'o' to toggle output, can be high volume");
  OutputStream->println("  - Send 'd' to toggle packet dump, can be high volume");
  
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
      case 'u': pListDevices->list(true); break;
      case 'o': Serial.println("Output Toggle"); pDataDisplay->showData = !pDataDisplay->showData;  break;
      case 'd': Serial.println("Data Toggle");enableForward = !enableForward; NMEA2000.EnableForward(enableForward); break;
    }
  }
}



//*****************************************************************************
void loop() { 
  NMEA2000.ParseMessages();
  pListDevices->list();
  CheckCommand();
}
