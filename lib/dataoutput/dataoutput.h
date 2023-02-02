#pragma once

/**
 * Dataoutput methods for N2K messages stored in the NK2Collector
 */

#include "httpserver.h"
#include "N2KCollector.h"

class EngineDataOutput: public JsonOutput,  public CsvOutput {
    public:
        EngineDataOutput(N2KCollector &n2kCollector):  n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream); 

    private:
        N2KCollector &n2kCollector;

};

class BoatDataOutput: public JsonOutput,  public CsvOutput {
    public:
        BoatDataOutput(N2KCollector &n2kCollector): n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
    private:
        N2KCollector &n2kCollector;
};
class NavigationDataOutput: public JsonOutput,  public CsvOutput { 
    public:
        NavigationDataOutput(N2KCollector &n2kCollector): n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);

    private:
        Stream *outputStream;
        N2KCollector &n2kCollector;
};

class EnvironmentDataOutput: public JsonOutput,  public CsvOutput  {
    public:
        EnvironmentDataOutput(N2KCollector &n2kCollector): 
                n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);

    private:
        Stream *outputStream;
        N2KCollector &n2kCollector;
};
class TemperatureDataOutput: public JsonOutput ,  public CsvOutput {
    public:
        TemperatureDataOutput(N2KCollector &n2kCollector): n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        N2KCollector &n2kCollector;
};

class XteDataOutput: public JsonOutput,  public CsvOutput  {
    public:
        XteDataOutput(N2KCollector &n2kCollector): n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        N2KCollector &n2kCollector;
};

class MagneticVariationDataOutput: public JsonOutput, public CsvOutput {
    public:
        MagneticVariationDataOutput(N2KCollector &n2kCollector): n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        N2KCollector &n2kCollector;
};

class WindSpeedDataOutput: public JsonOutput, public CsvOutput {
    public:
        WindSpeedDataOutput(N2KCollector &n2kCollector): n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);

    private:
        Stream *outputStream;
        N2KCollector &n2kCollector;
};
class LogDataOutput: public JsonOutput, public CsvOutput {
    public:
        LogDataOutput(N2KCollector &n2kCollector): n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        N2KCollector &n2kCollector;
};
class LatLonDataOutput: public JsonOutput, public CsvOutput {
    public:
        LatLonDataOutput(N2KCollector &n2kCollector): n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        N2KCollector &n2kCollector;
};
class LeewayDataOutput: public JsonOutput,  public CsvOutput {
    public:
        LeewayDataOutput(N2KCollector &n2kCollector): n2kCollector{n2kCollector} {};
        void outputJson(AsyncResponseStream *outputStream);
        void outputCsv(AsyncResponseStream *outputStream);
    private:
        Stream *outputStream;
        N2KCollector &n2kCollector;
};



