#ifndef _DISPLAY_H
#define _DISPLAY_H


#include <Wire.h>
#define SSD1306_NO_SPLASH 1
#include <Adafruit_SSD1306.h>

#define OLED_WIDTH  128
#define OLED_HEIGHT 64
#define OLED_SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define MAX_DISPLAYPAGES 10

class DisplayPage {
    public:
        DisplayPage() {};
        virtual bool drawPage(Adafruit_SSD1306 * display) = 0; 
        int getSubPage() {
            return subPage;
        };

        bool incycle = true;
    protected:
        int subPage = 0;


};

class History128over24 {
    public:
        History128over24(const char *title, float offset, float scale, unsigned long periodMs=675000) : 
            title{title},
            offset{offset}, 
            scale{scale}, 
            historyPeriod{periodMs} { };
        void storeHistory(double v);
        float changeSince(unsigned long tms);
        void startIterator();
        bool hasNext();
        float nextValue();
        bool drawHistory(Adafruit_SSD1306 * display, bool fixedRange=false, float fixedRangeMin=0.0, float fixedRangeMax=0.0 );
        unsigned long getPeriodMs() { return historyPeriod; }
        unsigned long getTimeWindowMs() { return historyPeriod*128; }
    private:
        uint16_t nextValueRaw();
        uint8_t index(unsigned long t) { return (t/historyPeriod)%128; }
        const char *title;
        float offset;
        float scale;
        float graphRangeMin = 5; // 5mBar by default.
        unsigned long historyPeriod =  675000;
        static const unsigned long historyLength =  128;
        int currentSlot=-1;
        double pmean;
        int hend;
        int iv;
        uint8_t historyDrawState = 0;
        uint8_t iteratorState;
        uint16_t history[128];
};

typedef enum DisplayState {
    AWAKE,
    START_SLEEP,
    WAIT_SLEEP_END,
    SLEEPING,
    START_WAKE
} DisplayState;



class OledDisplay : public DisplayPage {
    public:
        OledDisplay() : 
            display{OLED_WIDTH,OLED_HEIGHT}, 
            address{OLED_SCREEN_ADDRESS} {
                
        };
        void begin();
        void update();
        void addDisplayPage(DisplayPage * displayPage) {
            if ( displayPage->incycle && lastPage < MAX_DISPLAYPAGES ) {
                displayPages[lastPage] = displayPage;
                lastPage++;
            }
            if (lastStaticPage < MAX_DISPLAYPAGES) {
                staticPages[lastStaticPage] = displayPage;
                lastStaticPage++;
            }
        };
        bool drawPage(Adafruit_SSD1306 * display) override;
        void dim();
        void nextPage();
        void startDim();
        void endDim();
        void sleep();
        void wake();

    private:
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
        int8_t lastPage = 0;
        int8_t staticPage = 0;
        int8_t lastStaticPage = 0;
        DisplayPage * displayPages[MAX_DISPLAYPAGES] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
        DisplayPage * staticPages[MAX_DISPLAYPAGES] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };


};

#endif