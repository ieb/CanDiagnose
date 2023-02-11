

#include "TFTDisplay.h"
#include "TFTWidgets.h"

void TFTDisplay::begin() {
Serial.println("TFT Begin");
  tft.begin();
  //Serial.begin(9600);
  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("LUNA", 10, 10, 8); // Value in middle

  Serial.println("TFT Begin done");

}
#define MAX_PAGES 4
#define LOGO_PAGE 0
#define ENGINE_PAGE 1
#define SAILING_PAGE 2
#define MAXI_PAGE 3

void TFTDisplay::update() {
 	if ( currentPage == NULL ) {
 		pageNo = MAX_PAGES-1;
 		nextPage();
 	}
	currentPage->update();
}
void TFTDisplay::nextPage() {
	if (currentPage != NULL ) {
		delete(currentPage);
	}
	pageNo++;
	if ( pageNo == MAX_PAGES) {
		pageNo = 0;
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




/**
 * One large dial for RPM
 * Smaller for Temperature
 * Smaller for Fuel
 * numbers for Engine volts, Service Volts and Alternator volts
 * numbers for Alternator temperature, Exhaust temperature, Charger temperature, service battery temperatire.
 */
void TFTEngineDisplayPage::update() {
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

void TFTLogoDisplayPage::update() {
	if ( !displaying ) {
		// draw for the first time
		Serial.println("Enter Logo Page");
		displaying = true;
		tft.fillScreen(TFT_BLACK);
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
  		tft.setTextDatum(MC_DATUM);
	    tft.drawString("Luna Pogo 1250", 480/2, 320/2, 4); // Value in middle
  	}
}
void TFTLogoDisplayPage::update(const char *message, int lineNo) {
  		tft.setTextDatum(TC_DATUM);
	    tft.drawString(message, 480/2, 320/2+15*(lineNo+1), 2); // Value in middle
}


void TFTMaxiDisplayPage::update() {
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


void TFTGridBoxesDisplayPage::update() {
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
