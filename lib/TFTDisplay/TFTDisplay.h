#pragma once
#include <Arduino.h>

// In platform.ini
// Defining here or in tft_setup.h doesnt work since the 
// values are required by the library which see these when its cpp file is built,
// hence it stick to the defauls.
// editing the library is not sustainable, hence these defines re done in platform.ini
// as build_opts.
//#define TFT_MISO GPIO_NUM_35
//#define TFT_MOSI GPIO_NUM_32
//#define TFT_SCLK GPIO_NUM_33
//#define TFT_CS   GPIO_NUM_25  // Chip select control pin
//#define TFT_DC   GPIO_NUM_26  // Data Command control pin
//#define TFT_RST  GPIO_NUM_27  // Reset pin (could connect to Arduino RESET pin)
//#define TFT_BL   GPIO_NUM_14  // LED back-light (required for M5Stack)
//#define ILI9488_DRIVER     // WARNING: Do not connect ILI9488 display SDO to MISO if other devices share the SPI bus (TFT SDO does NOT tristate when CS is high)



#include <TFT_eSPI.h> // Hardware-specific library, includes tft_setup.h fro the hardware settings.
#include <SPI.h>
#include "N2KCollector.h"
#include "modbus.h"
#include "httpserver.h"

#include "TFTWidgets.h"


class TFTDisplayPage {
	public:
		TFTDisplayPage(TFT_eSPI &tft): tft{tft} {
		};
		virtual ~TFTDisplayPage() = default;
		virtual void update(bool paintScreen);
		virtual void update(const char * message, int lineNo) {};

	protected:
        TFT_eSPI &tft;
		TFTDisplayPage *nextDisplayPage;
		bool displaying = false; // if false draw for the first time.

};


class TFTEngineDisplayPage : public TFTDisplayPage {	
	public:
		TFTEngineDisplayPage(TFT_eSPI &tft, N2KCollector &n2kCollector, Modbus &modbus):  
			TFTDisplayPage{tft},
			n2kCollector{n2kCollector},
            modbus{modbus}{
		};
		~TFTEngineDisplayPage() override = default;
		void update(bool paintScreen) override;
	private:
        N2KCollector &n2kCollector;
        Modbus &modbus;
#ifdef DEMO_MODE
		int d = 0;
		int rpm = 0; // dummy value
		int rpmadd = 1000;
		int fuelLevel = 80;
		int fuelLevelAdd = 12;
		int coolantTemperature = 20;
		int coolantTemperatureAdd = 12;
#endif
		unsigned long lastUpdate = 0;
		TFTTachometer tachometer = TFTTachometer(10,10);
		TFTFuelGauge fuel = TFTFuelGauge(480-130-10,10);
		TFTCoolantGauge coolant = TFTCoolantGauge(480-130-10,320-130-10);
};

class TFTLogoDisplayPage : public TFTDisplayPage {	
	public:
		TFTLogoDisplayPage(TFT_eSPI &tft):  TFTDisplayPage{tft} {
		};
		~TFTLogoDisplayPage() override = default;
		void update(bool paintScreen) override;
		void update(const char * message, int lineNo) override;
	private:
#ifdef DEMO_MODE
		int d = 0; // dummy value
#endif
		TFTSplash splash = TFTSplash("/LunaLogo480_320.jpg");
};


class TFTMaxiDisplayPage : public TFTDisplayPage {	
	public:
		TFTMaxiDisplayPage(TFT_eSPI &tft, N2KCollector &n2kCollector, Modbus &modbus, WebServer &webServer):  
			TFTDisplayPage{tft},
			n2kCollector{n2kCollector},
            modbus{modbus},
            webServer{webServer} {
		};
		~TFTMaxiDisplayPage() override = default;
		void update(bool paintScreen) override;
	private:
        N2KCollector &n2kCollector;
        Modbus &modbus;
        WebServer &webServer;
		uint16_t currentValue = 0; 

};



class TFTSailingDisplayPage : public TFTDisplayPage {	
	public:
		TFTSailingDisplayPage(TFT_eSPI &tft, N2KCollector &n2kCollector):  
			TFTDisplayPage{tft},
			n2kCollector{n2kCollector} {
		};
		~TFTSailingDisplayPage() override = default;
		void update(bool paintScreen) override;
	private:
        N2KCollector &n2kCollector;
		unsigned long lastUpdate = 0;
		float d = 0;
		TFTSailing sailing = TFTSailing(480/2-320/2,0,320,320);
		TFTTrueWindBlock trueWind = TFTTrueWindBlock();
		TFTApparentWindBlock apparentWind = TFTApparentWindBlock();
		TFTVMGBlock polar = TFTVMGBlock();
		TFTCurrentBlock current = TFTCurrentBlock();
};

