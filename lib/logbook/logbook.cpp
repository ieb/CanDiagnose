#include "logbook.h"
#include <TimeLib.h>
#define M_TO_NM(x) ((x)/1852.0)
#define SPEED_MS_TO_KN(x) ((x)*1.94384)
#define K_TO_C(x) (((x)-273.15))
#define PASCAL_TO_MBAR(x) (0.01*x)




LogBook::LogBook(N2KCollector &n2kCollector): n2kCollector{n2kCollector} {
};


double LogBook::relativeAngleToDeg(double a) {
    a = a*57.2958;
    if(a < -180.0) a += 360.0;
    if(a > 180.0) a -= 360.0;
    return a;
}
double LogBook::headingAngleToDeg(double a) {
    a = a*57.2958;
    if(a < -360.0) a += 360.0;
    if(a > 360.0) a -= 360.0;
    return a;
}

void LogBook::demoMode() {
    unsigned long now = millis();
    if ( (now-lastDemoUpdate) >  5000) {
        lastDemoUpdate = now;
         n2kCollector.log[0].lastModified = now;
         n2kCollector.log[0].source = 1;
         unsigned long atime = now;
         n2kCollector.log[0].daysSince1970 = 18973+(atime/(24*3600000));
         n2kCollector.log[0].secondsSinceMidnight = (atime%(24*3600000))/1000;
         n2kCollector.possition[0].source = 2;
         n2kCollector.possition[0].lastModified = now;
         n2kCollector.possition[0].longitude = 0.11987607061305089-((1.0*now)/(3600000));
         n2kCollector.possition[0].latitude = 52.18623058185942-((1.0*now)/(3600000));
         n2kCollector.log[0].lastModified = now;
         n2kCollector.log[0].log = 1852  + (1852.0*now)/(6000);
         n2kCollector.log[0].tripLog = (1852.0*now)/(6000);
    }

}


void LogBook::log() {
    unsigned long now = millis();
    if ( (now-lastLogUpdate) >  logPeriod) {
        LogData * log = n2kCollector.getLog();
        GnssData * gnss = n2kCollector.getGnss();

    
        if ( gnss != NULL && gnss->lastModified > lastLogUpdate ) {
            // new gnss data is avaiable, this indicates that the NMEA2000 instruments are on 
            
            uint16_t daySerial = gnss->daysSince1970; 
            double seconds = gnss->secondsSinceMidnight;
            tmElements_t tm;
            // docu
            double tofLog = seconds+daySerial*SECS_PER_DAY; 
            breakTime((time_t)tofLog, tm);
            char filename[30]; 
            sprintf(&filename[0],"/logbook/log%04d%02d%02d.txt", tm.Year+1970, tm.Month, tm.Day);
            File f;
            if ( !SPIFFS.exists(filename) ) {
                if ( !SPIFFS.exists("/logbook")) {
                    Serial.println("mkdir /logbook");
                    SPIFFS.mkdir("/logbook");
                }
                Serial.print("Creating ");Serial.println(filename);
                f = SPIFFS.open(filename,"a");
                f.println("time,logTime,lat,long,log,fixage,trip,cog,sog,stw,hdg,awa,aws,mbar,mbarage,rpm,coolant,serviceVolts,engineVolts,serviceCurrent");
            } else {
                //Serial.print("Opening ");Serial.println(filename);
                f = SPIFFS.open(filename,"a");
            }

            f.printf("%04d-%02d-%02dT%02d:%02d:%02dZ",tm.Year+1970, tm.Month, tm.Day,tm.Hour,tm.Minute,tm.Second);
            f.printf(",%lu",(unsigned long)tofLog);
            // lat, lon


            double latitude, longitude;
            int16_t age;
            if ( n2kCollector.getLatLong(latitude, longitude, age)) {
                f.printf(",%10.6f,%10.6f,%d",latitude, longitude,age);
            } else {
                f.printf(",,,");
            }


            //log,trip
            if ( log != NULL && log->lastModified+30000 > now) {
                f.printf(",%6.1f,%6.1f",M_TO_NM(log->log),M_TO_NM(log->tripLog));
            } else {
                f.print(",,");
            }
            // cog,sog  radians, m/s
            int i;
            for ( i = 0; i < MAX_COGSOG_SOURCES; i++) {
                if (n2kCollector.cogSog[i].source != 255 && n2kCollector.cogSog[i].lastModified+30000 > now ) {
                    if ( n2kCollector.cogSog[i].cog > -1000000) {
                        f.printf(",%d,%4.1f",(int)headingAngleToDeg(n2kCollector.cogSog[i].cog),
                             SPEED_MS_TO_KN(n2kCollector.cogSog[i].sog));
                        break;
                    }
                } 
            }
            if ( i == MAX_COGSOG_SOURCES ) {
                f.print(",,");
            }
            // stw m/s
            for ( i=0; i < MAX_SPEED_SOURCES; i++) {
                if (n2kCollector.speed[i].source != 255 && n2kCollector.speed[i].lastModified+30000 > now ) {
                    f.printf(",%4.1f",SPEED_MS_TO_KN(n2kCollector.speed[i].stw));
                    break;
                } 
            }
            if ( i == MAX_SPEED_SOURCES ) {
                f.print(",");
            }
            //hdg radians
            for ( i=0; i < MAX_HEADDING_SOURCES; i++) {
                if (n2kCollector.heading[i].source != 255 && n2kCollector.heading[i].lastModified+30000 > now ) {
                    f.printf(",%d",(int)headingAngleToDeg(n2kCollector.heading[i].heading));
                    break;
                } 
            }
            if ( i == MAX_HEADDING_SOURCES ) {
                f.print(",");
            }


            //aws, awa m/s, radians
            for ( i=0; i < MAX_WIND_SOURCES; i++) {
                if (n2kCollector.wind[i].source != 255 &&
                         n2kCollector.wind[i].windReference == N2kWind_Apparent && 
                         n2kCollector.wind[i].lastModified+30000 > now ) {
                    f.printf(",%4.1f,%d",SPEED_MS_TO_KN(n2kCollector.wind[i].windSpeed),(int)relativeAngleToDeg(n2kCollector.wind[i].windAngle));
                    break;
                } 
            }
            if ( i == MAX_WIND_SOURCES ) {
                f.print(",");
            }
            double pressure;

            if ( n2kCollector.getPressure(pressure, age) ) {
                f.printf(",%6.1f,%d",PASCAL_TO_MBAR(pressure),age);
            } else {
                f.print(",,");                
            }
            // rpm, ct, rpm, K
            for ( i = 0; i < MAX_ENGINE_SOURCES; i++) {
                if (n2kCollector.engine[i].source != 255 &&  n2kCollector.engine[i].lastModified+30000 > now ) { 
                    f.printf(",%d,%d",(int)n2kCollector.engine[i].speed,(int)K_TO_C(n2kCollector.engine[i].CoolantTemp));
                    break;
                } 
            }
            if ( i == MAX_ENGINE_SOURCES ) {
                f.print(",,");
            }
            f.print("\n");
            f.close();
        } else {
            // instruments are off. We could add an entry but we dont really know what the time is
            // so we probably should not
        }
        lastLogUpdate = now;
    }
}