#include "httpserver.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include "config.h"
#include <sys/random.h>
#include <base64.h>

AsyncWebServer server(80);


void WebServer::begin(const char * configurationFile) {
      // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    String ssid = WIFI_SSID;
    String password = WIFI_PASS;
    ConfigurationFile::get(configurationFile, "wifi.ssid:", ssid);
    Serial.print("Wifi ssid");Serial.println(ssid);

    if ( !ConfigurationFile::get(configurationFile, ssid+".password:", password) ) {
        Serial.println("Warning: no password configured, using detault");
    }
    String v;
    if ( ConfigurationFile::get(configurationFile, ssid+".ip", v)) {
        IPAddress local_ip;
        local_ip.fromString(v);
        IPAddress subnet(255,255,255,0), 
            gateway(local_ip[0],local_ip[1],local_ip[2],1), 
            dns1(local_ip[0],local_ip[1],local_ip[2],1), 
            dns2;
        dns2 = IPADDR_NONE;
        if (ConfigurationFile::get(configurationFile, ssid+".netmask", v) ) {
            subnet.fromString(v);
        }
        if (ConfigurationFile::get(configurationFile, ssid+".gateway", v) ) {
            gateway.fromString(v);
        }
        if (ConfigurationFile::get(configurationFile, ssid+".dns1", v) ) {
            dns1.fromString(v);
        }
        if (ConfigurationFile::get(configurationFile, ssid+".dns2", v) ) {
            dns2.fromString(v);
        }
        WiFi.config(local_ip, gateway, subnet, dns1, dns2);
        Serial.println("Network config loaded.");
    } else {
#ifdef IP_METHOD
        WiFi.config(WIFI_IP, WIFI_GATEWAY, WIFI_SUBNET, WIFI_DNS1);
        Serial.println("Using network config defaults.");
#else
        Serial.print("Using DHCP");
#endif
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    outputStream->println("");
    outputStream->println("WiFi connected.");
    Serial.print("IP Address: ");Serial.println(WiFi.localIP());
    Serial.print("Subnet Mask: ");Serial.println(WiFi.subnetMask());
    Serial.print("Gateway IP: ");Serial.println(WiFi.gatewayIP());
    Serial.print("DNS Server: ");Serial.println(WiFi.dnsIP());

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


    // management
    server.on("/admin/status", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if ( this->authorized(request) ) {
            AsyncResponseStream *response = request->beginResponseStream("application/json");
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->setCode(200);
            size_t total = SPIFFS.totalBytes();
            size_t used = SPIFFS.usedBytes();
            size_t free = total-used;
            response->print("{ \"heap\":");response->print(ESP.getFreeHeap());
            response->print(", \"disk\": { \"tota\":");response->print(total);
            response->print(", \"used\":");response->print(used);
            response->print(", \"free\":");response->print(free);
            response->println("}}");
            request->send(response);
        }
    });
    server.on("/admin/reboot", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if ( this->authorized(request) ) {
            request->send(200, "application/json", "{ \"ok\":true, \"msg\":\"reboot in 1s\" }");
            Serial.println("Rebooting in 1s, requested by Browser");
            delay(1000);
            ESP.restart();
        }
    });
    server.on("/admin/config.txt", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if ( this->authorized(request) ) {
            request->send(SPIFFS, "/config.txt", "text/plain");
        }
    });
    server.onFileUpload([this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
         //Handle upload
        this->handleAllFileUploads(request, filename, index, data, len, final);
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


    if ( !ConfigurationFile::get(configurationFile, "httpauth:", httpauth) ) {
        byte buffer[12];
        getrandom(&buffer[0], 12, 0);
        String basicAuth = "admin:";
        for(int i = 0; i < 12; i++) {
            basicAuth += (char)((48+buffer[i]%(125-48)));
        }
        httpauth = "Basic "+base64::encode(basicAuth);
        Serial.printf("Using generated http basic auth admin password: %s\n", basicAuth.c_str());
        Serial.printf("Use Header: Authorization: %s\n", httpauth.c_str());
    } else {
        Serial.println("Configured http basic auth");
    }


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


void WebServer::handleAllFileUploads(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    if ( authorized(request) ) {
        if ( request->url().equals("/admin/config.txt")) {
            File file = SPIFFS.open("/config.txt", FILE_WRITE);
            if ( file ) {
                if ( file.write(data,len) == len ) {
                    file.close();
                    request->send(200, "application/json", "{ \"ok\":true, \"msg\":\"saved\" }");
                } else {
                    file.close();
                    request->send(500, "application/json", "{ \"ok\":false, \"msg\":\"upload incomplete\" }");
                }
            } else {
                    request->send(500, "application/json", "{ \"ok\":false, \"msg\":\"Unable to update config file\" }");
            }
        } else {
            request->send(500, "application/json", "{ \"ok\":false, \"msg\":\"multi part posts not supported\" }");
        }
    }
}

bool WebServer::authorized(AsyncWebServerRequest *request) {
    AsyncWebHeader *authorization = request->getHeader("Authorization");
    if ( authorization == NULL || !httpauth.equals(authorization->value()) ) {
        AsyncWebServerResponse * response = request->beginResponse(401,"application/json","{ \"ok\": false, \"msg\":\"not authorized\"}");
        response->addHeader("WWW-Authenticate","Basic realm=\"BoatSystems Admin\", charset=\"UTF-8\"");
        request->send(response);
        return false;
    } else {
        return true;
    }
}




void JsonOutput::append(const char *key, const char *value) {
    appendCommaIfRequired();
    outputStream->print("\"");
    outputStream->print(key);
    outputStream->print("\":\"");
    outputStream->print(value);
    outputStream->print("\"");
};
void JsonOutput::append(const char *value) {
    appendCommaIfRequired();
    outputStream->print("\"");
    outputStream->print(value);
    outputStream->print("\"");
}
void JsonOutput::append(int value) {
    appendCommaIfRequired();
    outputStream->print(value);
}
void JsonOutput::append(double value) {
    appendCommaIfRequired();
    outputStream->print(value);
}
void JsonOutput::append(unsigned long value) {
    appendCommaIfRequired();
    outputStream->print(value);
}

void JsonOutput::appendCommaIfRequired() {
    if (levels[level]) {
        levels[level] = false;
    } else {
        outputStream->print(",");
    }
};

void JsonOutput::append(const char *key, int value) {
    appendCommaIfRequired();
    outputStream->print("\"");
    outputStream->print(key);
    outputStream->print("\":");
    outputStream->print(value);
};
void JsonOutput::append(const char *key, double value, int precision) {
    appendCommaIfRequired();
    outputStream->print("\"");
    outputStream->print(key);
    outputStream->print("\":");
    outputStream->print(value,precision);
};
void JsonOutput::append(const char *key, unsigned int value) {
    appendCommaIfRequired();
    outputStream->print("\"");
    outputStream->print(key);
    outputStream->print("\":");
    outputStream->print(value);
};

void JsonOutput::append(const char *key, unsigned long value) {
    appendCommaIfRequired();
    outputStream->print("\"");
    outputStream->print(key);
    outputStream->print("\":");
    outputStream->print(value);
};
void JsonOutput::startObject() {
    appendCommaIfRequired();
    outputStream->print("{");
    level++;
    levels[level] = true;
};
void JsonOutput::startObject(const char *key) {
    appendCommaIfRequired();
    outputStream->print("\"");
    outputStream->print(key);
    outputStream->print("\":{");
    level++;
    levels[level] = true;
};
void JsonOutput::endObject() {
    outputStream->print("}");
    level--;
};
void JsonOutput::startArray(const char *key) {
    appendCommaIfRequired();
    outputStream->print("\"");
    outputStream->print(key);
    outputStream->print("\":[");
    level++;
    levels[level] = true;
};
void JsonOutput::endArray() {
    outputStream->print("]");
    level--;
};
void JsonOutput::startJson(AsyncResponseStream *outputStream) {
    this->outputStream = outputStream;
    outputStream->print("{");
    level=0;
    levels[level] = true;
};
void JsonOutput::endJson() {
    outputStream->print("}");
    outputStream = NULL;
};