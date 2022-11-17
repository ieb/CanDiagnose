#pragma once

#define SPI_DISPLAY_MOSI GPIO_NUM_32
#define SPI_DISPLAY_MIS0 GPIO_NUM_35  // not connected
#define SPI_DISPLAY_SCK GPIO_NUM_33
#define SPI_DISPLAY_CS GPIO_NUM_25
#define SPI_DISPLAY_DC GPIO_NUM_26
#define SPI_DISPLAY_RST GPIO_NUM_27
#define SPI_DISPLAY_BUSY GPIO_NUM_35

#include <pgmspace.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include "bitmaps.h"
#include <TimeLib.h>
#include "dataoutput.h"

#define DUMMYDATA 1

class EinkDisplay {
    public:
        EinkDisplay(
            DataCollector &dataCollector,
        	int8_t din_pin=SPI_DISPLAY_MOSI,
        	int8_t sck_pin=SPI_DISPLAY_SCK,
        	int8_t cs_pin=SPI_DISPLAY_CS, 
        	int8_t dc_pin=SPI_DISPLAY_DC,
        	int8_t rst_pin=SPI_DISPLAY_RST,
        	int8_t busy_pin=SPI_DISPLAY_BUSY) : 
                display{GxEPD2_370_TC1(/*CS=D8*/ cs_pin, 
        					/*DC=D3*/ dc_pin, 
        					/*RST=D4*/ rst_pin, 
        					/*BUSY=D2*/ busy_pin)},
        		hspi(HSPI),
        		spi_settings(4000000, MSBFIRST, SPI_MODE0),
                dataCollector{dataCollector} {
        	hspi.begin(sck_pin, SPI_DISPLAY_MIS0, din_pin, cs_pin);
                
        };
        void begin(int32_t baud=115200 );
        void update(void);
        void nextPage(void);
        /*void dim(void);
        void endDim(void);
        void startDim(void);
        void wake(void);
        void sleep(void);*/




    private:
        GxEPD2_BW<GxEPD2_370_TC1, GxEPD2_370_TC1::HEIGHT> display;
        SPIClass hspi;
        SPISettings spi_settings;
        DataCollector dataCollector;

        unsigned long pageChangeAt = 0;
        uint8_t pageNo = 0;
        uint8_t pageVisible = 255;
        
        uint16_t marginY = 20;
        uint16_t marginX = 10;
        uint16_t lineHeight = 30;
        uint16_t cellWidth = 180;
        uint16_t middleX = 250;

        float logReading, pressureReading, sogReading, cogReading,
            stwReading, hdmReading, awsReading, awaReading, twsReading, twaReading,
            serviceVoltReading, alternatorVoltReading, engineVoltReading, fuelLitersReading,
            engineHoursReading ,coolantTemperatureReading, rpmReading;

        void showLogo(void);
        void page1Display(void);
        void page2Display(void);
        /**
         * latValue filled with the latitude value
         * lonValue filled with the longitude value
         * latSign filled with 
         */
        void formatLatLon(char * latValue /* char[11] */, 
                        char * lonValue /* char[11] */, 
                        char * latSign /* char[2] */, 
                        char * lonSign /* char[2] */);
        void formatSystemTime(char *systemTimeValue /* char[20] */);
        void printCell(int row, int col, 
                const char * label, 
                const char *v="", 
                const char * units="" );
        void printCell(int row, int col,  
                const char * label, 
                float v,  
                const char * units, 
                const char * format );
        void printCell(int row, int col,  
                const char * label, 
                float v,  
                const char * posUnits,  
                const char * negUnits, 
                const char * format );
        void printFloat(int x, int y, 
                float f, 
                const char * format, 
                bool rightAlign = true);
        void printString(int x, int y, 
                const char * text, 
                bool rightAlign = false);
        void updateValues(void);

};


