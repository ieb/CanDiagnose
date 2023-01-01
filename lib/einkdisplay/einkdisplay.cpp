

#include "einkdisplay.h"

// Public methods.

void EinkDisplay::begin(int32_t baud ) {
//    void init(uint32_t serial_diag_bitrate, bool initial, uint16_t reset_duration = 10, bool pulldown_rst_mode = false)
//    void init(uint32_t serial_diag_bitrate, bool initial, uint16_t reset_duration, bool pulldown_rst_mode, SPIClass& spi, SPISettings spi_settings)

	display.init(/* debug baud rate*/ baud, 
				 /* initial init */ true, 
				 /* reset duration */ 10, 
				 /* reset pulldown mode */ false, 
				 /* spi class */ hspi,
				 /* spi settings */ spi_settings); 
    Serial.println("Eink init done");
    update();
    Serial.println("Eink update done");
};
/**
 * Performs an update of the display 
 */
void EinkDisplay::update(void) {
	updateValues(false);
	unsigned long now = millis();
	if ( (now-pageChangeAt) > 30000 ) {
		pageNo = 0;
	}
	if ( pageVisible != pageNo ) {
      Serial.print("calls=");
      Serial.print(pageNoCalls);
      Serial.print(" changes=");
      Serial.print(pageNoChanges);
      Serial.print(" pageNo=");
      Serial.println(pageNo);
        switch(pageNo) {
            case 0:
                showLogo();
                break;
            case 1:
                page1Display();
                break;
            case 2:
                page2Display();
                break;
        } 
        pageVisible = pageNo;
    }
};

// called by ISR, must be fast.
void EinkDisplay::nextPage(void) {
	pageNoCalls++;
	if ( pageNo == pageVisible ) {
		pageNoChanges++;
		pageNo++;
		pageChangeAt = millis();
		if ( pageNo > 2) {
			pageNo = 1;
		}
	}
};


// Private methods



void EinkDisplay::showLogo(void) {
    display.clearScreen(); // use default for white
    display.drawImage(lunaLogo_Large, 0, 0, lunaLogo_Large_w, lunaLogo_Large_h);
};