#define GRID_GAP 4
#define  GRID_4_WIDTH ((480/4)-GRID_GAP)
#define  GRID_4_HEIGHT ((320/4)-GRID_GAP)
#define  GRID_4_SPACE_X(x) ((x)*(480/4))
#define  GRID_4_SPACE_Y(y) ((y)*(320/4))


class TFTGridBoxesDisplayPage : public TFTDisplayPage {	
	public:
		TFTGridBoxesDisplayPage(TFT_eSPI &tft, N2KCollector &n2kCollector, Modbus &modbus, WebServer &webServer):  
			TFTDisplayPage{tft},
			n2kCollector{n2kCollector},
            modbus{modbus},
            webServer{webServer} {

		};
		~TFTGridBoxesDisplayPage() override = default;
		void update(bool paintScreen) override;
	private:
        N2KCollector &n2kCollector;
        Modbus &modbus;
        WebServer &webServer;
		unsigned long lastUpdate = 0;
		int d = 0; // dummy value
		TFTTextBox awa = TFTTextBox(GRID_4_SPACE_X(0), GRID_4_SPACE_Y(0), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 0, -180, 180,  "AWA", "deg", 'P', 'S');
		TFTTextBox aws = TFTTextBox(GRID_4_SPACE_X(0), GRID_4_SPACE_Y(1), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 1, 0, 99, "AWS", "kn");
		TFTTextBox twa = TFTTextBox(GRID_4_SPACE_X(0), GRID_4_SPACE_Y(2), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 0, -180, 180, "TWA", "deg", 'P', 'S');
		TFTTextBox tws = TFTTextBox(GRID_4_SPACE_X(0), GRID_4_SPACE_Y(3), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 1, 0, 99, "TWS", "kn");

		TFTTextBox cog = TFTTextBox(GRID_4_SPACE_X(1), GRID_4_SPACE_Y(0), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 0, 0, 360,  "COG", "deg");
		TFTTextBox sog = TFTTextBox(GRID_4_SPACE_X(1), GRID_4_SPACE_Y(1), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 1, 0, 99, "SOG", "kn");
		TFTTextBox hdg = TFTTextBox(GRID_4_SPACE_X(1), GRID_4_SPACE_Y(2), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 0, 0, 360, "HDG", "deg");
		TFTTextBox stw = TFTTextBox(GRID_4_SPACE_X(1), GRID_4_SPACE_Y(3), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 1, 0, 99, "STW", "kn");

		TFTTextBox dbt = TFTTextBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(0), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 1, 0, 250,  "DBT", "deg");
		TFTTextBox log = TFTTextBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(1), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 1, 0, 9999,  "Log", "Nm");
		// 2 wide so we loose 3,2
		TFTLatLonBox pos = TFTLatLonBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(2), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP);
		//TFTTextBox swt = TFTTextBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(2), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 1, -99, 99, "SWT", "C");
		TFTTextBox mbar = TFTTextBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(3), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP,0, 700, 1100, "MBAR", "mbar");
		//TFTTextBox ert = TFTTextBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(4), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 1, -99, 99, "ERT", "C");

		TFTTextBox ebv = TFTTextBox(GRID_4_SPACE_X(3), GRID_4_SPACE_Y(0), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 2, 0, 30,  "EBV", "V");
		TFTTextBox sbv = TFTTextBox(GRID_4_SPACE_X(3), GRID_4_SPACE_Y(1), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 2, 0, 30, "SBV", "V");
		TFTTextBox sbc = TFTTextBox(GRID_4_SPACE_X(3), GRID_4_SPACE_Y(3), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 1, -200, 200, "SBC", "A");
		//TFTTextBox sbt = TFTTextBox(GRID_4_SPACE_X(3), GRID_4_SPACE_Y(3), GRID_4_WIDTH, GRID_4_HEIGHT, GRID_GAP, 1, -99, 99, "SBT", "C");




};

class TFTDisplay {

	public:
        TFTDisplay(N2KCollector &n2kCollector, Modbus &modbus, WebServer &webServer, TFT_eSPI &tft) : 
            n2kCollector{n2kCollector},
            modbus{modbus},
            webServer{webServer},
            tft{tft}
             {
             	currentPage = new TFTLogoDisplayPage(tft);


        };
        void begin();
        void update(unsigned long lastButtonPress);
        void update(const char * message, int lineNo) {
        	currentPage->update(message, lineNo);
        };
        void nextPage();
		void setBacklightLevel(int level);
        void incTargetBrightness() {
        	targetLevel++;
        };
    private:
        N2KCollector &n2kCollector;
        Modbus &modbus;
        WebServer &webServer;
        TFT_eSPI &tft;
        uint8_t pageNo = 0;
        TFTDisplayPage *currentPage = NULL;
		uint8_t lastLevel = 0; // current backlight level
		uint8_t targetLevel = 255; // target backlight level
		unsigned long startRampUp = 0;
		bool paintScreen(unsigned long lastButtonPress);

	
};




