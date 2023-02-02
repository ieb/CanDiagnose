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
		void setNextPage(TFTDisplayPage *nextPage) {
			nextDisplayPage = nextPage;
		};
		TFTDisplayPage *nextPage() {
			nextDisplayPage->displaying = false;
			return nextDisplayPage;
		};
		virtual void update();
	protected:
        TFT_eSPI &tft;
		TFTDisplayPage *nextDisplayPage;
		bool displaying = false; // if false draw for the first time.

};

class TFTEngineDisplayPage : public TFTDisplayPage {	
	public:
		TFTEngineDisplayPage(TFT_eSPI &tft):  TFTDisplayPage{tft} {
		};
		void update() override;
	private:
		int d = 0; // dummy value
};


class TFTDisplay {

	public:
        TFTDisplay(N2KCollector &n2kCollector, Modbus &modbus, WebServer &webServer, TFT_eSPI &tft) : 
            n2kCollector{n2kCollector},
            modbus{modbus},
            webServer{webServer},
            tft{tft}
             {
             	tftEngineDisplayPage.setNextPage(&tftEngineDisplayPage);
             	currentPage = &tftEngineDisplayPage;


        };
        void begin();
        void update();
        void nextPage();

    private:
        N2KCollector &n2kCollector;
        Modbus &modbus;
        WebServer &webServer;
        TFT_eSPI &tft;
        TFTDisplayPage *currentPage;
        TFTEngineDisplayPage tftEngineDisplayPage = TFTEngineDisplayPage(tft);

	
};




