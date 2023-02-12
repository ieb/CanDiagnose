

#include "TFTDisplay.h"
#include "TFTWidgets.h"
#include <TJpg_Decoder.h>

// Include SPIFFS
#define FS_NO_GLOBALS
#include <FS.h>
#include "SPIFFS.h" 


void TFTDisplay::begin() {
	Serial.println("TFT Begin");
	if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed, no images.");
  } 
  // backlight control
  pinMode(TFT_PWM_BL, OUTPUT);
  ledcSetup(0, 5000, 8);
	ledcAttachPin(TFT_PWM_BL, 0);
	ledcWrite(0, 0);


  tft.begin();
  tft.setRotation(1);
  Serial.println("TFT Begin done");

}
#define MAX_PAGES 4
#define LOGO_PAGE 0
#define ENGINE_PAGE 1
#define SAILING_PAGE 2
#define MAXI_PAGE 3

void TFTDisplay::update(unsigned long lastButtonPress) {
 	if ( currentPage == NULL ) {
 		pageNo = MAX_PAGES-1;
 		nextPage();
 	}
	currentPage->update(paintScreen(lastButtonPress));
	
}
void TFTDisplay::nextPage() {
	// dont advance the page if waking up the display.
	if ( lastLevel > 0 ) {
		pageNo++;
	} else if ( currentPage != NULL ) {
		// keep the current page if its available, otherwise make sure its not null.
		return;
	}

	// dispose of the current page as it will have changed here.
	if (currentPage != NULL ) {
		delete(currentPage);
		currentPage = NULL; // just to be sure the pointer is not re-used.
	}
	switch(pageNo) {
		case ENGINE_PAGE:
			currentPage = new TFTEngineDisplayPage(tft);
			break;
		case MAXI_PAGE:
			currentPage = new TFTMaxiDisplayPage(tft);
			break;
		case SAILING_PAGE:
			currentPage = new TFTGridBoxesDisplayPage(tft);
			break;
		default:
			pageNo = 0;
			currentPage = new TFTLogoDisplayPage(tft);
	}
}

void TFTDisplay::setBacklightLevel(int level) {
	if ( level < 0 ) {
		level = 0;
	} else if (level > targetLevel) {
		level = targetLevel;
	}
	if ( level != lastLevel ) {
			ledcWrite(0, level);
			lastLevel = level;		
	}
}

/** 
 * Sleep the screen after 60s of no activity,
 * After 70s it should be turned off.
 * After a press brighten to the target level in 10s
 * Otherwise track the target level 
 * @return false if the screen is off, and true if on.
 */
bool TFTDisplay::paintScreen(unsigned long lastButtonPress) {
	unsigned long now = millis();
	unsigned long elapsed = now - lastButtonPress;
	if ( elapsed > 70000 ) {
		setBacklightLevel(0);
		return false;
	} else if ( elapsed > 60000 ) {
		setBacklightLevel((255-((elapsed-60000)/39)));
	} else if (startRampUp > 0) {
		if ( lastLevel < targetLevel ) {
			// complete the ramp un in brightness.
			elapsed = now - startRampUp;
			setBacklightLevel((elapsed/39));
		} else {
			// once complete disable ramp up.
			startRampUp = 0;
		}
	} else {
		if ( lastLevel < targetLevel ) {
			if ( elapsed < 10000 ) {
				// store when the turn on happened so that if 
				// there is a repeat button press it doesnt start
				// again from scratch.
				startRampUp = now;
				setBacklightLevel((elapsed/39));
			} else {
				setBacklightLevel(lastLevel+1);
			}
		} else if ( lastLevel > targetLevel) {
			setBacklightLevel(lastLevel-1);
		}
	}
	return true;
}




/**
 * One large dial for RPM
 * Smaller for Temperature
 * Smaller for Fuel
 * numbers for Engine volts, Service Volts and Alternator volts
 * numbers for Alternator temperature, Exhaust temperature, Charger temperature, service battery temperatire.
 */
void TFTEngineDisplayPage::update(bool paintScreen) {
	if ( !paintScreen) {
		return;
	}
	if ( !displaying ) {
		// draw for the first time
		Serial.println("Enter Engine Display");
		displaying = true;
		tft.fillScreen(TFT_BLACK);
	} 
	// update data elements.

	d += 4; if (d >= 360) d = 0;


	// RPM
    int reading = 0 + 3500 * TFTWidgets::sineWave(d + 90);
    TFTWidgets::ringMeter(&tft,reading, /* min */ 0, /* max */ 3500, /* x */ (480/2)-128,  /* y */ (320/2)-128,  /* radius */ 128, "RPM", BLUE2RED); // Draw analogue meter

    // temperature
    reading = 0 + 100 * TFTWidgets::sineWave(d + 90);
    TFTWidgets::ringMeter(&tft, reading, /* min */ 0, /* max */ 100, /* x */ 480-128,  /* y */ 0,  /* radius */ 64, "Coolant C", BLUE2RED); // Draw analogue meter

    // fuel
    reading = 0 + 100 * TFTWidgets::sineWave(d + 90);
    TFTWidgets::ringMeter(&tft, reading, /* min */ 0, /* max */ 100, /* x */ 480-128,  /* y */ 320-128,  /* radius */ 64, "Fuel %", BLUE2RED); // Draw analogue meter



}


