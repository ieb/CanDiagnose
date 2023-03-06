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
#define PLT_BLACK 0
#define PLT_ORANGE 1
#define PLT_TRANSPARENT 2
#define PLT_PURPLE 5
#define PLT_LIGHTGREY 6
#define PLT_DARKGREY 7
#define PLT_GREY 8
#define PLT_BLUE 9
#define PLT_GREEN 10
#define PLT_RED 12
#define PLT_YELLOW 14
#define PLT_WHITE 15


void TFTSailing::rotatePoints(int16_t *x, int16_t *y, int8_t n, int16_t a, int16_t tx, int16_t ty) {
  float sinA = sin(-a*DEG_TO_RAD);
  float cosA = cos(-a*DEG_TO_RAD);
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
  tx[1] = 8;
  tx[2] = -8;
  tx[3] = 0;
  ty[0] = -inner_r;
  ty[1] = 18-inner_r;
  ty[2] = 18-inner_r;
  ty[3] = 25-inner_r;
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

  tx[0] = 0; ty[0] = -inner_r+25;
  rotatePoints(tx,ty,1,a, cy, cy);
  tx[1] = tx[0]; ty[1] = ty[0];  
  int16_t hlen = ((uw*47)/100)/MAX_WIND_HISTORY;
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



void TFTSailing::drawBoat(TFT_eSprite *dial, int16_t cx, int16_t cy) {
#define BOAT_HEIGHT 78
#define BOAT_SEG_SPACE 7
  int16_t ys = cy+BOAT_HEIGHT/2;
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
  for (int16_t ypx = 1; ypx < BOAT_HEIGHT; ypx++ ) {
    int16_t yi = ypx/BOAT_SEG_SPACE;
    int16_t xw = segwidth[yi]+(((segwidth[yi+1]-segwidth[yi])*(ypx-yi*BOAT_SEG_SPACE))/BOAT_SEG_SPACE);
    dial->drawFastHLine(cx-xw, ys-ypx, xw*2, PLT_BLACK);      
  }

  // draw the outline using lines.
  dial->drawFastHLine(cx-segwidth[0], ys, segwidth[0]*2, PLT_LIGHTGREY);
  for (int i = 1; i < 12; i++) {
    dial->drawLine(cx-segwidth[i-1],ys-BOAT_SEG_SPACE*(i-1),cx-segwidth[i],ys-BOAT_SEG_SPACE*(i), PLT_LIGHTGREY);
    dial->drawLine(cx+segwidth[i-1],ys-BOAT_SEG_SPACE*(i-1),cx+segwidth[i],ys-BOAT_SEG_SPACE*(i), PLT_LIGHTGREY);
  }
}






void TFTSailing::drawDial(TFT_eSPI *tft, 
    int sx, // sprite x offset from dial origin
    int sy, // sprite y offset from dial origin
    int sw, // sprite witdth
    int sh  // sprite height
    ) {
    TFT_eSprite dial = TFT_eSprite(tft);
    uint16_t palette[16];

    // select custom colors using http://www.barth-dev.de/online/rgb565-color-picker/
    palette[PLT_BLACK]  = TFT_BLACK;
    palette[PLT_ORANGE]  = 0xDAA3; //TFT_ORANGE;
    palette[PLT_TRANSPARENT]  = TFT_GREEN;
    palette[3]  = TFT_DARKCYAN;
    palette[4]  = TFT_MAROON;
    palette[PLT_PURPLE]  = 0x780D; //TFT_PURPLE;
    palette[PLT_LIGHTGREY]  = 0x632C; // TFT_LIGHTGREY;
    palette[PLT_DARKGREY]  = TFT_DARKGREY;
    palette[PLT_GREY]  = TFT_GREY;
    palette[PLT_BLUE]  = 0x2CF9; // TFT_BLUE;
    palette[PLT_GREEN] = 0x0566; //TFT_GREEN;
    palette[11] = TFT_CYAN;
    palette[PLT_RED] = 0xD802; //TFT_RED;
    palette[13] = TFT_NAVY;
    palette[PLT_YELLOW] = 0xDE47; //TFT_YELLOW;
    palette[PLT_WHITE] = TFT_WHITE; 
    dial.setColorDepth(4);
    dial.createSprite(sw, sh); // 320*320/2, might be a problem, so may have to call this multiple times
    dial.createPalette(palette);
    dial.fillSprite(PLT_TRANSPARENT);





    // use dial origin as drawing origin.
    int16_t cx = uw/2-sx;
    int16_t cy = uh/2-sy;




    int16_t tx[8];
    int16_t ty[8];

    dial.fillCircle(cx, cy, outer_r, PLT_BLACK);
    dial.drawCircle(cx, cy, outer_r, PLT_GREY);
    dial.drawCircle(cx, cy, inner_r, PLT_GREY);
    dial.drawCircle(cx, cy, uw/5, PLT_GREY);

    // draw compass dial rotated for heading.
    char deg[4];
    dial.setTextDatum(MC_DATUM);
    for (int i = 0; i < 360; i+=10) {




      if ( i%90 == 0  ) {
        tx[0] = 0; 
        tx[1] = 0; 
        tx[2] = 0; 
        tx[3] = 12; 
        tx[4] = -12;
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
          tx[4], ty[4],
            PLT_DARKGREY
          );
        // add dots
        dial.fillCircle(tx[1],ty[1],2,PLT_GREY);
        dial.fillCircle(tx[2],ty[2],2,PLT_GREY);
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
        dial.fillCircle(tx[0],ty[0],2,PLT_GREY);
      }
    } 

    // draw heading rectangle, allways at the top.
    int16_t rectd = (outer_r-inner_r)-4;
    dial.drawRoundRect(cx-rectd,0,2*rectd,rectd,4,PLT_LIGHTGREY);
    dial.fillRoundRect(cx-rectd+1,1,(2*rectd)-2,rectd-2,4,PLT_BLACK);
    dial.setTextColor(PLT_WHITE);
    dial.setTextDatum(MC_DATUM);
    //itoa(hdg,deg,10);
    deg[0] = ((int8_t)((hdg%1000)/100))+'0';
    deg[1] = ((int8_t)((hdg%100)/10))+'0';
    deg[2] = ((int8_t)(hdg%10))+'0';
    deg[3] = '\0';
    dial.drawString(deg, cx,(rectd/2)+2, 4);

    // port and starboard track
    drawSector(&dial, hdg-portLL, cx, cy, PLT_RED); 
    drawSector(&dial, hdg-stbdLL, cx, cy, PLT_GREEN);

    // wind pointers
    drawWindpointer(&dial, twa, twah, ntwah, cx, cy, PLT_RED);
    drawWindpointer(&dial, awa, awah, nawah, cx, cy, PLT_GREEN);


    // orange pointer
    tx[0] = -7;
    tx[1] = 7;
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
    ty[0] = -inner_r+15;
    ty[1] = -inner_r;
    rotatePoints(tx,ty,2, yellowPointerAngle, cy, cy);
    dial.fillCircle(tx[0], ty[0], 7, PLT_YELLOW);
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
    dial.fillCircle(tx[3], ty[3], 2, PLT_PURPLE);

    drawBoat(&dial, cx, cy);

    // current arrow
    /*
     point numbering
            0
          13 42
           567

    */
    tx[0] = 0;
    tx[1] = -7;
    tx[2] = 7;

    tx[3] = -3;
    tx[4] = 3;

    tx[5] = -4;
    tx[6] = 0;
    tx[7] = 4;
    
    ty[0] = -20;
    ty[1] = -5;
    ty[2] = -5;

    ty[3] = -5;
    ty[4] = -5;
    ty[5] = 15;
    ty[6] = 15;
    ty[7] = 15;
    rotatePoints(tx,ty,8, currentAngle, cy, cy);
    dial.fillTriangle(
      tx[0], ty[0],
      tx[1], ty[1],
      tx[2], ty[2],
      PLT_BLUE);

    dial.fillTriangle(
      tx[5], ty[5],
      tx[3], ty[3],
      tx[6], ty[6],
      PLT_BLUE);
    dial.fillTriangle(
      tx[3], ty[3],
      tx[6], ty[6],
      tx[4], ty[4],
      PLT_BLUE);
    dial.fillTriangle(
      tx[6], ty[6],
      tx[4], ty[4],
      tx[7], ty[7],
      PLT_BLUE);
    
    
    dial.pushSprite(x+sx,y+sy, PLT_TRANSPARENT);    
}




