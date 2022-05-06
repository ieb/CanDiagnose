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
    if(!SPIFFS.begin(false)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    ssid = WIFI_SSID;
    password = WIFI_PASS;
    ConfigurationFile::get(configurationFile, "wifi.ssid:", ssid);
    Serial.print("Wifi ssid ");Serial.println(ssid);

    if ( !ConfigurationFile::get(configurationFile, ssid+".password:", password) ) {
        Serial.println("Warning: no password configured, using default");
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
    for  (int i = 0; i < 30; i++) {
        if ( WiFi.status() == WL_CONNECTED ) {
            break;
        }
        delay(500);
        Serial.print(".");
    }
    if ( WiFi.status() != WL_CONNECTED ) {
        WiFi.disconnect(true, true);
        password = "nopassword";
        ssid = "boatsys";
        ConfigurationFile::get(configurationFile, "softap.password:", password);
        outputStream->print("WiFi connect failed, switching to SofAP on SSID boatsys PW ");Serial.println(password);
        WiFi.softAP(ssid.c_str(), password.c_str());
        
        Serial.print("IP address: ");
        Serial.println(WiFi.softAPIP());
    } else {
        // Print local IP address and start web server
        outputStream->println("");
        outputStream->println("WiFi connected.");
        Serial.print("IP Address: ");Serial.println(WiFi.localIP());
        Serial.print("Subnet Mask: ");Serial.println(WiFi.subnetMask());
        Serial.print("Gateway IP: ");Serial.println(WiFi.gatewayIP());
        Serial.print("DNS Server: ");Serial.println(WiFi.dnsIP());

    }

    MDNS.begin("boatsystems");
    MDNS.addService("_http","_tcp",80);


    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html", false, [this, request](const String& var){
            return this->handleTemplate(request, var);
        });
    });
    server.on("/admin.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if ( this->authorized(request) ) {
            request->send(SPIFFS, "/admin.html", "text/html", false, [this, request](const String& var){
                return this->handleTemplate(request, var);
            });
        }
    });
    server.on("/admin.js", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if ( this->authorized(request) ) {
            request->send(SPIFFS, "/admin.js", "application/javascript");
        }
    });
    server.on("/admin.css", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if ( this->authorized(request) ) {
            request->send(SPIFFS, "/admin.css", "text/css");
        }
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



    server.on("/api/logbook.json", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->setCode(200);
        response->print("{ \"logbooks\": [ \"/api/logbookj.json\" ");

        Serial.println("GET /api/logbook.json");
        File f = SPIFFS.open("/logbook");
        File lf = f.openNextFile();
        while(lf) {
            response->printf(",\n{ \"name\":\"%s\", \"size\":%d }",lf.name(),lf.size());
            lf = f.openNextFile();
        }
        response->print("\n]}");
        request->send(response);
    });

    server.on("^\\/logbook/(.*)$", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String path = String("/logbook/") + request->pathArg(0);
        Serial.print("GET ");Serial.println(path);
        request->send(SPIFFS, path, "text/plain");
    });

    server.on("^\\/logbook/(.*)$", HTTP_DELETE, [this](AsyncWebServerRequest *request) {
        String path = String("/logbook/") + request->pathArg(0);
        Serial.print("DELETE ");Serial.println(path);
        if ( SPIFFS.exists(path)) {
            SPIFFS.remove(path);
            request->send(200,"application/json","{ \"ok\":true,\"msg\":\"deleted\"}");
        } else {
            request->send(404);
        }
    });



    server.on("^\\/api\\/data\\/([0-9]*).csv$", HTTP_GET, [this](AsyncWebServerRequest *request) {
        int id = request->pathArg(0).toInt();
        unsigned long start = millis();
        Serial.print("http GET /api/data/");
        Serial.print(id);
        
        if ( id >= 0 && id < MAX_DATASETS && this->dataSets[id] != NULL) {
            AsyncResponseStream *response = request->beginResponseStream("text/csv");
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->setCode(200);
            ((CsvOutput *)(this->dataSets[id]))->outputCsv(response);
            request->send(response);
        } else {
            AsyncResponseStream *response = request->beginResponseStream("text/csv");
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->setCode(404);
            request->send(response);
        }
        Serial.print(" ");
        Serial.println(millis() - start);
    });


    server.on("^\\/api\\/data\\/([0-9]*).json$", HTTP_GET, [this](AsyncWebServerRequest *request) {
        int id = request->pathArg(0).toInt();
        unsigned long start = millis();
        Serial.print("http GET /api/data/");
        Serial.print(id);
        
        if ( id >= 0 && id < MAX_DATASETS && this->dataSets[id] != NULL) {
            AsyncResponseStream *response = request->beginResponseStream("application/json");
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->setCode(200);
            ((JsonOutput *)(this->dataSets[id]))->outputJson(response);
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
    server.on("/api/data/all.json", HTTP_GET, [this](AsyncWebServerRequest *request) {
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
                ((JsonOutput *)(this->dataSets[i]))->outputJson(response);
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
    server.on("/admin/reboot", HTTP_POST, [this](AsyncWebServerRequest *request) {
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


    if ( !ConfigurationFile::get(configurationFile, "httpauth:", basicAuth) ) {
        byte buffer[12];
        getrandom(&buffer[0], 12, 0);
        basicAuth = "admin:";
        for(int i = 0; i < 12; i++) {
            basicAuth += (char)((48+buffer[i]%(125-48)));
        }
        httpauth = "Basic "+base64::encode(basicAuth);
        Serial.printf("Using generated http basic auth admin password: %s\n", basicAuth.c_str());
        Serial.printf("Use Header: Authorization: %s\n", httpauth.c_str());
    } else {
        httpauth = "Basic "+base64::encode(basicAuth);
        Serial.print("Configured http Authorzation header to be ");
        Serial.println(httpauth);
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



void CsvOutput::startBlock(AsyncResponseStream *outputStream) {
        this->outputStream = outputStream;
        startRecord("t");
        appendField(millis());
        endRecord();
};

void CsvOutput::endBlock() {
        this->outputStream = NULL;
};

void CsvOutput::startRecord(const char *name) {
    outputStream->print(name);
};

void CsvOutput::endRecord() {
    outputStream->print("\n");
};


void CsvOutput::appendField(const char *value) {
    outputStream->print(",");
    outputStream->print(value);
}
void CsvOutput::appendField(int value) {
    outputStream->print(",");
    outputStream->print(value);
}
void CsvOutput::appendField(double value) {
    outputStream->print(",");
    outputStream->print(value);
}
void CsvOutput::appendField(unsigned long value) {
    outputStream->print(",");
    outputStream->print(value);
}
void CsvOutput::appendField(uint32_t value) {
    outputStream->print(",");
    outputStream->print(value);
}




bool WebServer::drawPage(Adafruit_SSD1306 * display) {
    display->clearDisplay();
    display->setTextSize(1);              // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE); // Draw white text
    display->setCursor(0,0);              // Start at top-left corner
#if OLED_HEIGHT == 32
    switch(subPage) {
        case 0:
            display->setTextSize(1);   // 21 chars, 4 lines
            display->print("SSID:");display->println(ssid);
            if ( ssid.equals("boatsys")) {
                display->print("Password:");display->println(password);
            } 
            display->display();
            subPage = 1;
            return false;
        case 1:
            display->setTextSize(1);   // 21 chars, 4 lines
            display->print("IP:");display->println(WiFi.localIP());
            display->print("Auth:");display->println(basicAuth);


            display->display();
            subPage = 0;
            return true;
        default:
            subPage = 0;
            return false;
    }
#else
    // no subpage required
    display->setTextSize(1);   // 12x16, 4 rows, 10.6 chars
    display->print("SSID:");display->println(ssid);
    if ( ssid.equals("boatsys")) {
        display->print("Password:");display->println(password);
    } 
    display->print("IP:");display->println(WiFi.localIP());
    display->print("Auth:");display->println(basicAuth);
    display->print("Free Heap:");display->println(ESP.getFreeHeap());
    display->printf("Fs:%d/%d\n",SPIFFS.usedBytes(), SPIFFS.totalBytes());

    display->display();
    return true;
#endif
    
}
