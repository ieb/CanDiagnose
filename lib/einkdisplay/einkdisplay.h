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
#include "modbus.h"
#include "httpserver.h"

//#define DUMMYDATA 1

class EinkDisplay {
    public:
        EinkDisplay(
            N2KCollector &n2kCollector,
            Modbus &modbus,
            WebServer &webServer,
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
                n2kCollector{n2kCollector},
                modbus{modbus} {
        	hspi.begin(sck_pin, SPI_DISPLAY_MIS0, din_pin, cs_pin);
                
        };
        void begin(int32_t baud=0 );
        void update();
        void nextPage(void); // must be fast as will be called by ISR.
        uint8_t getPageNo(void) {
            return pageNo;
        };

        uint16_t getPageNoCalls() {
            return pageNoCalls;
        };
        uint16_t getPageNoChanges() {
            return pageNoChanges;
        };


    private:
        GxEPD2_BW<GxEPD2_370_TC1, GxEPD2_370_TC1::HEIGHT> display;
        SPIClass hspi;
        SPISettings spi_settings;
        N2KCollector &n2kCollector;
        Modbus &modbus;
        volatile unsigned long pageChangeAt = 0;
        volatile uint16_t pageNoCalls = 0;
        volatile uint16_t pageNoChanges = 0;
        volatile uint8_t pageNo = 0;
        unsigned long lastUpdate = 0;
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
        void updateValues(bool force=true);
        double average(double v, double r); 
        double angularAverage(double v, double r);


};


