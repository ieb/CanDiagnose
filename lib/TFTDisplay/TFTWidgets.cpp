/*
 An example showing 'ring' analogue meter on a TFT
 colour screen

 Needs Fonts 2, 4 and 7 (also Font 6 if using a large size meter)
 */



//#include "alert.h" // Out of range alert icon

#include "TFTWidgets.h"
#include <TJpg_Decoder.h>

// Include SPIFFS
#define FS_NO_GLOBALS
#include <FS.h>
#include "SPIFFS.h" 


#include "images.h"



// 


TFTOutputConfig tft_output_config;
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
    if ( tft_output_config.tft == NULL ) return 0;
    if ( (tft_output_config.y_offset+y) >= tft_output_config.tft->height() ) return 0; // start of bitmap is beyond area
    tft_output_config.tft->pushImage(tft_output_config.x_offset+x, tft_output_config.y_offset+y, w, h, bitmap);  
    return 1;    
}


bool sprite_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
    if ( tft_output_config.sprite == NULL ) return 0;
    if ( tft_output_config.y_offset+y >= tft_output_config.sprite->height() ) return 0;
    tft_output_config.sprite->pushImage(tft_output_config.x_offset+x, tft_output_config.y_offset+y, w, h, bitmap);
    return 1;
}  


/*
void loop() {
  if (millis() - runTime >= 0L) { // Execute every TBD ms
    runTime = millis();

    // Test with a slowly changing value from a Sine function
    d += 4; if (d >= 360) d = 0;

    // Set the the position, gap between meters, and inner radius of the meters
    int xpos = 0, ypos = 5, gap = 4, radius = 52;

    // Draw meter and get back x position of next meter

    // Test with Sine wave function, normally reading will be from a sensor
    reading = 250 + 250 * sineWave(d+0);
    xpos = gap + ringMeter(reading, 0, 500, xpos, ypos, radius, "mA", GREEN2RED); // Draw analogue meter

    reading = 20 + 30 * sineWave(d+60);
    xpos = gap + ringMeter(reading, -10, 50, xpos, ypos, radius, "degC", BLUE2RED); // Draw analogue meter

    reading = 50 + 50 * sineWave(d + 120);
    ringMeter(reading, 0, 100, xpos, ypos, radius, "%RH", BLUE2BLUE); // Draw analogue meter


    // Draw two more larger meters
    xpos = 20, ypos = 115, gap = 24, radius = 64;

    reading = 1000 + 150 * sineWave(d + 90);
    xpos = gap + ringMeter(reading, 850, 1150, xpos, ypos, radius, "mb", BLUE2RED); // Draw analogue meter

    reading = 15 + 15 * sineWave(d + 150);
    xpos = gap + ringMeter(reading, 0, 30, xpos, ypos, radius, "Volts", GREEN2GREEN); // Draw analogue meter

    // Draw a large meter
    //xpos = 480/2 - 160, ypos = 0, gap = 15, radius = 170;
    //reading +=(ramp);
    //if (reading>98) ramp = -1;
    //if (reading<0) ramp = 1;
    // Comment out above meters, then uncomment the next line to show large meter
    //ringMeter(reading,0,100, xpos,ypos,radius," Watts",GREEN2RED); // Draw analogue meter
    //if (reading<0) delay(1000);
  }
}
*/

/*
void TFTWidgets::textBox(TFT_eSPI *tft, const char *value, const char *previousValue, 
      int x, int y, int width, int height, const char *bl, const char *br, bool firstPaint) {
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
  if ( firstPaint ) {
    tft->drawRoundRect(x, y, width, height, 5, TFT_WHITE);
    tft->setTextDatum(BL_DATUM);
    tft->drawString(bl, x+5, y+height-5, 2); // Value in middle, Font 8 only as numbers, no letters.
    tft->setTextDatum(BR_DATUM);
    tft->drawString(br, x+width-5, y+height-5, 2); // Value in middle, Font 8 only as numbers, no letters.
  }
  if ( firstPaint || strcmp(value, previousValue) != 0) {
    tft->setTextDatum(MC_DATUM);
    tft->setTextColor(TFT_BLACK, TFT_BLACK);
    tft->drawString(previousValue, x+width/2, y+height/2, 4);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->drawString(value, x+width/2, y+height/2, 4); // Value in middle, Font 8 only as numbers, no letters.    
  }
}
*/

void TFTTextBox::formatValue(float value, char *buffer) {
  if ( value < minValue) {
    buffer[0] = 'L';
    buffer[1] = 0;
  } else if ( value >  maxValue) {
    buffer[0] = 'L';
    buffer[1] = 0;    
  } else if ( value < 0 ) {
    buffer[0] = negative;
    dtostrf(-value,1,precision, &buffer[1]);
  } else {
    if (positive != 0) {
      buffer[0] = positive;
      dtostrf(value,1,precision, &buffer[1]);
    } else {
      dtostrf(value,1,precision, buffer);
    }
  }
}
float TFTTextBox::adjustValue(float v, float p) {
  float d = v - p;
  float inc = 1;
  for (int i = 0; i < precision; i++) {
    inc = inc/10;
  }  
  if ( d > 50*inc ) {
    return p+49*inc;
  } else if ( d < -50*inc ) {
    return p-49*inc;
  } else if ( d > inc ) {
    return p+inc;
  } else if ( d < -inc ) {
    return p-inc;
  } else {
    return v;
  }
}

void TFTTextBox::update(TFT_eSPI *tft, float value, bool firstPaint) {  
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  if ( firstPaint ) {
    tft->drawRoundRect(x, y, width, height, 5, TFT_WHITE);
    tft->setTextDatum(BL_DATUM);
    tft->drawString(bl, x+5, y+height-5, 2); // Value in middle, Font 8 only as numbers, no letters.
    tft->setTextDatum(BR_DATUM);
    tft->drawString(br, x+width-5, y+height-5, 2); // Value in middle, Font 8 only as numbers, no letters.
  }

  char buffer1[10];
  char buffer2[10];
  float displayValue = adjustValue(value, previousValue);


  formatValue(displayValue, buffer1);
  formatValue(previousValue, buffer2);
  if ( firstPaint || strcmp(buffer1, buffer2) != 0) {
    TFT_eSprite textSprite = TFT_eSprite(tft);
    textSprite.setColorDepth(1);
    textSprite.createSprite(width-4, height-40);
    textSprite.setTextColor(TFT_WHITE, TFT_BLACK);
    textSprite.setTextDatum(MC_DATUM);
    textSprite.drawString(buffer1, (width-4)/2, (height-40)/2, 4);
    tft->setBitmapColor(TFT_WHITE, TFT_BLACK); 
    textSprite.pushSprite(x+2, y+20);

  }
  previousValue = displayValue;
}

