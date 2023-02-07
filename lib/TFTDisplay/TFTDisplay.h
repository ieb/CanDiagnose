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
		virtual void update();
		virtual void update(const char * message, int lineNo) {};
	protected:
        TFT_eSPI &tft;
		TFTDisplayPage *nextDisplayPage;
		bool displaying = false; // if false draw for the first time.

};

class TFTEngineDisplayPage : public TFTDisplayPage {	
	public:
		TFTEngineDisplayPage(TFT_eSPI &tft):  TFTDisplayPage{tft} {
		};
		~TFTEngineDisplayPage() override = default;
		void update() override;
	private:
		int d = 0; // dummy value
};

class TFTLogoDisplayPage : public TFTDisplayPage {	
	public:
		TFTLogoDisplayPage(TFT_eSPI &tft):  TFTDisplayPage{tft} {
		};
		~TFTLogoDisplayPage() override = default;
		void update() override;
		void update(const char * message, int lineNo) override;
	private:
		int d = 0; // dummy value
};


class TFTMaxiDisplayPage : public TFTDisplayPage {	
	public:
		TFTMaxiDisplayPage(TFT_eSPI &tft):  TFTDisplayPage{tft} {
		};
		~TFTMaxiDisplayPage() override = default;
		void update() override;
	private:
		uint16_t currentValue = 0; 

};

#define GRID_GAP 4
#define  GRID_4_WIDTH ((480/4)-GRID_GAP)
#define  GRID_4_HEIGHT ((320/4)-GRID_GAP)
#define  GRID_4_SPACE_X(x) ((x)*(480/4))
#define  GRID_4_SPACE_Y(y) ((y)*(320/4))


class TFTGridBoxesDisplayPage : public TFTDisplayPage {	
	public:
		TFTGridBoxesDisplayPage(TFT_eSPI &tft):  TFTDisplayPage{tft} {

		};
		~TFTGridBoxesDisplayPage() override = default;
		void update() override;
	private:
		unsigned long lastUpdate = 0;
		int d = 0; // dummy value
		TFTTextBox awa = TFTTextBox(GRID_4_SPACE_X(0), GRID_4_SPACE_Y(0), GRID_4_WIDTH, GRID_4_HEIGHT, 0, -180, 180,  "AWA", "deg", 'P', 'S');
		TFTTextBox aws = TFTTextBox(GRID_4_SPACE_X(0), GRID_4_SPACE_Y(1), GRID_4_WIDTH, GRID_4_HEIGHT, 1, 0, 99, "AWS", "kn");
		TFTTextBox twa = TFTTextBox(GRID_4_SPACE_X(0), GRID_4_SPACE_Y(2), GRID_4_WIDTH, GRID_4_HEIGHT, 0, -180, 180, "TWA", "deg", 'P', 'S');
		TFTTextBox tws = TFTTextBox(GRID_4_SPACE_X(0), GRID_4_SPACE_Y(3), GRID_4_WIDTH, GRID_4_HEIGHT, 1, 0, 99, "TWS", "kn");

		TFTTextBox cog = TFTTextBox(GRID_4_SPACE_X(1), GRID_4_SPACE_Y(0), GRID_4_WIDTH, GRID_4_HEIGHT, 0, 0, 360,  "COG", "deg");
		TFTTextBox sog = TFTTextBox(GRID_4_SPACE_X(1), GRID_4_SPACE_Y(1), GRID_4_WIDTH, GRID_4_HEIGHT, 1, 0, 99, "SOG", "kn");
		TFTTextBox hdg = TFTTextBox(GRID_4_SPACE_X(1), GRID_4_SPACE_Y(2), GRID_4_WIDTH, GRID_4_HEIGHT, 0, 0, 360, "HDG", "deg");
		TFTTextBox stw = TFTTextBox(GRID_4_SPACE_X(1), GRID_4_SPACE_Y(3), GRID_4_WIDTH, GRID_4_HEIGHT, 1, 0, 99, "STW", "kn");

		TFTTextBox dbt = TFTTextBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(0), GRID_4_WIDTH, GRID_4_HEIGHT, 1, 0, 250,  "DBT", "deg");
		TFTTextBox log = TFTTextBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(1), GRID_4_WIDTH, GRID_4_HEIGHT, 1, 0, 9999,  "Log", "Nm");
		TFTTextBox pos = TFTLatLonBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(1), GRID_4_WIDTH, GRID_4_HEIGHT);
		//TFTTextBox swt = TFTTextBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(2), GRID_4_WIDTH, GRID_4_HEIGHT, 1, -99, 99, "SWT", "C");
		TFTTextBox mbar = TFTTextBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(3), GRID_4_WIDTH, GRID_4_HEIGHT,0, 700, 1100, "MBAR", "mbar");
		//TFTTextBox ert = TFTTextBox(GRID_4_SPACE_X(2), GRID_4_SPACE_Y(4), GRID_4_WIDTH, GRID_4_HEIGHT, 1, -99, 99, "ERT", "C");

		TFTTextBox ebv = TFTTextBox(GRID_4_SPACE_X(3), GRID_4_SPACE_Y(0), GRID_4_WIDTH, GRID_4_HEIGHT, 2, 0, 30,  "EBV", "V");
		TFTTextBox sbv = TFTTextBox(GRID_4_SPACE_X(3), GRID_4_SPACE_Y(1), GRID_4_WIDTH, GRID_4_HEIGHT, 2, 0, 30, "SBV", "V");
		TFTTextBox sbc = TFTTextBox(GRID_4_SPACE_X(3), GRID_4_SPACE_Y(2), GRID_4_WIDTH, GRID_4_HEIGHT, 1, -200, 200, "SBC", "A");
		TFTTextBox sbt = TFTTextBox(GRID_4_SPACE_X(3), GRID_4_SPACE_Y(3), GRID_4_WIDTH, GRID_4_HEIGHT, 1, -99, 99, "SBT", "C");




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
        void update();
        void update(const char * message, int lineNo) {
        	currentPage->update(message, lineNo);
        };
        void nextPage();

    private:
        N2KCollector &n2kCollector;
        Modbus &modbus;
        WebServer &webServer;
        TFT_eSPI &tft;
        uint8_t pageNo = 0;
        TFTDisplayPage *currentPage = NULL;

	
};




