#pragma once

#include <TFT_eSPI.h> 

// Meter colour schemes
#define RED2RED 0
#define GREEN2GREEN 1
#define BLUE2BLUE 2
#define BLUE2RED 3
#define GREEN2RED 4
#define RED2GREEN 5

#define TFT_GREY 0x2104 // Dark grey 16 bit colour

typedef struct {
  TFT_eSPI *tft;
  TFT_eSprite *sprite;
  int x_offset, y_offset;
} TFTOutputConfig;

// callback and control for loading jpeg images.
extern TFTOutputConfig  tft_output_config;
extern bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);

class TFTWidgets {
public:
	static int ringMeter(TFT_eSPI *tft,int value, int vmin, int vmax, int x, int y, int r, const char *units, byte scheme);
	static void drawAlert(TFT_eSPI *tft, int x, int y , int side, bool draw);
	static unsigned int rainbow(byte value);
	static float sineWave(int phase);
	static void drawIcon(TFT_eSPI *tft, const unsigned short* icon, int16_t x, int16_t y, int8_t width, int8_t height);
	/*static void textBox(TFT_eSPI *tft, const char *value, const char *previousValue, 
			int x, int y, int height, int width, const char *bl, const char *br, bool firstPaint);*/

	/*
	static void textBot(TFT_eSPI *tft, float value, int x, int y, int w, const char *format, const char *title, const char * units);
*/
};


class TFTTextBox {
public:
	TFTTextBox(int x, int y, int width, int height, int gap, int precision, 
			float minValue, 
			float maxValue, const char *bl, const char *br, char positive=0, char negative='-') {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->precision = precision;
		this->bl = bl;
		this->br = br;
		this->maxValue = maxValue;
		this->minValue = minValue;
		this->positive = positive;
		this->negative = negative;

	};
	void update(TFT_eSPI *tft, float value,  bool firstPaint);
private:
	const char *br, *bl;
	int x, y, width, height, precision;
	float maxValue, minValue;
	char positive, negative;
	uint16_t checksum = 0;
	void formatValue(float value, char *buffer);

};


class TFTLatLonBox {
public:
	TFTLatLonBox(int x, int y, int width, int height, int gap) {
		this->x = x;
		this->y = y;
		this->width = width*2+gap;
		this->height = height;

	};
	void update(TFT_eSPI *tft, float lat, float lon,  bool firstPaint);
private:
	int x, y, width, height;
	uint16_t checksum = 0;
	bool updateString(TFT_eSPI *tft,  int x, int y, const char *prev, const char* current, bool force, bool updated);
	void formatMinDec( float v, char *buffer, bool isLatitude);
	void formatDeg( float v, char *buffer, bool isLatitude);
	void formatMin( float v, char *buffer, bool isLatitude);
};


class TFTDial {
public:
	TFTDial(int16_t x, int16_t y, int16_t width, int16_t height, 
			int8_t needleLength, int8_t needleWidth, int8_t needleRadius, 
			int8_t needleOffsetX = 0, int8_t needleOffsetY = 0 )  {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->needleLength = needleLength;
		this->needleWidth = needleWidth;
		this->needleRadius = needleRadius;
		this->needleOffsetX = needleOffsetX;
		this->needleOffsetY = needleOffsetY;
	};
	virtual void loadJpg();
	virtual int16_t getNeedleAngle(float value);

	void update(TFT_eSPI *tft, float value,  bool firstPaint);
	void dispose();
protected:
	int16_t x;
	int16_t y;

private:
	int16_t width;
	int16_t height;
	int8_t needleOffsetX;
	int8_t needleOffsetY;
	int8_t needleLength;
	int8_t needleWidth;
	int8_t needleRadius;


	float previousValue;

	int previousAngle = -120;

	TFT_eSprite * needle = NULL; // Sprite object for needle
	void drawBackground(TFT_eSPI *tft);

};


#define NEEDLE_LENGTH 62 // Visible length
#define NEEDLE_WIDTH   5  // Width of needle - make it an odd number
#define NEEDLE_RADIUS 84  // Radius at tip

typedef struct _TachometerValues {
  float rpm;
  float coolantTemperature;
  float fuel;
  float speed;
  float engineBatteryVoltage;
  float alternatorVoltage;
  float chargeCurrent;
  float serviceBatteryVoltage;
  uint16_t engineHours;
  uint16_t status1;
  uint16_t status2;
} TachometerValues;