void TFTLatLonBox::formatDeg( float v, char *buffer, bool isLatitude) {
  if ( v < 0 ) {
    v = -v;
  }
  int16_t d = (int) v;
  int8_t h =  (d/100);
  buffer[0] = h +'0';
  int8_t t = (d-(h*100))/10;
  buffer[1] = t+'0';
  int8_t u = (d-(h*100)-(t*10));  
  buffer[2] = u+'0';
  if ( isLatitude && buffer[0] == '0' ) {
    buffer[0] = ' ';
  }
  buffer[3] = 0;
}
void TFTLatLonBox::formatMin( float v, char *buffer, bool isLatitude) {
  if ( v < 0 ) {
    v = -v;
  }
  v = (v - (int)v)*60;
  int16_t d = (int) v;
  int8_t t = (d/10);
  buffer[0] = t+'0';
  int8_t u = (d-(t*10));  
  buffer[1] = u+'0';
  buffer[2] = 0;
}

void TFTLatLonBox::formatMinDec( float v, char *buffer, bool isLatitude) {
  char sign = 'N';
  if ( isLatitude ) {
    if ( v < 0 ) {
      sign = 'S';
    }
  } else {
    if ( v < 0 ) {
      sign = 'W';
    } else {
      sign = 'E';
    }
  }
  if ( v < 0 ) {
    v = -v;
  }
  v = (v - (int)v)*60;
  v = (v - (int)v)*1000;
  int16_t d = v;
  buffer[0] = '.';
  int8_t h =  (d/100);
  buffer[1] = h +'0';
  int8_t t =  (d-h*100)/10;
  buffer[2] = t +'0';
  int8_t u =  (d-h*100-t*10);
  buffer[3] = u +'0';
  buffer[4] = ' ';
  buffer[5] = sign;
  buffer[6] = 0;
}
        
/*
bool TFTLatLonBox::updateString(TFT_eSPI *tft, int x, int y, const char *prev, const char* current, bool force,  bool updated) {
  if ( force || strcmp(prev, current) != 0) {
    tft->setTextColor(TFT_BLACK , TFT_BLACK);
    tft->drawString(prev, x, y, 4);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->drawString(current, x, y, 4); // Value in middle, Font 8 only as numbers, no letters.
    return true;    
  }
  return updated;
} 
*/ 

void TFTLatLonBox::update(TFT_eSPI *tft, float lat, float lon, bool firstPaint) {  
  if ( ! (firstPaint || lat != previousLat || lon != previousLon) ) {
    // no change, dontupdate
    return;
  }

#define degWidth    42
#define degSymWidth 13
#define minWidth    28
#define minSymWidth 3
#define minDecWidth 72
#define fontHeight  26

#ifndef degWidth
  int degWidth = tft->textWidth("000",4);
  int degSymWidth = tft->textWidth("o",2)+5;
  int minWidth = tft->textWidth("00",4);
  int minSymWidth = tft->textWidth("'",2);
  int minDecWidth = tft->textWidth(".000 N",4);
  int fontHeight = tft->fontHeight(4);

  if ( firstPaint ) {
    Serial.print("#define degWidth    "); Serial.println(degWidth);
    Serial.print("#define degSymWidth "); Serial.print(degSymWidth);
    Serial.print("#define minWidth    "); Serial.print(minWidth);
    Serial.print("#define minSymWidth "); Serial.print(minSymWidth);
    Serial.print("#define minDecWidth "); Serial.print(minDecWidth);
    Serial.print("#define fontHeight  "); Serial.println(fontHeight);
  }
#endif

  // right aligned
  int degx = degWidth-((degWidth+degSymWidth+minWidth+minSymWidth+minDecWidth)/2);
  // left aligned
  int minx = degx+degSymWidth;
  int minDecx = minx+minWidth+minSymWidth;


  degx = degx+(width/2);
  minx = minx+(width/2);
  minDecx = minDecx+(width/2);
  int laty = height/2;
  int lony = height/2;


  char buffer[10];


  // setup 1 bit sprite for the whole widget.
  // and draw it all, no conditionals.
  TFT_eSprite textSprite = TFT_eSprite(tft);
  textSprite.setColorDepth(1);
  textSprite.createSprite(width, height);
  textSprite.setTextColor(TFT_WHITE, TFT_BLACK);


  formatDeg(lat, buffer, true);
  textSprite.setTextDatum(BR_DATUM);
  textSprite.drawString(buffer, degx, laty, 4); 

  
  formatMin(lat, buffer, true);
  textSprite.setTextDatum(BL_DATUM);
  textSprite.drawString(buffer, minx, laty, 4); 
  
  formatMinDec(lat, buffer, true);
  textSprite.drawString(buffer, minDecx, laty, 4); 
  textSprite.setTextDatum(TL_DATUM);
  textSprite.drawString("o", degx, laty-fontHeight-5, 2); 
  textSprite.drawString("'", minx+minWidth, laty-fontHeight, 2); 

  formatDeg(lon, buffer, false);
  textSprite.setTextDatum(TR_DATUM);
  textSprite.drawString(buffer, degx, lony, 4); 

  formatMin(lon, buffer, false);
  textSprite.setTextDatum(TL_DATUM);
  textSprite.drawString(buffer, minx, lony, 4); 

  formatMinDec(lon, buffer, false);
  textSprite.drawString(buffer, minDecx, lony, 4); 
  textSprite.drawString("o", degx, lony-5, 2); 
  textSprite.drawString("'", minx+minWidth, lony, 2);

  textSprite.drawRoundRect(0, 0, width, height, 5, TFT_WHITE);
  textSprite.setTextDatum(BL_DATUM);
  textSprite.drawString("FIX", 5, height-5, 2); // Value in middle, Font 8 only as numbers, no letters.
  textSprite.setTextDatum(BR_DATUM);
  textSprite.drawString("d m.m", width-5, height-5, 2); // Value in middle, Font 8 only as numbers, no letters.

  tft->setBitmapColor(TFT_WHITE, TFT_BLACK); 
  textSprite.pushSprite(x, y);

  previousLat = lat;
  previousLon = lon;

}







// #########################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// #########################################################################

