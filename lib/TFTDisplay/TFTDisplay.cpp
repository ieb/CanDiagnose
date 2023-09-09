

#include "TFTDisplay.h"
#include "TFTWidgets.h"
#include "N2kMessages.h"


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
#define MAX_PAGES 5
#define LOGO_PAGE 0
#define SAILING_PAGE 1
#define ENGINE_PAGE 2
#define GRIDBOXES_PAGE 3
#define MAXI_PAGE 4

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
      currentPage = new TFTEngineDisplayPage(tft, n2kCollector, modbus);
      break;
    case MAXI_PAGE:
      currentPage = new TFTMaxiDisplayPage(tft, n2kCollector, modbus, webServer);
      break;
    case GRIDBOXES_PAGE:
      currentPage = new TFTGridBoxesDisplayPage(tft, n2kCollector, modbus, webServer);
      break;
    case SAILING_PAGE:
      currentPage = new TFTSailingDisplayPage(tft, n2kCollector);
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
    tft.fillScreen(TFT_BLACK);
  } 
  // update data elements.

  unsigned long now = millis();
#ifdef DEMOMODE

  if ( now > lastUpdate + 2000) {
    lastUpdate = now;

    rpm+=rpmadd;
    if ( rpm > 4000 ) {
      rpmadd = -52;
    } else if ( rpm < 750 ) {
      rpm = 0;
      rpmadd = 179;
    } 
    /*
    rpm+=500;
    if (rpm > 4000 ) {
      rpm = 0;
    } 
    */

    /*
    fuelLevel+=25;
    if  (fuelLevel > 100) {
      fuelLevel = 0;
    }
    */

    fuelLevel+=fuelLevelAdd;
    if ( fuelLevel > 100 ) {
      fuelLevelAdd = -12;
    } else if ( fuelLevel < 0 ) {
      fuelLevel = 0;
      fuelLevelAdd = 31;
    } 
    /*
    coolantTemperature+=20;
    if ( coolantTemperature > 120 ) {
      coolantTemperature = 40;
    }
    */
    coolantTemperature+=coolantTemperatureAdd;
    if ( coolantTemperature > 120 ) {
      coolantTemperatureAdd = -12;
    } else if ( coolantTemperature < 20 ) {
      coolantTemperature = 20;
      coolantTemperatureAdd = 31;
    } 
    d += 4; if (d >= 360) d = 0;

//    Serial.print(" rpm: ");Serial.print(rpm);
//    Serial.print(" fuel: ");Serial.print(fuelLevel);
//    Serial.print(" coolant: ");Serial.println(coolantTemperature);

  }
  TachometerValues values; 
  values.rpm = rpm;
  values.coolantTemperature = coolantTemperature;
  values.fuelLevel = fuelLevel;
  values.speed = (7.0 + 2.0 * TFTWidgets::sineWave(d + 90));
  values.engineBatteryVoltage = (11.5 + 3 * TFTWidgets::sineWave(d + 10));
  values.alternatorVoltage = (11.5 + 3 * TFTWidgets::sineWave(d + 60));
  values.chargeCurrent = (20 + 20 * TFTWidgets::sineWave(d + 110));
  values.serviceBatteryVoltage = (11.5 + 3 * TFTWidgets::sineWave(d + 90));

