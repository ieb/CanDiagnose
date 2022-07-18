
#include "display.h"
#include <Wire.h>
#define SSD1306_NO_SPLASH 1
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "splash.h"


void OledDisplay::begin() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, address)) {
    Serial.println(F("SSD1306 allocation failed"));
    return;
  }
  Serial.print(F("SSD1306 started"));Serial.println(address,HEX);
  drawPage(&display);

  addDisplayPage(this);

}



void OledDisplay::update() {
    unsigned long now = millis();
    switch(displayState) {
        case START_SLEEP:
            this->drawPage(&display);
            displayState = WAIT_SLEEP_END;
            sleepEndAt = now+4000;
            Serial.println("Display Sleep Process complete  in 4s");
            break;
        case WAIT_SLEEP_END:

            if ( now > sleepEndAt ) {
                display.clearDisplay();
                display.display();
                Serial.println("Display Sleep Process is complete");
                displayState = SLEEPING;
            }
            break;
        case SLEEPING:
            break;
        case START_WAKE:
            page = 0;
            staticPage = 0;
            Serial.println("Display Wake up");
            displayState = AWAKE;
            break;
        case AWAKE:
            if ( dimming ) {
                if ( now > lastDim + dimPeriod ) {
                    lastDim = now;
                    dim();
                }
            } else {
                if ( now > staticPagePress+15000 && now > lastDisplay + displayPeriod )  {
                    lastDisplay = now;
                    lastDim = now;
                    if ( displayPages[page]->drawPage(&display) ) {
                        page++;
                        if ( page == lastPage || displayPages[page] == NULL ) {
                            page = 0;
                        }
                    }
                }
            }
            break;
    }
}

void OledDisplay::nextPage() {
    staticPagePress = millis();
    if ( staticPages[staticPage]->drawPage(&display) ) {
        staticPage++;
        if ( staticPage == lastStaticPage || staticPages[staticPage] == NULL ) {
            staticPage = 0;
        }
    }
}
void OledDisplay::startDim() {
    dimming = true;
    update();
}
void OledDisplay::endDim() {
    dimming = false;
    update();
}
void OledDisplay::sleep() {
    if ( displayState == AWAKE ) {
        Serial.println("Display Setting state: Sleeping");
        displayState = START_SLEEP;
    }
}
void OledDisplay::wake() {
    if ( displayState == SLEEPING ) {
        Serial.println("Display Setting State: Wake");
        displayState = START_WAKE;
    }
}

void OledDisplay::dim() {
    if (dimmer == 0) {
        dimmer = 10;
    }
    dimmer--;
    display.ssd1306_command1(SSD1306_SETCONTRAST);
    display.ssd1306_command1(dimmer*25);
    display.clearDisplay();
    display.setCursor(52,20);              // Start at top-left corne
    display.setTextSize(3);   // 18x24, 4 rows, 10.6 chars
    display.printf("%d",dimmer); // ADC0 is service battery 
    display.display();

}

bool OledDisplay::drawPage(Adafruit_SSD1306 * display) {
    display->clearDisplay();
#if OLED_HEIGHT == 32
    switch(subPage) {
        case 0:

            display->drawBitmap((display->width() - spash12832_width) / 2, (display->height() - spash12832_height) / 2,
                        spash12832_data, spash12832_width, spash12832_height, 1);
            display->display();
            subPage  = 1;
            return false;
        case 1:
            display->drawBitmap((display->width() - luna_width) / 2, (display->height() - luna_height) / 2,
                        luna_data, luna_width, luna_height, 1);
            display->display();
            subPage  = 0;
            return true;
        default:
            subPage  = 0;
            return false;

    }
#else
  display->drawBitmap((display->width() - spash12864_width) / 2, (display->height() - spash12864_height) / 2,
               spash12864_data, spash12864_width, spash12864_height, 1);
  display->display();
  return true;
#endif
    
}



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

bool History128over24::drawHistory(Adafruit_SSD1306 * display, bool fixedRange, float fixedRangeMin, float fixedRangeMax ) {
    uint16_t hmax = history[0], hmin = history[0];
    if (fixedRange) {
        hmin = (uint16_t)(round((fixedRangeMin-offset)*scale));;
        hmax = (uint16_t)(round((fixedRangeMax-offset)*scale));;
    }
    startIterator();
    while(hasNext()) {
        uint16_t v = nextValueRaw();
        if ( v > hmax) { hmax = v;}
        if ( v < hmin) { hmin = v;}
    }
    uint16_t minRange = graphRangeMin*scale;
    if ( hmax < hmin+minRange) {
        if ( hmin < minRange/2 ) {
            hmin = 0;
            hmax = minRange;
        } else {
            hmin = hmin - minRange/2;
            hmax = hmax + minRange/2;
        }
    }
    float dscale = 64.0/(hmax-hmin);
    int y;
    int x=0;
    startIterator();
    while(hasNext()) {
        uint16_t v = nextValueRaw();
        y = round(dscale*(v-hmin));
        switch (historyDrawState) {
            case 0:
                display->drawPixel(x,64-y,SSD1306_WHITE);
                break;
            case 1:
                display->drawFastVLine(x,64-y,y,SSD1306_WHITE);
                break;
        }
        x++;
    }
    display->setTextSize(1);
    display->setTextColor(SSD1306_INVERSE);
    display->setCursor(0,0);
//    Serial.printf("%d %d %f %f %f %f\n", hmin, hmax, offset+(hmin/scale), offset+(hmax/scale), offset, scale);
    display->printf("max:%6.1f", offset+(hmax/scale));
    display->setCursor(0,64-7);
    display->printf("min:%6.1f %s", offset+(hmin/scale), title);
    // Axis
    display->drawFastVLine(64,0,64,SSD1306_INVERSE);
    display->setCursor(65,0);
    if ( historyPeriod > 112500) { // 2h
        display->printf("%ldh", (historyPeriod*64)/3600000);
    } else if (historyPeriod > 1875 ) { //2m
        display->printf("%ldm", (historyPeriod*64)/60000);
    } else { // seconds
        display->printf("%lds", (historyPeriod*64)/1000);
    }

    switch (historyDrawState) {
        case 0:
            historyDrawState = 1;
            return false;
        default:
            historyDrawState = 0;
            return true;
    }
}