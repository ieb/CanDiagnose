
#include "OLEDDisplay.h"
#include <Wire.h>
#define SSD1306_NO_SPLASH 1
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "splash.h"
#include <TimeLib.h>
#include <SPIFFS.h>



void OledDisplay::begin() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, address)) {
    Serial.println(F("SSD1306 allocation failed"));
    return;
  }
  Serial.print(F("SSD1306 started"));Serial.println(address,HEX);
  update();
}



void OledDisplay::update() {
    unsigned long now = millis();
    switch(displayState) {
        case START_SLEEP:
            logoDisplayPage.drawPage(&display);
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
                    nextPage();
            }
            break;
       }
    }
}

void OledDisplay::nextPage() {
    staticPagePress = millis();
    bool advance = false;
    switch(page) {
        case 0:
            advance = logoDisplayPage.drawPage(&display);
            break; 
        case 1: 
            advance = windDisplayPage.drawPage(&display, &n2kCollector); 
            break;
        case 2:
            advance = environmentDisplayPage.drawPage(&display, &n2kCollector); 
            break;
        case 3:
            advance = navigationDisplayPage.drawPage(&display, &n2kCollector); 
            break;
        case 4:
            advance = latLongDisplayPage.drawPage(&display, &n2kCollector); 
            break;
        case 5:
            advance = logDisplayPage.drawPage(&display, &n2kCollector); 
            break;
        case 6:
            advance = modbusDisplayPage.drawPage(&display, &modbus); 
            break;
        case 7:
            advance = engineDisplayPage.drawPage(&display, &n2kCollector); 
            break;
        case 8:
            advance = webServerDisplayPage.drawPage(&display, &webServer); 
            break;
    }
    if ( advance ) {
        page++;
        if ( page >= lastPage ) {
            page = 0;
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

bool LogoDisplayPage::drawPage(Adafruit_SSD1306 * display) {
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



bool History128over24DisplayPage::drawHistory(Adafruit_SSD1306 * display, History128over24 * history128over24, bool fixedRange, float fixedRangeMin, float fixedRangeMax ) {
    uint16_t hmax = history128over24->getFirstRawValue();
    uint16_t hmin = hmax;
    if (fixedRange) {
        hmin = history128over24->getRawValueFor(fixedRangeMin);
        hmax = history128over24->getRawValueFor(fixedRangeMax); 
    }
    history128over24->startIterator();
    while(history128over24->hasNext()) {
        uint16_t v = history128over24->nextValueRaw();
        if ( v > hmax) { hmax = v;}
        if ( v < hmin) { hmin = v;}
    }
    uint16_t minRange = history128over24->getRawScaleFor(graphRangeMin);
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
    history128over24->startIterator();
    while(history128over24->hasNext()) {
        uint16_t v = history128over24->nextValueRaw();
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
    display->printf("max:%6.1f", history128over24->getRealValue(hmax));
    display->setCursor(0,64-7);
    display->printf("min:%6.1f %s", history128over24->getRealValue(hmin), title);
    // Axis
    display->drawFastVLine(64,0,64,SSD1306_INVERSE);
    display->setCursor(65,0);
    if ( history128over24->getPeriodMs() > 112500) { // 2h
        display->printf("%ldh", (history128over24->getPeriodMs()*64)/3600000);
    } else if (history128over24->getPeriodMs() > 1875 ) { //2m
        display->printf("%ldm", (history128over24->getPeriodMs()*64)/60000);
    } else { // seconds
        display->printf("%lds", (history128over24->getPeriodMs()*64)/1000);
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







bool  EngineDisplayPage::drawPage(Adafruit_SSD1306 * display, N2KCollector * n2kCollector) {
    
    EngineData *engine = n2kCollector->getEngineInstance(0);
    FluidLevelData *fluidLevel = n2kCollector->getFluidLevelInstance(0);

    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    switch(subPage) {
        case 0:
            display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
            display->setCursor(0,0);
            if ( engine == NULL) {
                display->printf(" ---- RPM\n"); // RPM
                display->printf("  --- C\n"); // Coolant temperatire
                display->printf("--.-- V a\n"); // Alternator Voltage
                display->printf("---.- h\n"); // Engine Hours
            }  else {
                //  1234 RPM
                //   123 C
                // 14.23
                display->printf(" %4.0f RPM\n",engine->speed); // RPM
                display->printf("  %3.0f C\n", engine->CoolantTemp-273.0); // Coolant temperatire
                display->printf("%5.2f V\n", engine->AltenatorVoltage); // Alternator Voltage
                display->printf("%5.1f h\n", engine->Hours); // Engine Hours
            }
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
            display->setCursor(0,0);
            if ( fluidLevel == NULL) {
                display->printf("Fuel\n--- %%\n"); // Fuel %
            } else {
                display->printf("Fuel\n%3.0f %%\n",fluidLevel->level); // Fuel %
            }
            display->display();
            subPage = 0;
            return true;
        default:
            subPage = 0;
            return false;
    }
#endif

}

bool  NavigationDisplayPage::drawPage(Adafruit_SSD1306 * display, N2KCollector * n2kCollector) {
    CogSogData *cogSog =  n2kCollector->getCogSog();



    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
    if ( cogSog == NULL ||  cogSog->cog < -100000 ) {
        display->setCursor(0,12);              
        display->printf("COG: -\n"); 
        display->setCursor(0,36);             
        display->printf("SOG: -\n"); 
    } else {
        display->setCursor(0,12);              
        display->printf("COG:%3.0f\n", RadToDeg(cogSog->cog)); 
        display->setCursor(0,36);             
        display->printf("SOG:%5.1f\n", msToKnots(cogSog->sog)); 
    }
    display->display();
    return true;
#endif

}

bool WindDisplayPage::drawPage(Adafruit_SSD1306 * display, N2KCollector * n2kCollector) {
    WindData *wind = n2kCollector->getWindInstance(0);


    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
    if ( wind == NULL ) {
        display->setCursor(0,12);              
        display->printf("AWS: -\n"); // AWS
        display->setCursor(0,36);             
        display->printf("AWA: -\n"); // AWA
    } else {
        display->setCursor(0,12);              
        display->printf("AWS:%5.1f\n", msToKnots(wind->windSpeed)); // AWS
        display->setCursor(0,36);             
        if ( wind->windAngle < 0 ) {
            display->printf("AWA:%3.0fP\n", RadToDeg(wind->windAngle)); // AWA
        }  else {
            display->printf("AWA:%3.0fS\n", RadToDeg(wind->windAngle)); // Wind
        }
    }
    display->display();
    return true;
#endif

}


bool EnvironmentDisplayPage::drawPage(Adafruit_SSD1306 * display, N2KCollector * n2kCollector) {
    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
    PressureData *pressure = &n2kCollector->pressure[0];
    TemperatureData *temperatureExt = &n2kCollector->temperatureExt[0];
    HumidityData *humidity = &n2kCollector->humidity[0];

#if OLED_HEIGHT == 32
    switch(subPage) {
        case 0:
            if ( pressure->source != 255 && pressure->lastModified > 0 ) {
                display->setTextSize(2);   // 18x24  128/24=7 chars 5,3 
                display->printf("%6.1f mB\n", PascalTomBar(pressure->actual));
                display->printf("%4.1f C\n", KelvinToC(temperatureExt->actual)); 
                display->printf("%5.1f %%RH\n", humidity->actual ); 
                display->display();
                subPage = 1;
                return false;
            } else {
                display->setTextSize(2);   // 18x24  128/24=7 chars 5,3 
                display->printf("----.- mB\n");
                display->printf("--.- C\n"); 
                display->printf("---.- %%RH\n" ); 
                display->display();
                subPage = 0;
                return true;
            }
        case 1:
            if ( drawHistory(display,pressure,false,970.0,1030.0) ) {
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

            display->setTextSize(2);   // 18x24  128/24=7 chars 5,3 
            display->setCursor(0,4);  
            display->printf("%6.1f mB\n", PascalTomBar(pressure->actual));
            display->printf("%4.1f C\n", KelvinToC(temperatureExt->actual)); 
            display->printf("%5.1f %%RH\n", humidity->actual ); 
            display->display();
            subPage = 1;
            return false;
        case 1:
            if ( drawHistory(display,pressure,false,970.0,1030.0) ) {
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


bool LogDisplayPage::drawPage(Adafruit_SSD1306 * display, N2KCollector * n2kCollector) {
    LogData *log = n2kCollector->getLog();
    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
    display->setCursor(0,12);   
    if ( log == NULL) {
        display->printf("L:----.-Nm\n"); // Log
        display->setCursor(0,36);             
        display->printf("T:----.-Nm\n"); // Trip
    } else {
        display->printf("L:%6.1fNm\n", log->log/1852.0); // Log
        display->setCursor(0,36);             
        display->printf("T:%6.1fNm\n", log->tripLog/1852.0); // Trip
    }
    display->display();
    return true;
#endif

}


bool LatLongDisplayPage::drawPage(Adafruit_SSD1306 * display, N2KCollector * n2kCollector) {



    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
#else
    display->setTextSize(2);   // 12x16, 4 rows, 10.6 chars
    display->setCursor(0,12);
    display->cp437(true);         // Use full 256 char 'Code Page 437' font
    double latitude,longitude;
    int16_t age;
    GnssData * gnss;
    
    switch(subPage) {
        case 0:
            if (!n2kCollector->getLatLong(latitude, longitude, age)) {
                display->printf(" --%c--.--N\n", 248); 
                display->printf("---%c--.--E\n", 248); 
            } else {
                int deg = (int) latitude;
                double min = (double) 60.0*(latitude-deg);
                if ( deg >= 0 && min >= 0) {
                    display->printf(" %02d%c%05.2fN\n", deg, 248, min); 
                } else {
                    display->printf(" %02d%c%05.2fS\n", -deg, 248, -min); 
                }
                deg = (int) longitude;
                min = (double) 60.0*(longitude-deg);
                display->setCursor(0,36);             
                if ( deg >= 0 && min >= 0) {
                    display->printf("%03d%c%05.2fE\n", deg,248,min); 
                } else {
                    display->printf("%03d%c%05.2fW\n", -deg,248,-min); 
                }
                display->setTextSize(1);   // 12x16, 4 rows, 10.6 chars
                if ( age < 600 ) {
                    display->printf("%ds\n", age); 
                } else if (age < 7200 ) {
                    display->printf("%dm\n", age/60); 
                } else {
                    display->printf("%dn\n", age/3600); 
                }
            }
            display->cp437(false);         
            display->display();
            subPage = 1;
            return false;
        case 1:
            gnss = n2kCollector->getGnss();
            if ( gnss != NULL ) {
                age = (millis() - gnss->lastModified)/1000;
                uint16_t daySerial = gnss->daysSince1970; 
                double seconds = gnss->secondsSinceMidnight;
                tmElements_t tm;
                // docu
                double tofLog = seconds+daySerial*SECS_PER_DAY; 
                breakTime((time_t)tofLog, tm);
                display->printf("%04d-%02d-%02d\n", tm.Year+1970, tm.Month, tm.Day);
                display->printf("%02d:%02d:%02dZ\n", tm.Hour,tm.Minute,tm.Second);
                display->setTextSize(1);   // 12x16, 4 rows, 10.6 chars
                display->printf("%dZ\n", age);

            } else {
                display->printf("no gps\n");
            }
            display->display();
            subPage = 0;
            return true;
        default:
            subPage = 0;
            return true;
    }

#endif

}


bool WebServerDisplayPage::drawPage(Adafruit_SSD1306 * display, WebServer * webServer) {

    display->clearDisplay();
    display->setTextSize(1);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
    switch(subPage) {
        case 0:
            display->setTextSize(1);   // 21 chars, 4 lines
            display->print("SSID:");display->println(webServer->getSSID());
            if ( webServer->getSSID().equals("boatsys")) {
                display->print("Password:");display->println(webServer->getPassword());
            } 
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->setTextSize(1);   // 21 chars, 4 lines
            display->print("IP:");display->println(WiFi.localIP());
            display->print("Auth:");display->println(webServer->getBasicAuth());


            display->display();
            subPage = 0;
            return true;
        default:
            subPage = 0;
            return false;
    }
#else
    // no subpage required
    display->setTextSize(1);   // 12x16, 4 rows, 10.6 chars
    display->print("SSID:");display->println(webServer->getSSID());
    if ( webServer->getSSID().equals("boatsys")) {
        display->print("Password:");display->println(webServer->getPassword());
    } 
    display->print("IP:");display->println(WiFi.localIP());
    display->print("Auth:");display->println(webServer->getBasicAuth());
    display->print("Free Heap:");display->println(ESP.getFreeHeap());
    display->printf("Fs:%d/%d\n",SPIFFS.usedBytes(), SPIFFS.totalBytes());

    display->display();
    return true;
#endif
}



bool ModbusDisplayPage::drawPage(Adafruit_SSD1306 * display, Modbus *modbus) {
    display->clearDisplay();
    display->setTextSize(2);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
    ModbusShunt * engineBattery = modbus->getEngineBattery();
    ModbusShunt * serviceBattery = modbus->getServiceBattery();
    
#if OLED_HEIGHT == 32
    switch(subPage) {
        case 0:
            display->setTextSize(2);   // 18x24  128/24=7 chars 5,3 
            if (serviceBattery->isEnabled() ) {
                display->printf("%5.2f V Sv\n", serviceBattery->voltage); 
                display->printf("%5.1f A\n", serviceBattery->current); 
            } else {
                display->printf("--.-- V Sv\n"); 
                display->printf("---.- A\n"); 

            }
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->setTextSize(2);
            if (engineBattery->isEnabled() ) {
                display->printf("%5.2f V Ev\n", engineBattery->voltage); 
                display->printf("%5.1f A\n", engineBattery->current); 
            } else {
                display->printf("--.-- V Ev\n"); 
                display->printf("---.- A\n"); 

            }
            display->display();
            subPage = 2;
            return false;
        case 2:
            if ( drawHistory(display, modbus, true,10.0,15.0) ) {
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

            if ( engineBattery->isEnabled() ) {
                display->printf("%5.2f V Ev\n", engineBattery->voltage);
            } else {
                display->printf("--.-- V Ev\n");
            }
            display->setCursor(0,24); // 4+16+4
            if ( serviceBattery->isEnabled() ) {
                display->printf("%5.2f V Sv\n", serviceBattery->voltage);
                display->setCursor(0,44); // 24+16+4
                display->printf("%5.1f A Sa\n", serviceBattery->current); 
            } else {
                display->printf("--.-- V Sv\n");
                display->setCursor(0,44); // 24+16+4
                display->printf("---.- A Sa\n"); 
            }
            display->display();
            subPage = 1;
            return false;
        case 1:
            if ( drawHistory(display, modbus, true,10.0,15.0) ) {
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
