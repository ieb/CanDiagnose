#include "httpserver.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>


AsyncWebServer server(80);


bool WebServer::getIP(const char * msg, IPAddress *ip) {
    for(int i = 0; i < 3; i++) {
        Serial.println(msg);
        String ipStr = Serial.readStringUntil('\n');
        if ( ip->fromString(ipStr) ) {
            return true;
        }
    }
    return false;
}


void WebServer::configureIP() {
    // prompt for IP configuration
    Serial.println("Reconfigure IP");
    Serial.println("Current Settings");
    Serial.print("IP Address: ");Serial.println(WiFi.localIP());
    Serial.print("Subnet Mask: ");Serial.println(WiFi.subnetMask());
    Serial.print("Gateway IP: ");Serial.println(WiFi.gatewayIP());
    Serial.print("DNS Server: ");Serial.println(WiFi.dnsIP());
    Serial.print("Static or DHCP (s/d):");
    unsigned long to = Serial.getTimeout();
    Serial.setTimeout(60000);
    String change = Serial.readStringUntil('\n');
    Serial.print(change);
    if ( change.startsWith("s") ) {
        IPAddress newIp, newSubnet, newGateway, newDns;
        if ( getIP("Enter new IP Address", &newIp) 
           && getIP("Enter new Subnet Mask", &newSubnet)
           && getIP("Enter new Gateway IP", &newGateway)
           && getIP("Enter new DNS IP", &newDns) ) {
            WiFi.config(newIp, newSubnet, newGateway, newDns);
            Serial.println("New Settings");
            Serial.print("IP Address: ");Serial.println(WiFi.localIP());
            Serial.print("Subnet Mask: ");Serial.println(WiFi.subnetMask());
            Serial.print("Gateway IP: ");Serial.println(WiFi.gatewayIP());
            Serial.print("DNS Server: ");Serial.println(WiFi.dnsIP());
        } else {
            Serial.println("Aborted, no change");
        }
    } else if ( change.startsWith("d") ) {
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
        Serial.println("New Settings");
        Serial.print("IP Address: ");Serial.println(WiFi.localIP());
        Serial.print("Subnet Mask: ");Serial.println(WiFi.subnetMask());
        Serial.print("Gateway IP: ");Serial.println(WiFi.gatewayIP());
        Serial.print("DNS Server: ");Serial.println(WiFi.dnsIP());
    } else {
        Serial.println("No change");
    }
    Serial.setTimeout(to);

}


bool WebServer::ipConfig(IPAddress ip, IPAddress gw, IPAddress sn, IPAddress dns1, IPAddress dns2) {
    // perhaps read from eprom or filesystem, probably can read from SPIFFS
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


    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html", false, [this, request](const String& var){
            return this->handleTemplate(request, var);
        });
    });
    server.on("^\\/(.*)\\.html$", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String path = String("/") + request->pathArg(0) + String(".html");
        Serial.print("GET ");Serial.println(path);
        request->send(SPIFFS, path, "text/html", false, [this, request](const String& var){
            return this->handleTemplate(request, var);
        });
    });
    server.on("^\\/(.*)\\.js$", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String path = String("/") + request->pathArg(0) + String(".js");
        Serial.print("GET ");Serial.println(path);
        request->send(SPIFFS, path, "text/javascript");
    });
    server.on("^\\/(.*)\\.css$", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String path = String("/") + request->pathArg(0) + String(".css");
        Serial.print("GET ");Serial.println(path);
        request->send(SPIFFS, path, "text/css");
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

String WebServer::handleTemplate(AsyncWebServerRequest * request, const String &var) {
    if (var == "WEB_SERVER_URL") {
        String url = "http://";
        url += WiFi.localIP().toString();
        return url;
    }
    return String();
}