int TFTWidgets::ringMeter(TFT_eSPI *tft,int value, int vmin, int vmax, int x, int y, int r, const char *units, byte scheme)
{
  // Minimum value of r is about 52 before value text intrudes on ring
  // drawing the text first is an option
  
  x += r; y += r;   // Calculate coords of centre of ring

  int w = r / 3;    // Width of outer ring is 1/4 of radius
  
  int angle = 150;  // Half the sweep angle of meter (300 degrees)

  int v = map(value, vmin, vmax, -angle, angle); // Map the value to an angle v

  byte seg = 3; // Segments are 3 degrees wide = 100 segments for 300 degrees
  byte inc = 6; // Draw segments every 3 degrees, increase to 6 for segmented ring

  // Variable to save "value" text colour from scheme and set default
  int colour = TFT_BLUE;
 
  // Draw colour blocks every inc degrees
  for (int i = -angle+inc/2; i < angle-inc/2; i += inc) {
    // Calculate pair of coordinates for segment start
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (r - w) + x;
    uint16_t y0 = sy * (r - w) + y;
    uint16_t x1 = sx * r + x;
    uint16_t y1 = sy * r + y;

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * 0.0174532925);
    float sy2 = sin((i + seg - 90) * 0.0174532925);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;

    if (i < v) { // Fill in coloured segments with 2 triangles
      switch (scheme) {
        case 0: colour = TFT_RED; break; // Fixed colour
        case 1: colour = TFT_GREEN; break; // Fixed colour
        case 2: colour = TFT_BLUE; break; // Fixed colour
        case 3: colour = TFTWidgets::rainbow(map(i, -angle, angle, 0, 127)); break; // Full spectrum blue to red
        case 4: colour = TFTWidgets::rainbow(map(i, -angle, angle, 70, 127)); break; // Green to red (high temperature etc)
        case 5: colour = TFTWidgets::rainbow(map(i, -angle, angle, 127, 63)); break; // Red to green (low battery etc)
        default: colour = TFT_BLUE; break; // Fixed colour
      }
      tft->fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      tft->fillTriangle(x1, y1, x2, y2, x3, y3, colour);
      //text_colour = colour; // Save the last colour drawn
    }
    else // Fill in blank segments
    {
      tft->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREY);
      tft->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREY);
    }
  }
  // Convert value to a string
  char buf[10];
  byte len = 3; if (value > 999) len = 5;
  dtostrf(value, len, 0, buf);
  buf[len] = ' '; buf[len+1] = 0; // Add blanking space and terminator, helps to centre text too!
  // Set the text colour to default
  tft->setTextSize(1);

/*
  if (value<vmin || value>vmax) {
    TFTWidgets::drawAlert(tft,x,y+90,50,1);
  }
  else {
    TFTWidgets::drawAlert(tft,x,y+90,50,0);
  }
*/
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  // Uncomment next line to set the text colour to the last segment value!
  tft->setTextColor(colour, TFT_BLACK);
  tft->setTextDatum(MC_DATUM);
  // Print value, if the meter is large then use big font 8, othewise use 4
  if (r > 84) {
    tft->setTextPadding(55*3); // Allow for 3 digits each 55 pixels wide
    tft->drawString(buf, x, y, 6); // Value in middle
  }
  else {
    tft->setTextPadding(3 * 14); // Allow for 3 digits each 14 pixels wide
    tft->drawString(buf, x, y, 4); // Value in middle
  }
  tft->setTextSize(1);
  tft->setTextPadding(0);
  // Print units, if the meter is large then use big font 4, othewise use 2
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  if (r > 84) tft->drawString(units, x, y + 30, 4); // Units display
  else tft->drawString(units, x, y + 15, 2); // Units display

  // Calculate and return right hand side x coordinate
  return x + r;
}

/*
static void TFTWidgets::drawAlert(TFT_eSPI *tft, int x, int y , int side, bool draw)
{
  if (draw && !range_error) {
    drawIcon(tft, alert, x - alertWidth/2, y - alertHeight/2, alertWidth, alertHeight);
    range_error = 1;
  }
  else if (!draw) {
// alert flickers on smaller meters.    tft.fillRect(x - alertWidth/2, y - alertHeight/2, alertWidth, alertHeight, TFT_BLACK);
    range_error = 0;
  }
}
*/

// #########################################################################
// Return a 16 bit rainbow colour
// #########################################################################
unsigned int TFTWidgets::rainbow(byte value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red

  byte red = 0; // Red is the top 5 bits of a 16 bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits

  byte quadrant = value / 32;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}

// #########################################################################
// Return a value in range -1 to +1 for a given phase angle in degrees
// #########################################################################
float TFTWidgets::sineWave(int phase) {
  return sin(phase * 0.0174532925);
}


//====================================================================================
// This is the function to draw the icon stored as an array in program memory (FLASH)
//====================================================================================

// To speed up rendering we use a 64 pixel buffer
#define BUFF_SIZE 64

// Draw array "icon" of defined width and height at coordinate x,y
// Maximum icon size is 255x255 pixels to avoid integer overflow

