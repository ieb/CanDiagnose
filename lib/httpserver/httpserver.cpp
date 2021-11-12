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
            response->setCode(200);
            this->dataSets[id]->outputJson(response);
            request->send(response);
        } else {
            request->send(404, "text/plain", "DataSet Not found");
        }
        Serial.print(" ");
        Serial.println(millis() - start);
    });
    server.onNotFound([](AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS) {
            request->send(200);
        } else {
            request->send(404);
        }
    });

    server.begin();
};
