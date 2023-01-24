
#include "N2KCollector.h"

void History128over24::storeHistory(double v) {
    unsigned long now = millis();
    int slot = index(now); // 24h one value every 675s
    if (slot != currentSlot) {
        pmean = v;
        currentSlot = slot;
    } else {
        pmean = pmean*0.9+v*0.1;
    }
    history[slot] = (uint16_t)(round((pmean-offset)*scale));

};
float History128over24::changeSince(unsigned long tms) {
    unsigned long now = millis();
    if ( tms > now ) { // no data
        return 0;
    }
    uint8_t slot = index(now);
    uint8_t pslot = index((now-tms));
    return (history[slot]-history[pslot])/scale;

};

void History128over24::startIterator() {
    hend = (millis()/historyPeriod)+1;
    iv = hend-History128over24::historyLength; 
    if ( iv < 0 ) {
        iv = 0;
    }
//    Serial.printf("Iterating %d %d \n",iv,hend);
}
bool History128over24::hasNext() {
    return iv < hend;
};
float History128over24::nextValue() {
    return offset+(nextValueRaw()/scale);
}
uint16_t History128over24::nextValueRaw() {
    uint16_t cv = history[iv%History128over24::historyLength];
    iv++;
    return cv;
}
