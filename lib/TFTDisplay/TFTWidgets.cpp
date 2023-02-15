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

void TFTTachometer::drawBackground(TFT_eSPI *tft) {
    tft->setSwapBytes(true); // We need to swap the colour bytes (endianess)
    tft_output_config.tft = tft;
    tft_output_config.x_offset = 0;
    tft_output_config.y_offset = 0;
    TJpgDec.setCallback(tft_output);
    TJpgDec.drawFsJpg(0, 0, backgroundImage);
    tft_output_config.tft = NULL;
    tft->setSwapBytes(false); // We need to swap the colour bytes (endianess)
}

void TFTTachometer::update(TFT_eSPI *tft, float value,  bool firstPaint) {
  if ( firstPaint ) {
 
#define NEEDLE_LENGTH 62 // Visible length
#define NEEDLE_WIDTH   5  // Width of needle - make it an odd number
#define NEEDLE_RADIUS 84  // Radius at tip

    if ( needle == NULL ) {
      needle = new TFT_eSprite(tft);

      needle->setColorDepth(16);
      needle->createSprite(NEEDLE_WIDTH, NEEDLE_LENGTH);  // create the needle Sprite
      needle->fillSprite(TFT_BLACK); // Fill with black

      // Define needle pivot point relative to top left corner of Sprite
      uint16_t piv_x = NEEDLE_WIDTH / 2; // pivot x in Sprite (middle)
      uint16_t piv_y = NEEDLE_RADIUS;    // pivot y in Sprite
      needle->setPivot(piv_x, piv_y);     // Set pivot point in this Sprite

      // Draw the red needle in the Sprite
      needle->fillRect(0, 0, NEEDLE_WIDTH, NEEDLE_LENGTH, TFT_MAROON);
      needle->fillRect(1, 1, NEEDLE_WIDTH-2, NEEDLE_LENGTH-2, TFT_RED);
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

  int16_t angle = (int16_t)(-120.0+((240.0)*(value)/(4000.0)));


  if ( angle < -120 ) angle = -120;
  else if (angle > 120) angle = 120;
  if ( angle != previousAngle ) {


    int16_t min_x, min_y, max_x, max_y;
    // before updating the angle, get the bounds of the current needle relative to the screen
    tft->setPivot(x+width/2, y+138);
    needle->getRotatedBounds(previousAngle, &min_x, &min_y, &max_x, &max_y);

    previousAngle += (angle-previousAngle)/6;
    if ( previousAngle < angle ) previousAngle++;
    if ( previousAngle > angle ) previousAngle--;

    // get the bounds of the new needle
    int16_t min_x1, min_y1, max_x1, max_y1;
    needle->getRotatedBounds(previousAngle, &min_x1, &min_y1, &max_x1, &max_y1);

    // find the bounds of both needles in screen co-ordinates.
    if ( min_x1 < min_x ) min_x = min_x1;
    if ( min_y1 < min_y ) min_y = min_y1;
    if ( max_x1 > max_x ) max_x = max_x1;
    if ( max_x1 > max_x ) max_x = max_x1;

    // translate to take account of widget origin
    min_x = min_x - x;
    max_x = max_x - x;
    min_y = min_y - y;
    max_y = max_y - y;

    int16_t uw = max_x-min_x;
    int16_t uh = max_y-min_y;




    // check the size of the sprite bitmap
    if ( firstPaint || (uw*uh*2) > 20000 ) {
      // fill background, needle can come on next call.
      drawBackground(tft);
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
      TJpgDec.drawFsJpg(0, 0, backgroundImage);
      tft_output_config.sprite = NULL;
      background.setSwapBytes(false); // We need to swap the colour bytes (endianess)



      // set the pivot point relative to the backgound sprite origin.

      background.setPivot((width/2)-min_x, 138-min_y);
      needle->pushRotated(&background, previousAngle, TFT_BLACK);
      background.pushSprite(min_x, min_y);

      Serial.print("angle:");Serial.print(previousAngle);
      Serial.print(" min_x:");Serial.print(min_x);
      Serial.print(" min_y:");Serial.print(min_y);
      Serial.print(" uw:");Serial.print(uw);
      Serial.print(" uh:");Serial.println(uh);



    }






  }
}




