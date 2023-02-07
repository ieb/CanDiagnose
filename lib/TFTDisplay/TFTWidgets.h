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
	TFTTextBox(int _x, int _y, int _width, int _height, int _precision, 
			float _minValue, 
			float _maxValue, const char *_bl, const char *_br, char _positive=0, char _negative='-') {
		x = _x;
		y = _y;
		width = _width;
		height = _height;
		precision = _precision;
		bl = _bl;
		br = _br;
		maxValue = _maxValue;
		minValue = _minValue;
		positive = _positive;
		negative = _negative;

	};
	void update(TFT_eSPI *tft, float value,  bool firstPaint);
private:
	void formatValue(float value, char *buffer);
	const char *br, *bl;
	int x, y, width, height, precision;
	float previousValue, maxValue, minValue;
	char positive, negative;
};


class TFTLatLonBox {
public:
	TFTLatLonBox(int _x, int _y, int _width, int _height) {
		x = _x;
		y = _y;
		width = _width;
		height = _height;

	};
	void update(TFT_eSPI *tft, float lat, float lon,  bool firstPaint);
private:
	const char *br, *bl;
	int x, y, width, height, precision;
	float previousLay, previousLon;
};