#else

  TachometerValues values = {
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0,
    0,
    0
  }; 

  {
    EngineData *engine = n2kCollector.getEngineInstance(1);
    if ( engine != NULL ) {
        values.rpm = engine->speed;
        values.alternatorVoltage = engine->AltenatorVoltage;
        values.coolantTemperature = KelvinToC(engine->CoolantTemp);
        values.status1 = engine->Status1;
        values.status2 = engine->Status2;
        values.engineHours = SecondsToh(engine->Hours);
    }

    DcBatteryData *battery = n2kCollector.getBatteryInstance(1);
    if ( battery != NULL ) {
        values.engineBatteryVoltage = battery->voltage;   
    }
    battery = n2kCollector.getBatteryInstance(2);
    if ( battery != NULL ) {
      values.serviceBatteryVoltage = battery->voltage;
    }

    FluidLevelData * fuel =  n2kCollector.getFuelLevel();
    if ( fuel != NULL ) {
      values.fuel = fuel->level;
    }

    // fetch modbus data if available.
    ModbusShunt * serviceBattery = modbus.getServiceBattery();
    if ( now < serviceBattery->lastModified+60000 ) {
      values.serviceBatteryVoltage = serviceBattery->voltage;
    } 
    ModbusShunt * engineBattery = modbus.getServiceBattery();
    if ( now < engineBattery->lastModified+60000 ) {
      values.engineBatteryVoltage = engineBattery->voltage;
    } 
  }

#endif

  tachometer.update(&tft, values.rpm, !displaying);
  tachometer.updateLCD(&tft, &values, !displaying);
  fuel.update(&tft, values.fuel, !displaying);
  coolant.update(&tft, values.coolantTemperature, !displaying);

  displaying = true;

}






