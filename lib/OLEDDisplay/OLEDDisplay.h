#pragma once

#include <Wire.h>
#define SSD1306_NO_SPLASH 1
#include <Adafruit_SSD1306.h>
#include "N2KCollector.h"
#include "modbus.h"
#include "httpserver.h"

#define OLED_WIDTH  128
#define OLED_HEIGHT 64
#define OLED_SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define MAX_DISPLAYPAGES 10

class DisplayPage {
    public:
        DisplayPage() {};
        int getSubPage() {
            return subPage;
        };

        bool incycle = true;
    protected:
        int subPage = 0;


};



typedef enum DisplayState {
    AWAKE,
    START_SLEEP,
    WAIT_SLEEP_END,
    SLEEPING,
    START_WAKE
} DisplayState;





class History128over24DisplayPage  {
    public:
        History128over24DisplayPage() {};
        bool drawHistory(Adafruit_SSD1306 * display, History128over24 * history128over24, bool fixedRange, float fixedRangeMin, float fixedRangeMax );
    private:
        float graphRangeMin = 5;
        uint8_t historyDrawState = 0;
        const char * title;
};
class LogoDisplayPage: public DisplayPage {
    public:
        LogoDisplayPage(){};
        bool drawPage(Adafruit_SSD1306 *display);    
};


class EngineDisplayPage: public DisplayPage {
    public:
        EngineDisplayPage(){};
        bool drawPage(Adafruit_SSD1306 *display, N2KCollector *n2kCollector);    
};
class LatLongDisplayPage: public DisplayPage {
    public:
        LatLongDisplayPage(){};
        bool drawPage(Adafruit_SSD1306 *display, N2KCollector *n2kCollector);    
};
class EnvironmentDisplayPage: public DisplayPage, public History128over24DisplayPage {
    public:
        EnvironmentDisplayPage(){};
        bool drawPage(Adafruit_SSD1306 *display, N2KCollector *n2kCollector);    
};
class WindDisplayPage: public DisplayPage {
    public:
        WindDisplayPage(){};
        bool drawPage(Adafruit_SSD1306 *display, N2KCollector *n2kCollector);    
};
class NavigationDisplayPage: public DisplayPage {
    public:
        NavigationDisplayPage() {};
        bool drawPage(Adafruit_SSD1306 *display, N2KCollector *n2kCollector);    
};
class LogDisplayPage: public DisplayPage {
    public:
        LogDisplayPage() {};
        bool drawPage(Adafruit_SSD1306 *display, N2KCollector *n2kCollector);    
};


class WebServerDisplayPage: public DisplayPage {
    public:
        WebServerDisplayPage()  {};
        bool drawPage(Adafruit_SSD1306 *display, WebServer *webServer);    
};

class ModbusDisplayPage: public DisplayPage, public History128over24DisplayPage {
    public:
        ModbusDisplayPage() {};
        bool drawPage(Adafruit_SSD1306 *display, Modbus *modbus);    
};


class OledDisplay {
    public:
        OledDisplay(N2KCollector &n2kCollector, Modbus &modbus, WebServer &webServer) : 
            n2kCollector{n2kCollector},
            modbus{modbus},
            webServer{webServer},
            display{OLED_WIDTH,OLED_HEIGHT}, 
            address{OLED_SCREEN_ADDRESS}
             {

        };
        void begin();
        void update();
        bool drawDisplay();
        void dim();
        void nextPage();
        void startDim();
        void endDim();
        void sleep();
        void wake();

    private:
        N2KCollector &n2kCollector;
        Modbus &modbus;
        WebServer &webServer;
        Adafruit_SSD1306 display;
        uint8_t address;
        unsigned long lastDisplay = 0;
        unsigned long displayPeriod = 4000;
        unsigned long lastDim = 0;
        unsigned long dimPeriod = 1000;
        unsigned long staticPagePress = 0;
        unsigned long sleepEndAt = 0;
        bool dimming = false;
        DisplayState displayState = AWAKE;
        uint8_t dimmer = 9;
        int8_t page = 0;
        int8_t lastPage = 9;

        LogoDisplayPage logoDisplayPage;
        WindDisplayPage windDisplayPage;
        EnvironmentDisplayPage environmentDisplayPage;
        NavigationDisplayPage navigationDisplayPage;
        LatLongDisplayPage latLongDisplayPage;
        LogDisplayPage logDisplayPage;
        ModbusDisplayPage modbusDisplayPage;
        EngineDisplayPage engineDisplayPage;
        WebServerDisplayPage webServerDisplayPage;


};

