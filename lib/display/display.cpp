
#include "display.h"
#include <Wire.h>
#define SSD1306_NO_SPLASH 1
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "splash.h"


void OledDisplay::begin() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, address)) {
    Serial.println(F("SSD1306 allocation failed"));
    return;
  }
  Serial.print(F("SSD1306 started"));Serial.println(address,HEX);

  addDisplayPage(this);

}


void OledDisplay::update() {
    unsigned long now = millis();
    if ( dimming ) {
        if ( now > lastDim + dimPeriod ) {
            lastDim = now;
            dim();
        }
    } else {
        if ( now > staticPagePress+15000 && now > lastDisplay + displayPeriod )  {
            lastDisplay = now;
            lastDim = now;
            if ( displayPages[page]->drawPage(&display) ) {
                page++;
                if ( page == lastPage || displayPages[page] == NULL ) {
                    page = 0;
                }
            }
        }
    }
    

}

void OledDisplay::nextPage() {
    staticPagePress = millis();
    Serial.print("Static Page");Serial.println(staticPage);
    if ( staticPages[staticPage]->drawPage(&display) ) {
        staticPage++;
        if ( staticPage == lastStaticPage || staticPages[staticPage] == NULL ) {
            staticPage = 0;
        }
    }
}
void OledDisplay::startDim() {
    dimming = true;
    update();
}
void OledDisplay::endDim() {
    dimming = false;
    update();
}

void OledDisplay::dim() {
    if (dimmer == 0) {
        dimmer = 10;
    }
    dimmer--;
    display.ssd1306_command1(SSD1306_SETCONTRAST);
    display.ssd1306_command1(dimmer*25);
    display.clearDisplay();
    display.setCursor(52,20);              // Start at top-left corne
    display.setTextSize(3);   // 18x24, 4 rows, 10.6 chars
    display.printf("%d",dimmer); // ADC0 is service battery 
    display.display();

}

bool OledDisplay::drawPage(Adafruit_SSD1306 * display) {
    display->clearDisplay();
#if OLED_HEIGHT == 32
    switch(subPage) {
        case 0:
            display->drawBitmap((display->width() - spash12832_width) / 2, (display->height() - spash12832_height) / 2,
                        spash12832_data, spash12832_width, spash12832_height, 1);
            display->display();
            subPage  = 1;
            return false;
        case 1:
            display->drawBitmap((display->width() - luna_width) / 2, (display->height() - luna_height) / 2,
                        luna_data, luna_width, luna_height, 1);
            display->display();
            subPage  = 0;
            return true;
        default:
            subPage  = 0;
            return false;

    }
#else
  display->drawBitmap((display->width() - spash12864_width) / 2, (display->height() - spash12864_height) / 2,
               spash12864_data, spash12864_width, spash12864_height, 1);
  display->display();
  return true;
#endif
    
}