void TFTLogoDisplayPage::update(bool paintScreen) {
  if ( !paintScreen ) {
    return;
  }
  if ( !displaying ) {
    splash.display(&tft);
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

 #if DEMOMODE
    if ( now-lastUpdate >  1000) {
      lastUpdate = now;
      d += 4; if (d >= 360) d = 0;
    }

  //  static void textBox(TFT_eSPI *tft,char *value, int x, int y, int height, int width, char *bl, char *br);


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

#else

    {
      WindData *wind = n2kCollector.getAparentWind();
      if (wind != NULL) {
        awa.update(&tft, RadToDeg(wind->windAngle), !displaying );
        aws.update(&tft, msToKnots(wind->windSpeed), !displaying );
      } else {
        awa.update(&tft, N2kDoubleNA, !displaying );
        aws.update(&tft, N2kDoubleNA, !displaying );      
      }
    }
    {
      WindData *wind = n2kCollector.getTrueWind();
      if (wind != NULL) {
        twa.update(&tft, RadToDeg(wind->windAngle), !displaying );
        tws.update(&tft, msToKnots(wind->windSpeed), !displaying );
      } else {
        twa.update(&tft, N2kDoubleNA, !displaying );
        tws.update(&tft, N2kDoubleNA, !displaying );      
      }

    }
    {
      CogSogData * cogSogData = n2kCollector.getCogSog();
      if ( cogSogData != NULL) {
        cog.update(&tft, RadToDeg(cogSogData->cog), !displaying );
        sog.update(&tft, msToKnots(cogSogData->sog), !displaying );
      } else {
        cog.update(&tft, N2kDoubleNA, !displaying );
        sog.update(&tft, N2kDoubleNA, !displaying );
      }
    }
    {
      HeadingData * headingData = n2kCollector.getHeading();
      if (headingData != NULL) {
        hdg.update(&tft, RadToDeg(headingData->heading), !displaying );
      } else {
        hdg.update(&tft, N2kDoubleNA, !displaying );
      }
    }
    {
      SpeedData * speedData = n2kCollector.getSpeed();
      if (speedData != NULL) {
        stw.update(&tft, msToKnots(speedData->stw), !displaying );
      } else {
        stw.update(&tft, N2kDoubleNA, !displaying );
      }
    }
    {
      WaterDepthData * waterDepthData = n2kCollector.getWaterDepth();
      if (waterDepthData != NULL) {
        dbt.update(&tft, waterDepthData->depthBelowTransducer, !displaying );
      } else {
        dbt.update(&tft, N2kDoubleNA, !displaying );
      }
    }
    {
      LogData * logData = n2kCollector.getLog();
      if (logData != NULL) {
        log.update(&tft, logData->log/1852.0, !displaying );
      } else {
        log.update(&tft, N2kDoubleNA, !displaying );
      }
    }
    {
      double lat, lon;
      int16_t age;
      n2kCollector.getLatLong(lat, lon, age);
      pos.update(&tft, RadToDeg(lat), RadToDeg(lon), !displaying );
    }
    {
      PressureData * pressure = n2kCollector.getAtmosphericPressure();
      if (pressure != NULL) {
        mbar.update(&tft, PascalTomBar(pressure->actual), !displaying );
      } else {
        mbar.update(&tft, N2kDoubleNA, !displaying );
      }
    }
    {
      ModbusShunt * engineBattery = modbus.getEngineBattery();
      if ( now < engineBattery->lastModified+60000) {
        ebv.update(&tft, engineBattery->voltage, !displaying );
      } else {
        ebv.update(&tft, N2kDoubleNA, !displaying );
      }
    }
    {
      ModbusShunt * serviceBattery = modbus.getServiceBattery();
      if ( now < serviceBattery->lastModified+60000) {
        sbv.update(&tft, serviceBattery->voltage, !displaying );
        sbc.update(&tft, serviceBattery->current, !displaying );
      } else {
        sbv.update(&tft, N2kDoubleNA, !displaying );
        sbc.update(&tft, N2kDoubleNA, !displaying );
      }
    }
#endif

    displaying = true;





}

void TFTSailingDisplayPage::update(bool paintScreen) {
  if ( !paintScreen ) {
    return;
  }
  SailingScreenValues values = {
    0,
    0,
    0,
    -45,
    45,
    120,
    210,
    270,
    225,
    N2kFloatNA,
    N2kFloatNA,
    N2kFloatNA,
    N2kFloatNA,
    N2kFloatNA
  };

#if DEVMODE

  SailingScreenValues values;

  d = (d+0.2);
  if ( d >= 360 ) d = d - 360;
  values.hdg = d;
  values.awa = -180 + 360 * TFTWidgets::sineWave(d + 90);
  values.tws = 20 + 5 * TFTWidgets::sineWave(d + 90);
  values.aws = 22 + 5 * TFTWidgets::sineWave(d + 70);
  values.vmg = 5 + 3 * TFTWidgets::sineWave(d + 45);
  values.polar = .7 + .3 * TFTWidgets::sineWave(d + 30);
  values.currentSpeed = 1.5 + .3 * TFTWidgets::sineWave(d + 15);
  values.currentDirection = d;

  values.portLL = -180 + 360 * TFTWidgets::sineWave(d);
  values.stbdLL = -180 + 360 * TFTWidgets::sineWave(d+90);
  values.orangePointerAngle = -180 + 360 * TFTWidgets::sineWave(d+45);
  values.yellowPointerAngle = -180 + 360 * TFTWidgets::sineWave(d+135);
  values.purplePointerAngle = -180 + 360 * TFTWidgets::sineWave(d+225);

#else

  {
    WindData *wind = n2kCollector.getAparentWind();
    if (wind != NULL) {
      values.awa = round(RadToDeg(wind->windAngle));
      values.aws = msToKnots(wind->windSpeed);
    }
  }
  {
    WindData *wind = n2kCollector.getTrueWind();
    if (wind != NULL) {
      values.twa = round(RadToDeg(wind->windAngle));
      values.tws = msToKnots(wind->windSpeed);
    }
  }
  {
    HeadingData * headingData = n2kCollector.getHeading();
    if (headingData != NULL) {
      values.hdg = round(RadToDeg(headingData->heading));
    }
  }

// Need to support polar calculations for the remainder of information
// The javascript version of polar calculatons was not fast enough without
// an already interpolated polar map. We may need to build a lookup table, 
// but lookup tables take up space.
// ideally the polar calcs need to be done in the N2KCollector so the values are available to all
// and dont trigger high levels of compute here, as this is called on every loop.

#endif

  sailing.display(&tft, &values, !displaying);
  trueWind.display(&tft, &values, !displaying);
  apparentWind.display(&tft, &values, !displaying);
  polar.display(&tft, &values, !displaying);
  current.display(&tft, &values, !displaying);

}

