#ifndef HTTPSERVER_H
#define HTTPSERVER_H



#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include "local_secrets.h"
#include "display.h"

#ifndef WIFI_SSID
#error "WIFI_SSID not defined, add in local_secrets.h file"
#endif
#ifndef WIFI_PASS
#error "WIFI_PASS not defined, add in local_secrets.h file"
#endif



class JsonOutput {
    public:
        JsonOutput() {};
        virtual void outputJson(AsyncResponseStream* outputStream) {};
        void append(const char *key, const char *value);
        void append(const char *value);
        void append(int value);
        void append(double value);
        void append(unsigned long value);
        void appendCommaIfRequired();
        void append(const char *key, int value);
        void append(const char *key, double value, int precision=2);
        void append(const char *key, unsigned int value);
        void append(const char *key, unsigned long value);
        void startObject();
        void startObject(const char *key);
        void endObject();
        void startArray(const char *key);
        void endArray();
        void startJson(AsyncResponseStream *outputStream);
        void endJson();
    protected:
        bool levels[10];
        int level = 0;
        AsyncResponseStream *outputStream;

};

class CsvOutput {
    public:
        CsvOutput() {};
        virtual void outputCsv(AsyncResponseStream *outputStream) {};
        void startBlock(AsyncResponseStream *outputStream);
        void endBlock();
        void startRecord(const char *);
        void endRecord();
        void appendField(const char *value);
        void appendField(int value);
        void appendField(double value, int precision = 2);
        void appendField(unsigned long value);
        void appendField(uint32_t value);
    protected:
        AsyncResponseStream *outputStream;
};

#define MAX_DATASETS 14

class WebServer : public DisplayPage {
    public:
        WebServer(Stream *outputStream) : outputStream{outputStream} {};
        void begin(const char * configurationFile = "/config.txt");
        void addJsonOutputHandler(int id, JsonOutput *handler) {
            if ( id >= 0 && id < MAX_DATASETS ) {
                jsonHandlers[id] = handler;
            }
        };
        void addCsvOutputHandler(int id, CsvOutput *handler) {
            if ( id >= 0 && id < MAX_DATASETS ) {
                csvHandlers[id] = handler;
            }
        };

        bool drawPage(Adafruit_SSD1306 * display);
    private:    
        String handleTemplate(AsyncWebServerRequest * request, const String &var);
        void handleAllFileUploads(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
        bool authorized(AsyncWebServerRequest *request);
        JsonOutput *jsonHandlers[MAX_DATASETS]={ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
        CsvOutput *csvHandlers[MAX_DATASETS]={ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
        Stream *outputStream;
        String httpauth;
        String ssid;
        String password;
        String basicAuth;

};



#endif