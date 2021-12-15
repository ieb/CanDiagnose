#include "logbook.h"
#include <TimeLib.h>
#define M_TO_NM(x) ((x)/1852.0)
#define SPEED_MS_TO_KN(x) ((x)*1.94384)
#define K_TO_C(x) (((x)-273.15))





LogBook::LogBook(DataCollector *dataCollector, BME280Sensor *bme280): dataCollector{dataCollector}, bme280{bme280} {
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
    if ( now > lastDemoUpdate + 5000) {
        lastDemoUpdate = now;
         dataCollector->log.lastModified = now;
         unsigned long atime = now;
         dataCollector->log.daysSince1970 = 18973+(atime/(24*3600000));
         dataCollector->log.secondsSinceMidnight = (atime%(24*3600000))/1000;
         dataCollector->possition.lastModified = now;
         dataCollector->possition.longitude = 0.11987607061305089-((1.0*now)/(3600000));
         dataCollector->possition.latitude = 52.18623058185942-((1.0*now)/(3600000));
         dataCollector->log.lastModified = now;
         dataCollector->log.log = 1852  + (1852.0*now)/(6000);
         dataCollector->log.tripLog = (1852.0*now)/(6000);
    }

}


void LogBook::log() {
    unsigned long now = millis();
    if ( now > lastLogUpdate + logPeriod) {
        if ( dataCollector->log.lastModified > lastLogUpdate ) {
            // new log data is avaiable, this indicates that the NMEA2000 instruments are on 
            uint16_t daySerial = dataCollector->log.daysSince1970; 
            double seconds = dataCollector->log.secondsSinceMidnight;
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
                f.println("time,logTime,lat,long,log,trip,cog,sog,stw,hdg,awa,aws,mbar,rpm");
            } else {
                Serial.print("Opening ");Serial.println(filename);
                f = SPIFFS.open(filename,"a");
            }


            f.printf("%04d-%02d-%02dT%02d:%02d:%02dZ",tm.Year+1970, tm.Month, tm.Day,tm.Hour,tm.Minute,tm.Second);
            f.printf(",%ld",(unsigned long)tofLog);
            // lat, lon

            if ( dataCollector->possition.lastModified+15000 < dataCollector->gnss[0].lastModified ) {
                f.printf(",%10.6f,%10.6f",dataCollector->gnss[0].latitude, dataCollector->gnss[0].longitude);
            } else if (dataCollector->possition.lastModified+30000 > now) {
                f.printf(",%10.6f,%10.6f",dataCollector->possition.latitude, dataCollector->possition.longitude);
            } else {
                f.printf(",,");
            } 
            //log,trip
            if ( dataCollector->log.lastModified+30000 > now) {
                f.printf(",%6.1f,%6.1f",M_TO_NM(dataCollector->log.log),M_TO_NM(dataCollector->log.tripLog));
            } else {
                f.print(",,");
            }
            // cog,sog  radians, m/s
            int i;
            for ( i = 0; i < MAX_COGSOG_SOURCES; i++) {
                if (dataCollector->cogSog[i].lastModified+30000 > now ) {
                    f.printf(",%d,%4.1f",(int)headingAngleToDeg(dataCollector->cogSog[i].cog),
                         SPEED_MS_TO_KN(dataCollector->cogSog[i].sog));
                    break;
                } 
            }
            if ( i == MAX_COGSOG_SOURCES ) {
                f.print(",,");
            }
            // stw m/s
            for ( i=0; i < MAX_SPEED_SOURCES; i++) {
                if (dataCollector->speed[i].lastModified+30000 > now ) {
                    f.printf(",%4.1f",SPEED_MS_TO_KN(dataCollector->speed[i].sow));
                    break;
                } 
            }
            if ( i == MAX_SPEED_SOURCES ) {
                f.print(",");
            }
            //hdg radians
            for ( i=0; i < MAX_HEADDING_SOURCES; i++) {
                if (dataCollector->heading[i].lastModified+30000 > now ) {
                    f.printf(",%d",(int)headingAngleToDeg(dataCollector->heading[i].heading));
                    break;
                } 
            }
            if ( i == MAX_HEADDING_SOURCES ) {
                f.print(",");
            }


            //aws, awa m/s, radians
            if ( dataCollector->wind.lastModified+30000 > now) {
                f.printf(",%4.1f,%d",SPEED_MS_TO_KN(dataCollector->wind.windSpeed),(int)relativeAngleToDeg(dataCollector->wind.windAngle));
            } else {
                f.print(",,");
            }
            // mbar mbar
            f.printf(",%6.1f",bme280->getPressure());
            // rpm, ct, rpm, K
            for ( i = 0; i < MAX_ENGINE_SOURCES; i++) {
                if (dataCollector->engine[i].lastModified+30000 > now ) {
                    f.printf(",%d,%d",(int)dataCollector->engine[i].speed,(int)K_TO_C(dataCollector->engine[i].CoolantTemp));
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