void TFTWidgets::drawIcon(TFT_eSPI *tft, const unsigned short* icon, int16_t x, int16_t y, int8_t width, int8_t height) {

  uint16_t  pix_buffer[BUFF_SIZE];   // Pixel buffer (16 bits per pixel)

  tft->startWrite();

  // Set up a window the right size to stream pixels into
  tft->setAddrWindow(x, y, width, height);

  // Work out the number whole buffers to send
  uint16_t nb = ((uint16_t)height * width) / BUFF_SIZE;

  // Fill and send "nb" buffers to TFT
  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < BUFF_SIZE; j++) {
      pix_buffer[j] = pgm_read_word(&icon[i * BUFF_SIZE + j]);
    }
    tft->pushColors(pix_buffer, BUFF_SIZE);
  }

  // Work out number of pixels not yet sent
  uint16_t np = ((uint16_t)height * width) % BUFF_SIZE;

  // Send any partial buffer left over
  if (np) {
    for (int i = 0; i < np; i++) pix_buffer[i] = pgm_read_word(&icon[nb * BUFF_SIZE + i]);
    tft->pushColors(pix_buffer, np);
  }

  tft->endWrite();
}
/*

static void TFTWidgets::textBox(TFT_eSPI *tft, float value, int x, int y, int w, const char *format, const char *title, const char * units) {
  tft->fillRect(x, y, 239, 126, TFT_WHITE);
  tft->fillRect(x+5, y+3, 230, 119, TFT_WHITE);



}



// #########################################################################
//  Draw the analogue meter on the screen
// #########################################################################
static void TFTWidgets::analogMeter(TFT_eSPI *tft, int x, int y)
{
  // Meter outline
  tft->fillRect(x, y, 239, 126, TFT_GREY);
  tft->fillRect(x+5, y+3, 230, 119, TFT_WHITE);

  tft->setTextColor(TFT_BLACK);  // Text colour

  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for (int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 15;

    // Coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = x + sx * (100 + tl) + 120;
    uint16_t y0 = y + sy * (100 + tl) + 140;
    uint16_t x1 = x + sx * 100 + 120;
    uint16_t y1 = y + sy * 100 + 140;

    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = x + sx2 * (100 + tl) + 120;
    int y2 = y + sy2 * (100 + tl) + 140;
    int x3 = x + sx2 * 100 + 120;
    int y3 = y + sy2 * 100 + 140;

    // Yellow zone limits
    //if (i >= -50 && i < 0) {
    //  tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_YELLOW);
    //  tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_YELLOW);
    //}

    // Green zone limits
    if (i >= 0 && i < 25) {
      tft->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREEN);
      tft->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREEN);
    }

    // Orange zone limits
    if (i >= 25 && i < 50) {
      tft->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_ORANGE);
      tft->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_ORANGE);
    }

    // Short scale tick length
    if (i % 25 != 0) tl = 8;

    // Recalculate coords incase tick lenght changed
    x0 = x + sx * (100 + tl) + 120;
    y0 = y + sy * (100 + tl) + 140;
    x1 = x + sx * 100 + 120;
    y1 = y + sy * 100 + 140;

    // Draw tick
    tft->drawLine(x0, y0, x1, y1, TFT_BLACK);

    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = x + sx * (100 + tl + 10) + 120;
      y0 = y + sy * (100 + tl + 10) + 140;
      switch (i / 25) {
        case -2: tft->drawCentreString("0", x0, y0 - 12, 2); break;
        case -1: tft->drawCentreString("25", x0, y0 - 9, 2); break;
        case 0: tft->drawCentreString("50", x0, y0 - 6, 2); break;
        case 1: tft->drawCentreString("75", x0, y0 - 9, 2); break;
        case 2: tft->drawCentreString("100", x0, y0 - 12, 2); break;
      }
    }

    // Now draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = x + sx * 100 + 120;
    y0 = y + sy * 100 + 140;
    // Draw scale arc, don't draw the last part
    if (i < 50) tft->drawLine(x0, y0, x1, y1, TFT_BLACK);
  }

  tft->drawString("%RH", x + 5 + 230 - 40, y + 119 - 20, 2); // Units at bottom right
  tft->drawCentreString("%RH", x + 120, y + 70, 4); // Comment out to avoid font 4
  tft->drawRect(x + 5, y + 3 , 230, 119, TFT_BLACK); // Draw bezel line
}

// #########################################################################
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
// #########################################################################
static int TFTWidgets::plotNeedle(TFT_eSPI *tft, int value, int displayed_value, 
        int minValue, int maxValue, int x, int y, byte ms_delay)
{
  tft->setTextColor(TFT_BLACK, TFT_WHITE);
  char buf[8]; dtostrf(value, 4, 0, buf);
  tft->drawRightString(buf, 40, 119 - 20, 2);

  if (value < minValue) value = minValue; // Limit value to emulate needle end stops
  if (value > maxValue) value = maxValue;



  float sdeg = map(displayed_value, minValue, maxValue, -150, -30); // Map value to angle
  float sx = cos(sdeg * 0.0174532925);
  float sy = sin(sdeg * 0.0174532925);
  float ltx = tan((sdeg + 90) * 0.0174532925);
  float osx = x + sx * 98 + 120;
  float osy = x + sy * 98 + 140;

  // Move the needle util new value reached
  while (!(value == displayed_value)) {
    if (displayed_value < value) displayed_value++;
    else displayed_value--;

    if (ms_delay == 0) displayed_value = value; // Update immediately id delay is 0

    sdeg = map(displayed_value, minValue, maxValue, -150, -30); // Map value to angle
    // Calcualte tip of needle coords
    sx = cos(sdeg * 0.0174532925);
    sy = sin(sdeg * 0.0174532925);

    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    // Erase old needle image
    tft->drawLine(x + 120 + 20 * ltx - 1, y + 140 - 20, osx - 1, osy, TFT_WHITE);
    tft->drawLine(x + 120 + 20 * ltx, y + 140 - 20, osx, osy, TFT_WHITE);
    tft->drawLine(x + 120 + 20 * ltx + 1, y +  140 - 20, osx + 1, osy, TFT_WHITE);

    // Re-plot text under needle
    tft->setTextColor(TFT_BLACK);
    tft->drawCentreString("%RH", x + 120, y + 70, 4); // // Comment out to avoid font 4

    // Store new needle end coords for next erase
    ltx = tx;
    osx = x + sx * 98 + 120;
    osy = x + sy * 98 + 140;

    // Draw the needle in the new postion, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    tft->drawLine(x + 120 + 20 * ltx - 1, y + 140 - 20, osx - 1, osy, TFT_RED);
    tft->drawLine(x + 120 + 20 * ltx, y + 140 - 20, osx, osy, TFT_MAGENTA);
    tft->drawLine(x + 120 + 20 * ltx + 1, y + 140 - 20, osx + 1, osy, TFT_RED);

    // Slow needle down slightly as it approaches new postion
    if (abs(displayed_value - value) < 10) ms_delay += ms_delay / 5;

    // Wait before next update
    delay(ms_delay);
  }
  return displayed_value;
}

// #########################################################################
//  Draw a linear meter on the screen
// #########################################################################
void TFTLinearMeter::linearMeters(TFT_eSPI *tft, const char **label, int n, int x, int y)
{
  int w = 36;

  for (int i = 0; i < n; i++)  {
    int xp = x+i*40;
    tft->drawRect(xp, y, w, 155, TFT_GREY);
    tft->fillRect(xp+2, y + 19, w-3, 155 - 38, TFT_WHITE);
    tft->setTextColor(TFT_CYAN, TFT_BLACK);
    tft->drawCentreString(label[i], x + w / 2, y + 2, 2);

    for (int i = 0; i < 110; i += 10)
    {
      tft->drawFastHLine(xp + 20, y + 27 + i, 6, TFT_BLACK);
    }

    for (int i = 0; i < 110; i += 50)
    {
      tft->drawFastHLine(xp + 20, y + 27 + i, 9, TFT_BLACK);
    }
    
    tft->fillTriangle(xp+3, y + 127, xp+3+16, y+127, xp + 3, y + 127 - 5, TFT_RED);
    tft->fillTriangle(xp+3, y + 127, xp+3+16, y+127, xp + 3, y + 127 + 5, TFT_RED);
    
    tft->drawCentreString("---", xp + w / 2, y + 155 - 18, 2);
  }
}

// #########################################################################
//  Adjust 6 linear meter pointer positions
// #########################################################################
void TFTLinearMeter::plotLinearPointers(TFT_eSPI *tft, int * value, int *display_value, int n, int x, int y)
{
  int dy = 187;
  byte pw = 16;


  // todo, make it possible to possition the pointers so they match the linear meterx x and y.
  tft->setTextColor(TFT_GREEN, TFT_BLACK);

  // Move the 6 pointers one pixel towards new value
  for (int i = 0; i < n; i++)
  {
    char buf[8]; dtostrf(value[i], 4, 0, buf);
    tft->drawRightString(buf, i * 40 + 36 - 5, 187 - 27 + 155 - 18, 2);

    int dx = 3 + 40 * i;
    if (value[i] < 0) value[i] = 0; // Limit value to emulate needle end stops
    if (value[i] > 100) value[i] = 100;

    while (!(value[i] == display_value[i])) {
      dy = 187 + 100 - display_value[i];
      if (old_value[i] > value[i])
      {
        tft->drawLine(dx, dy - 5, dx + pw, dy, TFT_WHITE);
        display_value[i]--;
        tft->drawLine(dx, dy + 6, dx + pw, dy + 1, TFT_RED);
      }
      else
      {
        tft->drawLine(dx, dy + 5, dx + pw, dy, TFT_WHITE);
        display_value[i]++;
        tft->drawLine(dx, dy - 6, dx + pw, dy - 1, TFT_RED);
      }
    }
  }
}
*/


