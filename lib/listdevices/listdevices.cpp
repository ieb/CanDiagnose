#include "listdevices.h"
#include "httpserver.h"


ListDevices::ListDevices(tNMEA2000 *_pNMEA2000, Stream *outputStream) : 
        tN2kDeviceList(_pNMEA2000),
        OutputStream{outputStream} {
}


void ListDevices::printUlongList(const char *prefix, const unsigned long * List) {
  uint8_t i;
  if ( List!=0 ) {
    OutputStream->print(prefix);
    for (i=0; List[i]!=0; i++) { 
      if (i>0) OutputStream->print(", ");
      OutputStream->print(List[i]);
    }
    OutputStream->println();
  }
}


//*****************************************************************************
void ListDevices::printText(const char *Text, bool AddLineFeed) {
  if ( Text!=0 ) OutputStream->print(Text);
  if ( AddLineFeed ) OutputStream->println();   
}

void ListDevices::outputJson(AsyncResponseStream *outputStream) {  
  startJson(outputStream);
  startArray("devices");
  for (uint8_t i = 0; i < N2kMaxBusDevices; i++)  {
    const tNMEA2000::tDevice *pDevice = FindDeviceBySource(i);
    if ( pDevice != 0) {
      startObject();
      append("source",pDevice->GetSource());
      append("mcode",pDevice->GetManufacturerCode());
      append("id",pDevice->GetUniqueNumber());
      append("swcode",pDevice->GetSwCode());
      append("version",pDevice->GetModelVersion());
      append("info",pDevice->GetManufacturerInformation());
      append("desc1",pDevice->GetInstallationDescription1());
      append("desc2",pDevice->GetInstallationDescription2());
      startArray("tpgn");
      const unsigned long *tpgn = pDevice->GetTransmitPGNs();
      while(tpgn[0]!=0) {
        append(tpgn[0]);
        tpgn++;
      }
      endArray();
      startArray("rpgn");
      const unsigned long *rpgn = pDevice->GetReceivePGNs();
      while(rpgn[0]!=0) {
        append(rpgn[0]);
        rpgn++;
      }
      endArray();
      endObject();
    }
  }
  endArray();
  endJson();
}

//*****************************************************************************
void ListDevices::printDevice(const tNMEA2000::tDevice *pDevice) {
  if ( pDevice == 0 ) return;

  OutputStream->println("----------------------------------------------------------------------");
  OutputStream->println(pDevice->GetModelID());
  OutputStream->print("  Source: "); OutputStream->println(pDevice->GetSource());
  OutputStream->print("  Manufacturer code:        "); OutputStream->println(pDevice->GetManufacturerCode());
  OutputStream->print("  Unique number:            "); OutputStream->println(pDevice->GetUniqueNumber());
  OutputStream->print("  Software version:         "); OutputStream->println(pDevice->GetSwCode());
  OutputStream->print("  Model version:            "); OutputStream->println(pDevice->GetModelVersion());
  OutputStream->print("  Manufacturer Information: "); printText(pDevice->GetManufacturerInformation());
  OutputStream->print("  Installation description1: "); printText(pDevice->GetInstallationDescription1());
  OutputStream->print("  Installation description2: "); printText(pDevice->GetInstallationDescription2());
  printUlongList("  Transmit PGNs :",pDevice->GetTransmitPGNs());
  printUlongList("  Receive PGNs  :",pDevice->GetReceivePGNs());
  OutputStream->println();
}

#define START_DELAY_IN_S 8
//*****************************************************************************
void ListDevices::list(bool force) {
  static bool StartDelayDone=false;
  static int StartDelayCount=0;
  static unsigned long NextStartDelay=0;
  if ( !StartDelayDone ) { // We let system first collect data to avoid printing all changes
    if ( millis()>NextStartDelay ) {
      if ( StartDelayCount==0 ) {
        OutputStream->print("Reading device information from bus ");
        NextStartDelay=millis();
      }
      OutputStream->print(".");
      NextStartDelay+=1000;
      StartDelayCount++;
      if ( StartDelayCount>START_DELAY_IN_S ) {
        StartDelayDone=true;
        OutputStream->println();
      }
    }
    return;
  }
  if ( !force && !ReadResetIsListUpdated() ) return;

  OutputStream->println();
  OutputStream->println("**********************************************************************");
  for (uint8_t i = 0; i < N2kMaxBusDevices; i++) printDevice(FindDeviceBySource(i));
}
