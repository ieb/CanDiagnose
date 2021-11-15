#include "httpserver.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>


AsyncWebServer server(80);



bool WebServer::ipConfig(IPAddress ip, IPAddress gw, IPAddress sn, IPAddress dns1, IPAddress dns2) {
    return WiFi.config(ip, gw, sn, dns1, dns2);
}

void WebServer::begin(const char * ssid, const char * password) {

      // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    outputStream->println("");
    outputStream->println("WiFi connected.");
    outputStream->println("IP address: ");
    outputStream->println(WiFi.localIP());

    MDNS.begin("boatsystems");
    MDNS.addService("_http","_tcp",80);


    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });
    
    server.on("^\\/api\\/data\\/([0-9]+)$", HTTP_GET, [this](AsyncWebServerRequest *request) {
        int id = request->pathArg(0).toInt();
        unsigned long start = millis();
        Serial.print("http GET /api/data/");
        Serial.print(id);
        
        if ( id >= 0 && id < MAX_DATASETS && this->dataSets[id] != NULL) {
            AsyncResponseStream *response = request->beginResponseStream("application/json");
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->setCode(200);
            this->dataSets[id]->outputJson(response);
            request->send(response);
        } else {
            AsyncResponseStream *response = request->beginResponseStream("application/json");
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->setCode(404);
            request->send(response);
        }
        Serial.print(" ");
        Serial.println(millis() - start);
    });
    server.on("/api/data/all", HTTP_GET, [this](AsyncWebServerRequest *request) {
        unsigned long start = millis();
        Serial.print("http GET /api/data/all");
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->setCode(200);
        response->print("{");
        bool first = true;
        for (int i = 0; i < MAX_DATASETS; i++) {
            if (this->dataSets[i] != NULL) {
                if (!first) {
                    response->print(",");
                }
                first = false;
                response->print("\"");response->print(i);response->print("\":");
                this->dataSets[i]->outputJson(response);
            }
        }
        response->print("}");
        request->send(response);
        Serial.print(" ");
        Serial.println(millis() - start);
    });

    server.onNotFound([](AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS) {
            AsyncWebServerResponse *response = request->beginResponse(200);
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
        } else {
            AsyncWebServerResponse *response = request->beginResponse(404);
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
        }
    });

    server.begin();
};