void TFTSplash::display(TFT_eSPI *tft) {
    tft->setTextColor(TFT_BLACK, TFT_WHITE);
    tft->fillScreen(TFT_WHITE);
    tft->setSwapBytes(true); // We need to swap the colour bytes (endianess)
    // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
    TJpgDec.setJpgScale(1);
    // create a lambda to call back to
    tft_output_config.tft = tft;
    tft_output_config.x_offset = 0;
    tft_output_config.y_offset = 0;
    // The decoder must be given the exact name of the rendering function above
    TJpgDec.setCallback(tft_output);
    // Draw the image, top left at 0,0
    TJpgDec.drawFsJpg(0, 0, fileName);
    tft_output_config.tft = NULL;
}


void TFTDial::drawBackground(TFT_eSPI *tft) {
    tft->setSwapBytes(true); // We need to swap the colour bytes (endianess)
    tft_output_config.tft = tft;
    tft_output_config.x_offset = x;
    tft_output_config.y_offset = y;
    TJpgDec.setCallback(tft_output);


    loadJpg();

    tft_output_config.tft = NULL;
    tft->setSwapBytes(false); // We need to swap the colour bytes (endianess)
}

void TFTDial::dispose() {
    if ( this->needle != NULL) {
      delete(this->needle);
      this->needle = NULL;
    }
}

void TFTDial::update(TFT_eSPI *tft, float value,  bool firstPaint) {
  if ( firstPaint ) {
 
    if ( needle == NULL ) {
      needle = new TFT_eSprite(tft);

      needle->setColorDepth(16);
      needle->createSprite(needleWidth, needleLength);  // create the needle Sprite
      needle->fillSprite(TFT_BLACK); // Fill with black

      // Define needle pivot point relative to top left corner of Sprite
      uint16_t piv_x = needleWidth / 2; // pivot x in Sprite (middle)
      uint16_t piv_y = needleRadius;    // pivot y in Sprite
      needle->setPivot(piv_x, piv_y);     // Set pivot point in this Sprite

      // Draw the red needle in the Sprite
      needle->fillRect(0, 0, needleWidth, needleLength, TFT_MAROON);
      needle->fillRect(1, 1, needleWidth-2, needleLength-2, TFT_RED);
    }

  }


    // redraw the area to update in a sprite and push it to the image.
    // the area to update needs to cover the old needle and the new.
    // we need to draw to the sprite the background and the rotaited needle
    // reading from the display to fetch the background image is not reliable
    // on a ILI9488 which is why the TFT_eSPI animated needle doesnt work for me.
    // There is not enough memory to write the whole jpeg image (312x312x2=194K)
    // into a sprite, but there is enough to write, small needle movements.
  // Sprites and screen have to be drawn 16 bit.


  int16_t angle = getNeedleAngle(value);
  if ( angle != previousAngle ) {


    int16_t min_x, min_y, max_x, max_y;
    // before updating the angle, get the bounds of the current needle relative to the screen
    tft->setPivot(x+(width/2)+needleOffsetX, y+(height/2)+needleOffsetY);
    needle->getRotatedBounds(previousAngle, &min_x, &min_y, &max_x, &max_y);

    previousAngle += (angle-previousAngle)/6;
    if ( previousAngle < angle ) previousAngle++;
    if ( previousAngle > angle ) previousAngle--;

    //previousAngle  = angle;

    // get the bounds of the new needle
    int16_t min_x1, min_y1, max_x1, max_y1;
    needle->getRotatedBounds(previousAngle, &min_x1, &min_y1, &max_x1, &max_y1);

    // find the bounds of both needles in screen co-ordinates.
    if ( min_x1 < min_x ) min_x = min_x1;
    if ( min_y1 < min_y ) min_y = min_y1;
    if ( max_x1 > max_x ) max_x = max_x1;
    if ( max_y1 > max_y ) max_y = max_y1;


    // translate screen co-ordinates to widget co-ordinates.
    min_x = min_x - x;
    max_x = max_x - x;
    min_y = min_y - y;
    max_y = max_y - y;

    min_x -= 2;
    min_y -= 2;
    max_x += 2;
    max_y += 2;
    if ( min_x < 0 ) min_x = 0;
    if ( min_y < 0 ) min_y = 0;

    int16_t uw = max_x-min_x;
    int16_t uh = max_y-min_y;

    // The sprite will be uw, uh
    // the origin of the sprite relative to the jpg is at min_x, min_y
    // so the jpg must be shifted to -min_x, -min_y when drawing onto the 
    // sprite.
    //


    // check the size of the sprite bitmap
    if ( firstPaint || (uw*uh*2) > 20000 ) {
      // fill background, needle can come on next call.
      drawBackground(tft);
      Serial.print("tobig:");Serial.print(uw*uh*2);
    } else {
      // use sprite.
      TFT_eSprite background = TFT_eSprite(tft);
      background.setColorDepth(16);
      background.createSprite(uw, uh);  // create the background Sprite
      background.fillSprite(TFT_BLACK); // Fill with black


      background.setSwapBytes(true); // We need to swap the colour bytes (endianess)
      tft_output_config.sprite = &background;
      // offset the output back to the widget origin relative to sprite bounds.
      tft_output_config.x_offset = -min_x;
      tft_output_config.y_offset = -min_y;
      TJpgDec.setCallback(sprite_output);

      loadJpg();      
     
      tft_output_config.sprite = NULL;
      background.setSwapBytes(false); // We need to swap the colour bytes (endianess)



      // set the pivot point relative to the backgound sprite origin.
      //155-138
      // set the pivot relative to the sprite co-ordrinates.
      background.setPivot((width/2)-min_x+needleOffsetX, (height/2)-min_y+needleOffsetY);
      needle->pushRotated(&background, previousAngle, TFT_BLACK);
      // In screen co-ordinates.
      background.pushSprite(x+min_x, y+min_y);



//      Serial.print("nx:");Serial.print(x+min_x);
//      Serial.print("ny:");Serial.print(y+min_y);
    }
//    Serial.print(" value:");Serial.print(value);
//    Serial.print(" angle:");Serial.print(previousAngle);
//    Serial.print(" min_x:");Serial.print(min_x);
//    Serial.print(" min_y:");Serial.print(min_y);
//    Serial.print(" uw:");Serial.print(uw);
//    Serial.print(" uh:");Serial.println(uh);

  }
  /*

 rpm: 0 fuel: 0 coolant: 40
nx:84ny:163 value:0.00 angle:-120 min_x:74 min_y:153 uw:155 uh:38
 rpm: 1000 fuel: 50 coolant: 80
nx:75ny:117 value:1000.00 angle:-69 min_x:65 min_y:107 uw:61 uh:47
rpm: 500 fuel: 25 coolant: 60
nx:75ny:151 value:500.00 angle:-94 min_x:65 min_y:141 uw:61 uh:49
   rpm: 1500 fuel: 75 coolant: 100
nx:79ny:79 value:1500.00 angle:-35 min_x:69 min_y:69 uw:70 uh:68
   rpm: 2000 fuel: 100 coolant: 120
nx:109ny:65 value:2000.00 angle:0 min_x:99 min_y:55 uw:60 uh:66
rpm: 2500 fuel: 0 coolant: 40
nx:156ny:65 value:2500.00 angle:35 min_x:146 min_y:55 uw:60 uh:58
   rpm: 3000 fuel: 25 coolant: 60
nx:175ny:80 value:3000.00 angle:69 min_x:165 min_y:70 uw:68 uh:51
 rpm: 3500 fuel: 50 coolant: 80

nx:190ny:118 value:3500.00 angle:96 min_x:180 min_y:108 uw:59 uh:30
 rpm: 4000 fuel: 75 coolant: 100
nx:185ny:150 value:4000.00 angle:120 min_x:175 min_y:140 uw:64 uh:18
*/

}

