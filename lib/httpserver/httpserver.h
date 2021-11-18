#ifndef HTTPSERVER_H
#define HTTPSERVER_H



#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>


class JsonOutput {
    public:
        JsonOutput() {};
        virtual void outputJson(AsyncResponseStream* outputStream);
        void append(const char *key, const char *value) {
            appendCommaIfRequired();
            jsonOutput->print("\"");
            jsonOutput->print(key);
            jsonOutput->print("\":\"");
            jsonOutput->print(value);
            jsonOutput->print("\"");
        };
        void append(const char *value) {
            appendCommaIfRequired();
            jsonOutput->print("\"");
            jsonOutput->print(value);
            jsonOutput->print("\"");
        }
        void append(int value) {
            appendCommaIfRequired();
            jsonOutput->print(value);
        }
        void append(double value) {
            appendCommaIfRequired();
            jsonOutput->print(value);
        }
        void append(unsigned long value) {
            appendCommaIfRequired();
            jsonOutput->print(value);
        }

        void appendCommaIfRequired() {
            if (levels[level]) {
                levels[level] = false;
            } else {
                jsonOutput->print(",");
            }
        };

        void append(const char *key, int value) {
            appendCommaIfRequired();
            jsonOutput->print("\"");
            jsonOutput->print(key);
            jsonOutput->print("\":");
            jsonOutput->print(value);
        };
        void append(const char *key, double value, int precision=2) {
            appendCommaIfRequired();
            jsonOutput->print("\"");
            jsonOutput->print(key);
            jsonOutput->print("\":");
            jsonOutput->print(value,precision);
        };
        void append(const char *key, unsigned int value) {
            appendCommaIfRequired();
            jsonOutput->print("\"");
            jsonOutput->print(key);
            jsonOutput->print("\":");
            jsonOutput->print(value);
        };

        void append(const char *key, unsigned long value) {
            appendCommaIfRequired();
            jsonOutput->print("\"");
            jsonOutput->print(key);
            jsonOutput->print("\":");
            jsonOutput->print(value);
        };
        void startObject() {
            appendCommaIfRequired();
            jsonOutput->print("{");
            level++;
            levels[level] = true;
        };
        void startObject(const char *key) {
            appendCommaIfRequired();
            jsonOutput->print("\"");
            jsonOutput->print(key);
            jsonOutput->print("\":{");
            level++;
            levels[level] = true;
        };
        void endObject() {
            jsonOutput->print("}");
            level--;
        };
        void startArray(const char *key) {
            appendCommaIfRequired();
            jsonOutput->print("\"");
            jsonOutput->print(key);
            jsonOutput->print("\":[");
            level++;
            levels[level] = true;
        };
        void endArray() {
            jsonOutput->print("]");
            level--;
        };
        void startJson(AsyncResponseStream *outputStream) {
            jsonOutput = outputStream;
            jsonOutput->print("{");
            level=0;
            levels[level] = true;
        };
        void endJson() {
            jsonOutput->print("}");
            jsonOutput = NULL;
        };
    protected:
        bool levels[10];
        int level = 0;
        AsyncResponseStream *jsonOutput;

};

#define MAX_DATASETS 10

class WebServer {
    public:
        WebServer(Stream *outputStream) : outputStream{outputStream} {};
        void configureIP();
        bool ipConfig(IPAddress ip, IPAddress gw, IPAddress sn, IPAddress dns1=IPAddress(8,8,8,8), IPAddress dns2=IPAddress(4,4,4,4));
        void begin(const char * ssid, const char * password);
        void addDataSet(int id, JsonOutput *dataSet) {
            if ( id >= 0 && id < MAX_DATASETS ) {
                dataSets[id] = dataSet;
            }
        };
        
    private:    
        String handleTemplate(AsyncWebServerRequest * request, const String &var);
        void handleAllFileUploads(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
        bool getIP(const char * msg, IPAddress *ip);
        JsonOutput *dataSets[MAX_DATASETS]={ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
        Stream *outputStream;
};



#endif