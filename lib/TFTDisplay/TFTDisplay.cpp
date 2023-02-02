

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
void TFTDisplay::update() {
	currentPage->update();
}
void TFTDisplay::nextPage() {
    currentPage = currentPage->nextPage();
}


/**
 * One large dial for RPM
 * Smaller for Temperature
 * Smaller for Fuel
 * numbers for Engine volts, Service Volts and Alternator volts
 * numbers for Alternator temperature, Exhaust temperature, Charger temperature, service battery temperatire.
 */
void TFTEngineDisplayPage::update() {
	if ( displaying ) {
		// draw for the first time
		displaying = true;
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