void TFTTachometer::updateLCD(TFT_eSPI *tft, 
    float *values,  bool firstPaint) {

    unsigned long now = millis();
    if ( now > lastLCDUpdate + 500 ) {
      if ( now > lastLCDChange + 5000 ) {
        lcdView++;
        if ( lcdView > 3) lcdView = 0;
        lastLCDChange = now;
      } 
      int16_t uw = 118;
      int16_t uh = 50;
      int16_t posx = 93;  // offset from dial background
      int16_t posy = 192;

      // build the LCD background which is a subset of the dial background.
      TFT_eSprite background = TFT_eSprite(tft);
      background.setColorDepth(16);
      background.createSprite(uw, uh);  // create the background Sprite
      background.fillSprite(TFT_BLACK); // Fill with black


      background.setSwapBytes(true); // We need to swap the colour bytes (endianess)
      tft_output_config.sprite = &background;
      // offset the output back to the widget origin relative to sprite bounds.
      tft_output_config.x_offset = 0;
      tft_output_config.y_offset = 0;
      TJpgDec.setCallback(sprite_output);

      TJpgDec.drawJpg(0,0,jpg_tacho_lcd, sizeof(jpg_tacho_lcd));
     
      tft_output_config.sprite = NULL;
      background.setSwapBytes(false); // We need to swap the colour bytes (endianess)


      // 3 rows, 
      // vertical alignment top, center, bottom with 5 padding
      // horizontal left, center, right with 5 padding
      // rpm   blank  alternator Voltage
      // coolant  stw   alternator current
      // fule engine V service V


      char buffer[20];
      background.setTextColor(TFT_WHITE);
      switch(lcdView) {
        case 0:
          sprintf(buffer, "%4.0frpm",values[0]);
          background.setTextDatum(TC_DATUM);
          background.drawString(buffer, uw/2, 1, 4);

          sprintf(buffer, "%4.1fkn",values[3]);
          background.setTextDatum(BC_DATUM);
          background.drawString(buffer, uw/2, uh-1, 4);
          break;
        case 1:
          sprintf(buffer, "tmp %3.0fC",values[1]);
          background.setTextDatum(TC_DATUM);
          background.drawString(buffer, uw/2, 1, 4);

          sprintf(buffer, "fuel %3.0f%%",values[2]);
          background.setTextDatum(BC_DATUM);
          background.drawString(buffer, uw/2, uh-1, 4);
          break;
        case 2:
          sprintf(buffer, "alt %5.2fV",values[5]);
          background.setTextDatum(TC_DATUM);
          background.drawString(buffer, uw/2, 1, 4);

          sprintf(buffer, "chg %5.1fA",values[6]);
          background.setTextDatum(BC_DATUM);
          background.drawString(buffer, uw/2, uh-1, 4);
          break;
        case 3:
          sprintf(buffer, "eb %5.2fV",values[4]);
          background.setTextDatum(TC_DATUM);
          background.drawString(buffer, uw/2, 1, 4);

          sprintf(buffer, "sb %5.2fV",values[7]);
          background.setTextDatum(BC_DATUM);
          background.drawString(buffer, uw/2, uh-1, 4);
          break;
      }
      
      background.pushSprite(posx+x, posy+y);

    }

}


void TFTTachometer::loadJpg() {
  TJpgDec.drawJpg(0,0,jpg_tacho_gauge, sizeof(jpg_tacho_gauge));
}

int16_t TFTTachometer::getNeedleAngle(float value) {
  
  // the needle angle is non linear vs rpm
  // numbers measured from the image.
  // if the image changes the numbers need to change.
  // the error is < 1 degree for the interpolations here
  if ( value < 0 ) {
    return -120;
  } else if ( value < 500 ) {
    return round((0.0485187634*value)-120);
  } else if ( value < 1000 ) {
    return round((0.0508696651*(value-500))-94.3510779516);  
  } else if (value < 1500 ) {
    return round((0.068529859*(value-1000))-68.9162454176);
  } else if (value < 2000 ) {
    return round((0.0693026318*(value-1500))-34.651315912);
  } else if (value < 2500 ) {
    return round((0.0693026318*(value-2500))+34.651315912);
  } else if (value < 3000 ) {
    return round((0.068529859*(value-3000))+68.9162454176);
  } else if (value < 3500 ) {
    return round((0.0508696651*(value-3500))+94.3510779516);
  } else if (value < 4000 ) {
    return round((0.0485187634*(value-4000))+120);
  } else {
    return 120;
  }

  /*
  int16_t angle = (int16_t)(-120.0+((240.0)*(value)/(4000.0)));
  if ( angle < -120 ) angle = -120;
  else if (angle > 120) angle = 120;
  return angle;
  */
  
}



