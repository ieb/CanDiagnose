
#include <math.h>

#include "N2KCollector.h"
#include "pogo1250polar.h"


/**
 *  Calculate the polar speed by patch interpolation.
 *  input is in ms and radians unless nmea2000Units is false, in which case deg ann kn.
 *  output is in ms/s
 */ 
float PerformanceData::calcPolarSpeed(float twsv, float twav, bool nmea2000Units) {
    // polar Data is in KN and deg
    if ( nmea2000Units ) {
    	twsv = msToKnots(twsv);
    	twav = RadToDeg(twav);
    }

	updateTWSIdx(twsv);
	updateTWAIdx(twav);

	float twa[] = {
		(float)1.0*polar_twa[this->twaidx[0]],
		(float)1.0*polar_twa[this->twaidx[1]]
	};
	float tws[] = {
		(float)1.0*polar_tws[this->twsidx[0]],
		(float)1.0*polar_tws[this->twsidx[1]]
	};
	// polar is stored in 1/10th of a knot.
	float stw[] = {
		(float)0.1*polar_map[POLAR_MAP_LOOKUP(this->twsidx[0], this->twaidx[0])],
		(float)0.1*polar_map[POLAR_MAP_LOOKUP(this->twsidx[1], this->twaidx[0])],
		(float)0.1*polar_map[POLAR_MAP_LOOKUP(this->twsidx[0], this->twaidx[1])],
		(float)0.1*polar_map[POLAR_MAP_LOOKUP(this->twsidx[1], this->twaidx[1])]
	};
	// perform a very simple linear patch interpolation.
    // interpolate a stw low value for a given tws and range
    float stwl = interpolateForY(twav, twa[0], twa[1], stw[0], stw[2]);
    // interpolate a stw high value for a given tws and range
    float stwh = interpolateForY(twav, twa[0], twa[1], stw[1], stw[3]);

    // interpolate a stw final value for a given tws and range using the high an low values for twa.
    return KnotsToms(interpolateForY(twsv, tws[0], tws[1], stwl, stwh)); // in m/s

}

void PerformanceData::updateTWSIdx(float tws) {
	uint8_t v = (uint8_t)(tws);
	if ( v != this->lastTWSV) {
		findIndexes(v,  polar_tws, POLAR_NTWS, this->twsidx);
		this->lastTWSV = v;
	}
}

void PerformanceData::updateTWAIdx(float twa) {
	uint8_t v = (uint8_t)(twa);
	if ( v != this->lastTWAV) {
		findIndexes(v,  polar_twa, POLAR_NTWA, this->twaidx);
		this->lastTWAV = v;
	}
}

void PerformanceData::findIndexes(uint8_t v, const uint8_t *a, uint8_t al, int8_t *idx  ) {
	for (int i = 0; i < al; i++) {
	    if ( a[i] > v ) {
	        if ( i == 0 ) {
	            idx[0] = 0;
	            idx[1] = 0;
	            return;
	        } else {
	            idx[0] = i-1;
	            idx[1] = i;
	            return;
	        }
	    }
	}
	idx[0] = al-1;
	idx[1] = al-1;
}


float PerformanceData::interpolateForY(float x, float xl, float xh, float yl, float yh) {
    if ( x >= xh ) {
       return yh;
    } else if ( x <= xl ) {
       return yl;
    } else if ( abs(xh-xl) < 1.0E-4 ) { // could happen on stw interpolation.
       return (yl+yh)/2;
    } else {
       return yl+(yh-yl)*((x-xl)/(xh-xl));
    }
}


float PerformanceData::correctBearing(float b) {
	if ( b > (2*PI) ) {
		return b-2*PI;
	} else if ( b < 0 ) {
		return b+2*PI;
	} else {
		return b;
	}
}


