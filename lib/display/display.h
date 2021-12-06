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
        virtual bool drawPage(Adafruit_SSD1306 * display);
        int getSubPage() {
            return subPage;
        };
        bool incycle = true;
    protected:
        int subPage = 0;


};


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
        bool drawPage(Adafruit_SSD1306 * display);
        void dim();
        void nextPage();
        void startDim();
        void endDim();

    private:
        Adafruit_SSD1306 display;
        uint8_t address;
        unsigned long lastDisplay = 0;
        unsigned long displayPeriod = 4000;
        unsigned long lastDim = 0;
        unsigned long dimPeriod = 1000;
        unsigned long staticPagePress = 0;
        bool dimming = false;
        uint8_t dimmer = 9;
        int8_t page = 0;
        int8_t lastPage = 0;
        int8_t staticPage = 0;
        int8_t lastStaticPage = 0;
        DisplayPage * displayPages[MAX_DISPLAYPAGES] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
        DisplayPage * staticPages[MAX_DISPLAYPAGES] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };


};

#endif