void TFTFuelGauge::loadJpg() {
  TJpgDec.drawJpg(0,0,jpg_fuel_gauge, sizeof(jpg_fuel_gauge));
}

int16_t TFTFuelGauge::getNeedleAngle(float value) {
  int16_t angle = (int16_t)(-45.0+((87.0)*(value)/(100.0)));
  if ( angle < -45 ) angle = -45;
  else if (angle > 42) angle = 42;
  return angle;
}

void TFTCoolantGauge::loadJpg() {
  TJpgDec.drawJpg(0,0,jpg_coolant_gauge, sizeof(jpg_coolant_gauge));
}

int16_t TFTCoolantGauge::getNeedleAngle(float value) {
  // scale is non linear,
  // neasured from image.
  if ( value < 40 ) {
    return -45;
  } else if ( value < 60 ) {
    return round((0.4528944064*(value-40))-45);
  } else if ( value < 80 ) {
    return round((0.8381551923*(value-60))-35.9421118714);
  } else if ( value < 100 ) {
    return round((1.6175780725*(value-80))-19.1790080258);
  } else if ( value < 120 ) {
    return round((1.4179608659*(value-100))+13.1725534233);
  } else {
    return 44;
  }

/*
  int16_t angle = (int16_t)(-43.0+((86.0)*(value-40.0)/(80.0)));
  if ( angle < -43 ) angle = -43;
  else if (angle > 43) angle = 43;
  return angle;
  */
}

// in Arduino.h #define DEG_TO_RAD 0.0174532925

void TFTSailing::rotatePoints(int16_t *x, int16_t *y, int8_t n, int16_t a, int16_t tx, int16_t ty) {
  float sinA = sin(a*DEG_TO_RAD);
  float cosA = cos(a*DEG_TO_RAD);
  for (int i = 0; i < n; i++) {
    float rx = x[i]*cosA - y[i]*sinA;
    float ry = y[i]*cosA + x[i]*sinA;
    x[i] = rx+tx;
    y[i] = ry+ty;
  }
}

void TFTSailing::drawWindpointer(TFT_eSprite *dial, int16_t a, int16_t *ah, int8_t nah, int16_t cx, int16_t cy, int8_t pallet_idx) {
  int16_t tx[4];
  int16_t ty[4];
  tx[0] =  0;
  tx[1] = 10;
  tx[2] = -10;
  tx[3] = 0;
  ty[0] = -inner_r;
  ty[1] = 15-inner_r;
  ty[2] = 15-inner_r;
  ty[3] = 20-inner_r;
  rotatePoints(tx,ty,4,a, cx, cy);
  dial->fillTriangle(
    tx[0],ty[0],
    tx[1],ty[1],
    tx[2],ty[2],
    pallet_idx
  );
  dial->fillTriangle(
    tx[1],ty[1],
    tx[2],ty[2],
    tx[3],ty[3],
    pallet_idx
  );

  tx[0] = 0; ty[0] = -inner_r+20;
  rotatePoints(tx,ty,1,a, cy, cy);
  tx[1] = tx[0]; ty[1] = ty[0];  
  int16_t hlen = ((uw*47)/100)/10;
  for(int i = 0; i < nah; i++) {
    tx[0] = 0; ty[0] = -inner_r+(i+1)*hlen;
    rotatePoints(tx, ty, 1, ah[i], cy, cy);
    dial->drawLine(tx[1],ty[1],tx[0], ty[0], pallet_idx);
    tx[1] = tx[0];
    ty[1] = ty[0];
  }  

}

void TFTSailing::drawSector(TFT_eSprite *dial, int16_t a, int16_t cx, int16_t cy, int8_t pallet_idx) {
  int16_t tx[3];
  tx[0] = -5;
  tx[1] = 5;
  tx[2] = 0;
  int16_t ty[3];
  ty[0] = -inner_r;
  ty[1] = -inner_r;
  ty[2] = 0;
  rotatePoints(tx,ty,3,a, cy, cy);
  dial->fillTriangle(
    tx[0], ty[0],
    tx[1], ty[1],
    tx[2], ty[2],
    pallet_idx);
}



void TFTSailing::drawBoat(TFT_eSprite *dial, int16_t cx, int16_t cy, int8_t fill, int8_t outline) {
#define BOAT_HEIGHT 78
#define BOAT_SEG_SPACE 7
  int16_t ys = cy-BOAT_HEIGHT/2;
  // total height = 77
  // 7 pixel spacing
  uint8_t segwidth[12] = {
    13,
    15,  // 7
    17,  // 14
    18,  // 21
    19,  // 28
    19,  // 35
    18,  // 42
    17,  // 49
    15,  // 56
    12,  // 63
    6,  // 70
    0    // 77
  };
  
  // fill the shape by drawing horizontal lines, which are fast,
  // using widths interpolated from measurements every 7 pixels.
  // segwidth is half width to ensure center line remains straight.
  for (int16_t ypx = 1; y < BOAT_HEIGHT; y++ ) {
    int16_t yi = ypx/BOAT_SEG_SPACE;
    int16_t xw = ((segwidth[yi+1]-segwidth[yi])*(ypx-yi*BOAT_SEG_SPACE))/BOAT_SEG_SPACE;
    dial->drawFastHLine(cx-xw, ys+ypx, xw*2, fill);      
  }

  // draw the outline using lines.
  dial->drawFastHLine(cx-segwidth[0], ys, segwidth[0]*2, outline);
  for (int i = 1; i < 12; i++) {
    dial->drawLine(cx-segwidth[i-1],ys+BOAT_SEG_SPACE*(i-1),cx-segwidth[i],ys+BOAT_SEG_SPACE*(i), outline);
    dial->drawLine(cx+segwidth[i-1],ys+BOAT_SEG_SPACE*(i-1),cx+segwidth[i],ys+BOAT_SEG_SPACE*(i), outline);
  }
}