void PerformanceData::update(N2KCollector * n2kCollector) {

    WindData *aparentWind = n2kCollector->getAparentWind();
    SpeedData * speed = n2kCollector->getSpeed();
    HeadingData * heading = n2kCollector->getHeading();
    AttitudeData * attitude = n2kCollector->getAttitude();


    // calculate leeway
	this->leeway = 0;
    if ( aparentWind != NULL && speed != NULL && attitude != NULL) {
	    if ( abs(aparentWind->windAngle) < DegToRad(90) && aparentWind->windSpeed < KnotsToms(30.0)) {
	        if (speed->stw > 0.5) {
	            // This comes from Pedrick see http://www.sname.org/HigherLogic/System/DownloadDocumentFile.ashx?DocumentFileKey=5d932796-f926-4262-88f4-aaca17789bb0
	            // for aws < 30 and awa < 90. UK  =15 for masthead and 5 for fractional
	            this->leeway  = 5 * attitude->roll / (speed->stw * speed->stw);
	        }
	    }
    }

    // calculate true wind.
    if ( aparentWind != NULL && speed != NULL ) {
	    double aparentX = cos(aparentWind->windAngle) * aparentWind->windSpeed;
	    double aparentY = sin(aparentWind->windAngle) * aparentWind->windSpeed;
	    this->trueWind.windAngle =  atan2(aparentY, -speed->stw + aparentX);
	    this->trueWind.windSpeed = sqrt(pow(aparentY, 2) + pow(-speed->stw + aparentX, 2));




	    // calculate polar speed
	    float abs_twa = this->trueWind.windAngle;
	    if ( abs_twa < 0 ) abs_twa = -abs_twa;
	    this->polarSpeed = calcPolarSpeed(this->trueWind.windSpeed, abs_twa);
	    if ( this->polarSpeed > 1E-8) {
		    this->polarSpeedRatio = speed->stw/this->polarSpeed;
	    }
	    // calculate vmgs
	    float cos_twa = cos(abs_twa);
	    this->polarVmg = polarSpeed * cos_twa;
	    this->vmg = speed->stw * cos_twa;

	    //calculate optimal vmg angles, use degrees to iterate
	    int twal = 0, twah = 180, twa = RadToDeg(abs_twa);
	    if ( twa < 90) {
	    	// upwind
	    	twah = 90;
	    } else {
	    	// downwind.
	    	twal = 90;
	    }
	    this->targetVmg = 0;
	    float tws_kn = msToKnots(this->trueWind.windSpeed);
	    for (int ttwa = twal; ttwa <= twah; ttwa++) {
	    	float tstw = calcPolarSpeed(tws_kn, ttwa, false);
	    	float tvmg = tstw*cos(DegToRad(ttwa));
	    	if ( abs(tvmg) > abs(this->targetVmg) ) {
	    		this->targetVmg = tvmg;
	    		this->targetTwa = ttwa;
	    		this->targetStw = tstw;
	    	}
	    }
	    if ( this->trueWind.windAngle < 0) {
	    	this->targetTwa = -DegToRad(this->targetTwa);
	    } else {
	    	this->targetTwa = DegToRad(this->targetTwa);
	    }

	    // calculate the polar vmg Ratio
	    if ( abs(this->targetVmg) > 1.0E-8 ) {
		    this->polarVmgRatio = this->vmg/this->targetVmg;
	    }

	    if ( heading != NULL ) {
		    // calculate other track, depending on what the heading is
		    if ( heading->reference == N2khr_true ) {
			    this->windDirectionTrue = correctBearing(heading->heading+this->trueWind.windAngle);
			    this->windDirectionMagnetic = correctBearing(heading->heading+heading->variation + this->trueWind.windAngle);
			} else if ( heading->reference ==  N2khr_magnetic ) {
			    this->windDirectionMagnetic = correctBearing(heading->heading+this->trueWind.windAngle);
			    this->windDirectionTrue = correctBearing(heading->heading-heading->variation + this->trueWind.windAngle);
			} else {
				// cant calculate, no heading data
			}
			this->oppositeTrackHeadingTrue = correctBearing(this->windDirectionTrue + this->targetTwa);
			this->oppositeTrackHeadingMagnetic = correctBearing(this->oppositeTrackHeadingTrue + heading->variation);

			// calculate track taking account of leeway.
			if (this->trueWind.windAngle > 0 ) {
				this->oppositeTrackTrue = correctBearing(this->oppositeTrackHeadingTrue + this->leeway*2);
			} else {
				this->oppositeTrackTrue = correctBearing(this->oppositeTrackHeadingTrue - this->leeway*2);
			}
			this->oppositeTrackMagnetic = correctBearing(this->oppositeTrackTrue + heading->variation);
		}
	}

}