#include "config.h"
#include <SPIFFS.h>

bool ConfigurationFile::get(const char * filename, const String &key, String &value) {
    if (!SPIFFS.begin()) {
        return false;
    }
    if(! SPIFFS.exists(filename) ) {
        return false;
    }
    String searchKey = key;
    if ( !searchKey.endsWith(":") ) {
        searchKey += ":";
    }
    File f = SPIFFS.open(filename, "r");
    if ( !f) {
        return false;
    }
    while(f.available()) {
        String s = f.readStringUntil('\n');
        if (s.startsWith(searchKey) ) {
            s = s.substring(searchKey.length());
            if(s.endsWith("\n")) {
                s = s.substring(0,s.length()-1);
            }
            value = s;
            f.close();
            return true;
        }
    }
    f.close();
    return false;
}