class TFTTachometer : public TFTDial {
public:
	TFTTachometer(int16_t x, int16_t y): TFTDial{x,y, 310, 310, 
		/* needle length */ 50, 
		/* needle width */ 5, 
		/* needle radius */ 84, 
		-3, 
		-12} {

	};
	~TFTTachometer() {
		dispose();
	};
	void loadJpg() override;
	int16_t getNeedleAngle(float value) override;
	void updateLCD(TFT_eSPI *tft, TachometerValues *values,  bool firstPaint);
private:
	unsigned long lastLCDUpdate = 0;
	unsigned long lastLCDChange = 0;
	uint8_t lcdView = 0;


};

class TFTFuelGauge: public TFTDial {
public:
	TFTFuelGauge(int16_t x, int16_t y): TFTDial{x,y, 130, 130,		
	    /* needle length */    33, 
		/* needle width */      5, 
		/* needle tip radius */45, 
		/* center x offset */   3, 
		/* center y offset */  28} 
		 {

	};
	~TFTFuelGauge() {
		dispose();
	};
	void loadJpg() override;
	int16_t getNeedleAngle(float value) override;
};

class TFTCoolantGauge: public TFTDial {
public:
	TFTCoolantGauge(int16_t x, int16_t y): TFTDial{x,y, 130, 130, 
		/* needle length */    33, 
		/* needle width */      5, 
		/* needle tip radius */45, 
		/* center x offset */   0, 
		/* center y offset */  28} 
		{

	};
	~TFTCoolantGauge() {
		dispose();
	};
	void loadJpg() override;
	int16_t getNeedleAngle(float value) override;
};



class TFTSplash {
public:
	TFTSplash(const char *fileName) {
		this->fileName = fileName;
	};
	void display(TFT_eSPI *tft);
private:
	const char * fileName;
};



typedef struct _SailingScreenValues {
	int16_t hdg;
	int16_t twa;
	int16_t awa;
	int16_t portLL;
	int16_t stbdLL;
	int16_t orangePointerAngle;
	int16_t yellowPointerAngle;
	int16_t purplePointerAngle;
	int16_t currentDirection;
	float aws;
	float vmg;
	float polar;
	float tws;
	float currentSpeed;
} SailingScreenValues;


class TFTSailing {
public:

	TFTSailing(int16_t x, int16_t y, int16_t uw, int16_t uh) {
		this->x = x;
		this->y = y;
		this->uw = uw;
		this->uh = uh;
		outer_r = uw/2;
  		inner_r = (uw*40)/100;
  		txt_r = (uw*45)/100;
	};
 	void display(TFT_eSPI *tft, SailingScreenValues * values, bool firstPaint);

 	#define MAX_WIND_HISTORY 20
	// data



private:
	int16_t x; // top left
	int16_t y; // top left
	int16_t uw; // widget width
	int16_t uh; // widget height
	int16_t outer_r;
	int16_t inner_r;
	int16_t txt_r;
	int16_t twah[MAX_WIND_HISTORY];
	int16_t awah[MAX_WIND_HISTORY];
	uint8_t ntwah = 0;
	uint8_t nawah = 0;;
	unsigned long lastUpdate = 0;
	void rotatePoints(int16_t *x, int16_t *y, int8_t n, int16_t a, int16_t tx, int16_t ty);
	void drawWindpointer(TFT_eSprite *dial, int16_t a, int16_t *ah, int8_t nah, int16_t cx, int16_t cy, int8_t pallet_idx);
	void drawSector(TFT_eSprite *dial, int16_t a, int16_t cx, int16_t cy, int8_t pallet_idx);
	void drawBoat(TFT_eSprite *dial, int16_t cx, int16_t cy);
	void drawDial(TFT_eSPI *tft, SailingScreenValues * values, int sx, int sy, int sw, int sh);
};


class TFTTrueWindBlock {
public:
	TFTTrueWindBlock() {};
	void display(TFT_eSPI *tft, SailingScreenValues *values, bool firstPaint);
private:
	uint16_t checksum = 0;
};

class TFTVMGBlock {
public:
	TFTVMGBlock() {};
	void display(TFT_eSPI *tft, SailingScreenValues *values, bool firstPaint);
private:
	uint16_t checksum = 0;
};

class TFTApparentWindBlock {
public:
	TFTApparentWindBlock() {};
	void display(TFT_eSPI *tft, SailingScreenValues *values, bool firstPaint);
private:
	uint16_t checksum = 0;
};

class TFTCurrentBlock {
public:
	TFTCurrentBlock() {};
	void display(TFT_eSPI *tft, SailingScreenValues *values, bool firstPaint);
private:
	uint16_t checksum = 0;
};