void TFTSailing::display(TFT_eSPI *tft, bool firstPaint) {
  if ( firstPaint ) {
    tft->fillScreen(PLT_BLACK);    
  }
  drawDial(tft, 0,0,uw,uh);  // may need to iterate over the dial to reduce memory requirements.
}




void TFTInfoBlock::display(TFT_eSPI *tft, const char *line2, const char *line3, bool firstPaint) {
  TFT_eSprite block = TFT_eSprite(tft);
  block.setColorDepth(1);
  block.createSprite(uw, uh); // 320*320/2, might be a problem, so may have to call this multiple times
  block.fillSprite(TFT_BLACK);
  block.setTextColor(TFT_WHITE);
  int16_t lineHeight = 20;
  switch(alignment) {
    case TFTInfoBlock::topRight:
      block.setTextDatum(TR_DATUM);
      block.drawString(title, uw, 0,  4);
      block.drawString(line2, uw, lineHeight, 4);
      block.drawString(line3, uw, lineHeight*2, 4);
    break;
    case TFTInfoBlock::bottomLeft:
      block.setTextDatum(BL_DATUM);
      block.drawString(title, 0, uh, 4);
      block.drawString(line2, 0, uh-(lineHeight*2), 4);
      block.drawString(line3, 0, uh-(lineHeight), 4);
    break;
    case TFTInfoBlock::bottomRight:
      block.setTextDatum(BR_DATUM);
      block.drawString(title, uw, uh, 4);
      block.drawString(line2, uw, uh-(lineHeight*2), 4);
      block.drawString(line3,  uw, uh-lineHeight, 4);
    break;
    case TFTInfoBlock::topLeft:
    default:
      block.setTextDatum(TL_DATUM);
      block.drawString(title, 0,0, 4);
      block.drawString(line2, 0,lineHeight, 4);
      block.drawString(line3, 0,lineHeight*2, 4);
  }
  tft->setBitmapColor(colour, PLT_BLACK);
  block.pushSprite(x,y, PLT_BLACK);
}