// 
struct {
	TFT_eSPI *tft;
	int x_offset, y_offset;
} tft_output_config;
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
		if ( tft_output_config.tft == NULL ) return 0;
	  if ( y >= tft_output_config.tft->height() ) return 0;

	  // This function will clip the image block rendering automatically at the TFT boundaries
	  tft_output_config.tft->pushImage(tft_output_config.x_offset+x, tft_output_config.y_offset+y, w, h, bitmap);

	  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
	  // tft.drawRGBBitmap(x, y, bitmap, w, h);

	  // Return 1 to decode next block
	  return 1;
};



void TFTLogoDisplayPage::update(bool paintScreen) {
	if ( !paintScreen ) {
		return;
	}
	if ( !displaying ) {


	  tft.setTextColor(TFT_BLACK, TFT_WHITE);
	  tft.fillScreen(TFT_WHITE);
	  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

	  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
	  TJpgDec.setJpgScale(1);

	  // create a lambda to call back to
	  tft_output_config.tft = &tft;
	  tft_output_config.x_offset = 0;
	  tft_output_config.y_offset = 0;
	  // The decoder must be given the exact name of the rendering function above
	  TJpgDec.setCallback(tft_output);

	  unsigned long t = millis();
	  // Get the width and height in pixels of the jpeg if you wish
	  uint16_t w = 0, h = 0;
	  TJpgDec.getFsJpgSize(&w, &h, "/LunaLogo480_320.jpg"); // Note name preceded with "/"
	  Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);

	  // Draw the image, top left at 0,0
	  TJpgDec.drawFsJpg(0, 0, "/LunaLogo480_320.jpg");
	  tft_output_config.tft = NULL;

	  t = millis() - t;
	  Serial.print(t); Serial.println(" ms");


		displaying = true;
  }
}
void TFTLogoDisplayPage::update(const char *message, int lineNo) {
	tft.fillRect(0,318-30,480,30,TFT_WHITE);
	tft.setTextDatum(BC_DATUM);
  tft.drawString(message, 480/2, 318, 4); // Value in middle
}


void TFTMaxiDisplayPage::update(bool paintScreen) {
		if ( !paintScreen) {
			return;
		}
  	uint16_t value = millis()/1000;
  	if ( value != currentValue ) {
  		currentValue = value;
		char buf[10];
	  	ltoa(value, buf, 10);
		if ( !displaying ) {
			// draw for the first time
			Serial.println("Enter Maxi Display");
			displaying = true;
			tft.fillScreen(TFT_BLACK);
		}
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
  		tft.setTextDatum(MC_DATUM);
  		  // Convert value to a string

	    tft.drawString(buf, 480/2, 320/2, 8); // Value in middle, Font 8 only as numbers, no letters.

  	}
}


void TFTGridBoxesDisplayPage::update(bool paintScreen) {

	if ( !paintScreen) {
		return;
	}

	if ( !displaying ) {
			// draw for the first time
			Serial.println("Enter Logo Page");
			tft.fillScreen(TFT_BLACK);
  	}

  	unsigned long now = millis();
  	if ( now > lastUpdate + 1000) {
  		lastUpdate = now;
	  	d += 4; if (d >= 360) d = 0;
	  }

	//	static void textBox(TFT_eSPI *tft,char *value, int x, int y, int height, int width, char *bl, char *br);


	  int reading = d-180;
	  
  	awa.update(&tft, 0.1+reading, !displaying );
    reading = 20 + 20 * TFTWidgets::sineWave(d + 90);
  	aws.update(&tft, 0.2+reading, !displaying );
    reading = d-180+2;
  	twa.update(&tft, 0.1+reading, !displaying );
    reading = 20 + 20 * TFTWidgets::sineWave(d + 93);
  	tws.update(&tft, 0.2+reading, !displaying );


    reading = (d+45)%360;
  	cog.update(&tft, 0.1+reading, !displaying );
    reading = 20 + 20 * TFTWidgets::sineWave(d + 93);
  	sog.update(&tft, 0.2+reading, !displaying );
    reading = (d+55)%360;
  	hdg.update(&tft, 0.1+reading, !displaying );
    reading = 20 + 20 * TFTWidgets::sineWave(d + 93);
  	stw.update(&tft, 0.2+reading, !displaying );


    reading = 100 + 100 * TFTWidgets::sineWave(d + 93);
  	dbt.update(&tft, 0.1+reading, !displaying );
    reading = 500 + 500 * TFTWidgets::sineWave(d + 93);
  	log.update(&tft, 0.2+reading, !displaying );
    float lat = 0 + 90 * TFTWidgets::sineWave(d + 93);
    float lon = 0 + 180 * TFTWidgets::sineWave(d + 93);
  	pos.update(&tft, lat, lon, !displaying );
    reading = 1000 + 100 * TFTWidgets::sineWave(d + 93);
  	mbar.update(&tft, 0.1+reading, !displaying );

    reading = 12 + 3 * TFTWidgets::sineWave(d + 93);
  	ebv.update(&tft, 0.1+reading, !displaying );
    reading = 12 + 3 * TFTWidgets::sineWave(d + 88);
  	sbv.update(&tft, 0.2+reading, !displaying );
    reading = 0 + 100 * TFTWidgets::sineWave(d + 93);
  	sbc.update(&tft, 0.1+reading, !displaying );
    //reading = 20 + 20 * TFTWidgets::sineWave(d + 93);
  	//sbt.update(&tft, 0.2+reading, !displaying );
		

		/*
  	 reading = 12 + 3 * TFTWidgets::sineWave(d + 88);
  	sbv.update(&tft, 0.2+reading, !displaying );
		*/

		displaying = true;





}