void TFTSailing::drawDial(TFT_eSPI *tft, 
    int sx, // sprite x offset from dial origin
    int sy, // sprite y offset from dial origin
    int sw, // sprite witdth
    int sh  // sprite height
    ) {
    TFT_eSprite dial = TFT_eSprite(tft);
    dial.setColorDepth(4);
    uint16_t palette[16];
#define PLT_BLACK 0
#define PLT_ORANGE 1
#define PLT_PURPLE 5
#define PLT_DARKGREY 7
#define PLT_GREY 8
#define PLT_BLUE 9
#define PLT_GREEN 10
#define PLT_RED 12
#define PLT_YELLOW 14
#define PLT_WHITE 15

    palette[PLT_BLACK]  = TFT_BLACK;
    palette[PLT_ORANGE]  = TFT_ORANGE;
    palette[2]  = TFT_DARKGREEN;
    palette[3]  = TFT_DARKCYAN;
    palette[4]  = TFT_MAROON;
    palette[PLT_PURPLE]  = TFT_PURPLE;
    palette[6]  = TFT_OLIVE;
    palette[PLT_DARKGREY]  = TFT_DARKGREY;
    palette[PLT_GREY]  = TFT_GREY;
    palette[PLT_BLUE]  = TFT_BLUE;
    palette[PLT_GREEN] = TFT_GREEN;
    palette[11] = TFT_CYAN;
    palette[PLT_RED] = TFT_RED;
    palette[13] = TFT_NAVY;
    palette[PLT_YELLOW] = TFT_YELLOW;
    palette[PLT_WHITE] = TFT_WHITE; 
    dial.setColorDepth(4);
    dial.createSprite(sw, sh); // 320*320/2, might be a problem, so may have to call this multiple times
    dial.createPalette(palette);




    // use dial origin as drawing origin.
    int16_t cx = uw/2-sx;
    int16_t cy = uh/2-sy;
    int16_t tx[5];
    int16_t ty[5];

    dial.drawCircle(cx, cy, outer_r, PLT_GREY);
    dial.drawCircle(cx, cy, inner_r, PLT_GREY);
    dial.drawCircle(cx, cy, uw/6, PLT_GREY);

    // draw compass dial rotated for heading.
    char deg[4];
    dial.setTextDatum(MC_DATUM);
    for (int i = 0; i < 360; i+=10) {




      if ( i%90 == 0  ) {
        tx[0] = 0; 
        tx[1] = 0; 
        tx[2] = 0; 
        tx[3] = 5; 
        tx[4] = -5;
        ty[0] = -txt_r;
        ty[1] = -outer_r;
        ty[2] = -inner_r;
        ty[3] =  0;
        ty[4] =  0;
        rotatePoints(tx,ty,5,(hdg-i), cx, cy);
        dial.setTextColor(PLT_WHITE);
        if ( i == 0) {
          dial.drawString("N",tx[0],ty[0],2);
        } else {
          itoa(i,deg,10);
          dial.drawString(deg,tx[0],ty[0],2);
        }
        // add the triangle
        dial.fillTriangle(
          tx[2], ty[2],
          tx[3], ty[3],
          ty[4], ty[4],
            PLT_DARKGREY
          );
        // add dots
        dial.fillCircle(tx[1],ty[1],5,PLT_WHITE);
        dial.fillCircle(tx[2],ty[2],5,PLT_WHITE);
      } else if ( i%30 == 0 ) {
        tx[0] = 0;
        ty[0] = -txt_r;
        rotatePoints(tx,ty,1,(hdg-i), cx, cy);
        itoa(i,deg,10);
        dial.setTextColor(PLT_DARKGREY);
        dial.drawString(deg,tx[0],ty[0],2);            
      } else {
        tx[0] = 0;
        ty[0] = -txt_r;
        rotatePoints(tx,ty,1,(hdg-i), cx, cy);
        dial.fillCircle(tx[0],ty[0],5,PLT_WHITE);
      }
    } 

    // draw heading rectangle, allways at the top.
    dial.fillRoundRect(cx-15,0,15,10,4,PLT_WHITE);
    dial.setTextColor(PLT_WHITE);
    dial.setTextDatum(MC_DATUM);
    itoa(hdg,deg,10);
    dial.drawString(deg, cx,5, 2);


    // wind pointers
    drawWindpointer(&dial, twa, twah, ntwah, cx, cy, PLT_RED);
    drawWindpointer(&dial, awa, awah, nawah, cx, cy, PLT_GREEN);

    // port and starboard track
    drawSector(&dial, hdg-portLL, cx, cy, PLT_RED); 
    drawSector(&dial, hdg-stbdLL, cx, cy, PLT_RED);

    // orange pointer
    tx[0] = -5;
    tx[1] = 5;
    tx[2] = 0;
    ty[0] = (-txt_r-outer_r)/2;
    ty[1] = (-txt_r-outer_r)/2;
    ty[2] = -inner_r;
    rotatePoints(tx,ty,3, orangePointerAngle, cy, cy);
    dial.fillTriangle(
      tx[0], ty[0],
      tx[1], ty[1],
      tx[2], ty[2],
      PLT_ORANGE);

    // yellow pointer
    tx[0] = 0;
    tx[1] = 0;
    ty[0] = -inner_r+10;
    ty[1] = -inner_r;
    rotatePoints(tx,ty,2, yellowPointerAngle, cy, cy);
    dial.fillCircle(tx[0], ty[0], 8, PLT_YELLOW);
    dial.drawLine(tx[0], ty[0], tx[1], ty[1], PLT_YELLOW);

    // purple pointer
    tx[0] = 0;
    tx[1] = -3;
    tx[2] = 3;
    tx[3] = 0;
    ty[0] = -outer_r;
    ty[1] = -inner_r;
    ty[2] = -inner_r;
    ty[3] = -inner_r;
    rotatePoints(tx,ty,4, purplePointerAngle, cy, cy);
    dial.fillTriangle(
      tx[0], ty[0],
      tx[1], ty[1],
      tx[2], ty[2],
      PLT_PURPLE);
    dial.fillCircle(tx[3], ty[3], 3, PLT_PURPLE);

    drawBoat(&dial, cx, cy, PLT_BLACK, PLT_GREY);

    // current
    tx[0] = 0;
    tx[1] = -3;
    tx[2] = 3;
    tx[3] = 0;
    tx[4] = 0;
    ty[0] = -10;
    ty[1] = -2;
    ty[2] = -2;
    ty[3] = -2;
    ty[4] = -10;
    rotatePoints(tx,ty,5, currentAngle, cy, cy);
    dial.fillTriangle(
      tx[0], ty[0],
      tx[1], ty[1],
      tx[2], ty[2],
      PLT_BLUE);
    dial.drawWedgeLine(
      tx[3], ty[3],
      tx[4], ty[4],
      6, 7,
      PLT_BLUE
      ); 

    dial.pushSprite(x+sx,y+sy);    
}

void TFTSailing::display(TFT_eSPI *tft, bool firstPaint) {
    drawDial(tft, 0,0,uh,uw);  // may need to iterate over the dial to reduce memory requirements.
}