void EinkDisplay::page1Display(void) {
    char latValue[11] = "--:--.---";
    char lonValue[11] = "---:--.---";
    char latSign[2] = "N";
    char lonSign[2] = "E";
    char systemTimeValue[20] = "--:--:-- --/--/----";

    updateValues();

    formatLatLon(&latValue[0],&lonValue[0],&latSign[0],&lonSign[0]);
    formatSystemTime(&systemTimeValue[0]);


    display.setRotation(1);
    display.setFont(&FreeSans12pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.firstPage();


    do
    {
        display.clearScreen();
        display.setTextColor(GxEPD_BLACK);
        printCell(0,0,"Lat:",&latValue[0],&latSign[0]);
        printCell(1,0,"Lon:",&lonValue[0],&lonSign[0]);
        printCell(0,1,"Log:",logReading,"Nm","%7.1f");
        printCell(1,1,"Pressure:",pressureReading,"","%6.1f");
        printCell(2,0,"Sog:",sogReading,"Kn","%5.1f");
        printCell(2,1,"Cog:",cogReading,"M","%3.0f");
        printCell(3,0,"Stw:",stwReading,"Kn","%5.1f");
        printCell(3,1,"Hdg:",hdmReading,"M","%3.0f");
        printCell(4,0,"Aws:",awsReading,"Kn","%5.1f");
        printCell(4,1,"Awa:",awaReading,"S","P","%3.0f");
        printCell(5,0,"Tws:",twsReading,"Kn","%5.1f");
        printCell(5,1,"Twa:",twaReading,"S","P","%3.0f");
        display.setTextColor(GxEPD_WHITE);
        display.fillRect(0, 205, 480, 280, GxEPD_BLACK);
        printCell(7,0,"Navigation");
        printCell(8,0, &systemTimeValue[0],"");
        display.drawBitmap(480-lunaLogo_Small_w,280-lunaLogo_Small_h, lunaLogo_Small, lunaLogo_Small_w, lunaLogo_Small_h,GxEPD_WHITE);

    }
    while (display.nextPage());
};

void EinkDisplay::page2Display(void) {
    char systemTimeValue[20] = "--:--:-- --/--/----";
    updateValues();
    formatSystemTime(&systemTimeValue[0]);
    display.setRotation(1);
    display.setFont(&FreeSans12pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.firstPage();
    do
    {
        display.clearScreen();
        display.setTextColor(GxEPD_BLACK);
        printCell(0,0,"Service:",serviceVoltReading,"V","%7.2f");
        printCell(0,1,"Alt:",alternatorVoltReading,"V","%7.2f");
        printCell(1,0,"Engine:",engineVoltReading,"V","%7.2f");
        printCell(1,1,"Fuel:",fuelLitersReading,"l","%7.1f");
        printCell(2,0,"Hours:",engineHoursReading,"h","%7.1f");
        printCell(2,1,"Coolant:",coolantTemperatureReading,"C","%7.0f");
        //printCell(4,0,"RPM:",18.213,"Kn","%7.1f");
        printCell(3,1,"RPM:",rpmReading,"","%7.0f");
        display.setTextColor(GxEPD_WHITE);
        display.fillRect(0, 205, 480, 280, GxEPD_BLACK);
        printCell(7,0,"Engine");

        printCell(8,0,&systemTimeValue[0]);
        display.drawBitmap(480-lunaLogo_Small_w,280-lunaLogo_Small_h, lunaLogo_Small, lunaLogo_Small_w, lunaLogo_Small_h,GxEPD_WHITE);
    }
    while (display.nextPage());
};


void EinkDisplay::formatLatLon(char * latValue, 
								char * lonValue, 
								char * latSign, 
								char * lonSign) {
    double latitude,longitude;
    int16_t age;
#ifdef DUMMYDATA
    if (true) {
        latitude = -54.231;
        longitude = -1.2321;
        age = 10;
#else
    if (dataCollector.getLatLong(latitude, longitude, age)) {
#endif
        int deg = (int) latitude;
        double min = (double) 60.0*(latitude-deg);
        if ( deg >= 0 && min >= 0) {
            sprintf(latValue," %02d:%05.2f", deg, min); 
            latSign[0] = 'N';
        } else {
            sprintf(latValue, " %02d:%05.2f", -deg,  -min); 
            latSign[0] = 'S';
        }
        deg = (int) longitude;
        min = (double) 60.0*(longitude-deg);
        if ( deg >= 0 && min >= 0) {
            sprintf(lonValue, "%03d:%05.2f", deg, min); 
            lonSign[0] = 'E';
        } else {
            sprintf(lonValue, "%03d:%05.2f", -deg, -min); 
            lonSign[0] = 'W';
        }
    }
}



void EinkDisplay::formatSystemTime(char *systemTimeValue) {
#ifdef DUMMYDATA
    if (true) {
        uint16_t daySerial = 19313;  // 17/11/2022 16:53:02.321
        double seconds = 60782.321;
#else
    GnssData * gnssData = dataCollector.getGnss();
    if ( gnssData != NULL ) {
        uint16_t daySerial = gnssData->daysSince1970; 
        double seconds = gnssData->secondsSinceMidnight;
#endif
        tmElements_t tm;
        double tofLog = seconds+daySerial*SECS_PER_DAY; 
        breakTime((time_t)tofLog, tm);
        sprintf(systemTimeValue, "%02d:%02d:%02d %02d/%02d/%04d", tm.Hour,tm.Minute,tm.Second,  tm.Day, tm.Month, tm.Year+1970 );
    } else {
    }
}




void EinkDisplay::printCell(int row, int col, 
						const char * label, 
						const char *v, 
						const char * units ) {
    uint16_t x = col?middleX:marginX;
    uint16_t y = marginY+lineHeight*row;
    printString(x,y,label);
    printString(x+cellWidth,y,v,true);
    printString(x+cellWidth+10,y,units);
};

void EinkDisplay::printCell(int row, int col,  
						const char * label, 
						float v,  
						const char * units, 
						const char * format ) {
    uint16_t x = col?middleX:marginX;
    uint16_t y = marginY+lineHeight*row;
    printString(x,y,label);
    printFloat(x+cellWidth,y,v,format);
    printString(x+cellWidth+10,y,units);
};
void EinkDisplay::printCell(int row, int col,  
						const char * label, 
						float v,  
						const char * posUnits,  
						const char * negUnits, 
						const char * format ) {
    uint16_t x = col?middleX:marginX;
    uint16_t y = marginY+lineHeight*row;
    printString(x,y,label);
    if ( v < 0 ) {
        printFloat(x+cellWidth,y,-v,format);
        printString(x+cellWidth+10,y,negUnits);
    } else {
        printFloat(x+cellWidth,y,v,format);
        printString(x+cellWidth+10,y,posUnits);
    }
};
void EinkDisplay::printFloat(int x, int y, 
							float f, 
							const char * format, 
							bool rightAlign) {
    if ( f == 1E9 ) {
        printString(x, y, "--", rightAlign);
    } else {
        char buffer[20];
        sprintf(&buffer[0],format,f);
        printString(x, y, &buffer[0], rightAlign);
    }
};


void EinkDisplay::printString(int x, int y, 
								const char * text, 
								bool rightAlign) {
    int16_t tbx, tby; uint16_t tbw, tbh;
    if (rightAlign) {
        display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
        display.setCursor(x-tbw,y);
    } else {
        display.setCursor(x,y);
    }
    display.print(text);
};

double EinkDisplay::average(double v, double r) {
	if (v == 1E9 ) {
		return r;
	} else {
		return (0.9*v)+(0.1*r);
	}
};
double EinkDisplay::angularAverage(double v, double r) {
	if ( v == 1E9 ) {
		return r;
	} else {
		double vrad = DegToRad(v);
		double rrad = DegToRad(r);
		double s = (0.9*sin(vrad))+(0.1*sin(rrad));
		double c = (0.9*cos(vrad))+(0.1*cos(rrad));
		return RadToDeg(atan2(s,c));
	}
};

void EinkDisplay::updateValues(bool force) {
	unsigned long now = millis();
	if ( force || (now-lastUpdate) > 1000 ) {
		lastUpdate = now;


	    LogData * logData = dataCollector.getLog();
	    if ( logData != NULL) {
	        logReading = logData->log/1852.0;
	    } else {
#ifdef DUMMYDATA
	        logReading = 1234.21; 
#else
	        logReading = 1E9;
#endif
	    }
	    PressureData *pressure = dataCollector.getAtmosphericPressure();
	    if ( pressure != NULL ) {
	        pressureReading = average(pressureReading,PascalTomBar(pressure->actual));
	    } else {
#ifdef DUMMYDATA
	        pressureReading = average(pressureReading,1024.56); //1E9;
#else
	        pressureReading = 1E9;
#endif
	    }
	    CogSogData * cogSogData = dataCollector.getCogSog();
	    if ( cogSogData != NULL) {
	        cogReading = angularAverage(cogReading,RadToDeg(cogSogData->cog));
	        sogReading = average(sogReading,msToKnots(cogSogData->sog)); 
	    } else {
#ifdef DUMMYDATA
	        cogReading = angularAverage(cogReading,213.42); //1E9;
	        sogReading = average(sogReading,12.87); //1E9;
#else
	        cogReading = 1E9;
	        sogReading = 1E9;
#endif
	    }
	    SpeedData * speedData = dataCollector.getSpeed();
	    if ( speedData != NULL ) {
	        stwReading = average(stwReading,msToKnots(speedData->stw));
	    } else {
#ifdef DUMMYDATA
	        stwReading = average(stwReading,11.23); //1E9;
#else
	        stwReading = 1E9;
#endif
	    }
	    HeadingData * headingData = dataCollector.getHeading();
	    if ( headingData != NULL ) {
	        hdmReading = angularAverage(hdmReading,RadToDeg(headingData->heading)); 
	    } else {
#ifdef DUMMYDATA
	        hdmReading = angularAverage(hdmReading,211.67); //1E9;
#else
	        hdmReading = 1E9;
#endif
	    }
	    WindData * windData = dataCollector.getAparentWind();
	    if ( windData != NULL ) {
	        awsReading = average(awsReading,msToKnots(windData->windSpeed));
	        awaReading = angularAverage(awaReading,RadToDeg(windData->windAngle));
	        if (stwReading != 1E9) {
	            double apparentX = cos(windData->windAngle) * windData->windSpeed;
	            double apparentY = sin(windData->windAngle) * windData->windSpeed;
	            double twa = atan2(apparentY, - stwReading + apparentX);
	            double tws = sqrt(pow(apparentY, 2) + pow(-stwReading + apparentX, 2));
	            twsReading = msToKnots(tws);
	            twaReading = RadToDeg(twa);
	        } else {
#ifdef DUMMYDATA
	            twsReading = average(twsReading,22.543); //1E9;
	            twaReading = angularAverage(twaReading,-45.23); //1E9;
#else
	            twsReading = 1E9;
	            twaReading = 1E9;
#endif
	        }
	    } else {
#ifdef DUMMYDATA
	        awsReading = average(awsReading, 21.23); //1E9;
	        awaReading = angularAverage(awaReading, 32.214); //1E9;
	        twsReading = average(twsReading, 18.92); //1E9;
	        twaReading = angularAverage(twaReading, -45.23); //1E9;
#else
	        awsReading = 1E9;
	        awaReading = 1E9;
	        twsReading = 1E9;
	        twaReading = 1E9;
#endif
	    }

	    ModbusShunt * serviceBatteryShunt = modbus.getServiceBattery();
	    if (serviceBatteryShunt != NULL && serviceBatteryShunt->isEnabled() ) {
		        serviceVoltReading = average(serviceVoltReading, serviceBatteryShunt->voltage);
	    } else {
		    DcBatteryData * serviceBattery = dataCollector.getBatteryInstance(0);
		    if (serviceBattery != NULL ) {
		        serviceVoltReading = average(serviceVoltReading, serviceBattery->voltage);
		    } else {
#ifdef DUMMYDATA
		        serviceVoltReading = average(serviceVoltReading, 12.32); //1E9;
#else
		        serviceVoltReading = 1E9;
#endif
		    }

	    }

	    ModbusShunt * engineBatteryShunt = modbus.getEngineBattery();
	    if (engineBatteryShunt != NULL && engineBatteryShunt->isEnabled() ) {
		        engineVoltReading = average(engineVoltReading, engineBatteryShunt->voltage);
	   	} else {

		    DcBatteryData * engineBattery = dataCollector.getBatteryInstance(1);
		    if (engineBattery != NULL ) {
		        engineVoltReading = average(engineVoltReading, engineBattery->voltage);
		    } else {
#ifdef DUMMYDATA
		        engineVoltReading = average(engineVoltReading, 12.65); //1E9;
#else
		        engineVoltReading = 1E9;
#endif
		    }
		}
	    EngineData * engineData = dataCollector.getEngineInstance();
	    if (engineData != NULL ) {
	        engineHoursReading = engineData->Hours;
	        rpmReading = average(rpmReading, engineData->speed);
	        coolantTemperatureReading = average(coolantTemperatureReading, KelvinToC(engineData->CoolantTemp));
	        alternatorVoltReading = average(alternatorVoltReading, engineData->AltenatorVoltage);
	    } else {
#ifdef DUMMYDATA
	        engineHoursReading = 123.2; //1E9;
	        rpmReading = 2543.7; //1E9;
	        coolantTemperatureReading = 56.43; //1E9;
	        alternatorVoltReading = 13.82; //1E9;
#else
	        engineHoursReading = 1E9;
	        rpmReading = 1E9;
	        coolantTemperatureReading = 1E9;
	        alternatorVoltReading = 1E9;
#endif
	    }
	    FluidLevelData * fuelLevel = dataCollector.getFuelLevel();
	    if (fuelLevel != NULL ) {
	        fuelLitersReading = average(fuelLitersReading, fuelLevel->level * fuelLevel->capacity);
	    } else {
#ifdef DUMMYDATA
	        fuelLitersReading = 42.32; //1E9;
#else
	        fuelLitersReading = 1E9;
#endif
	    }
	}
}
