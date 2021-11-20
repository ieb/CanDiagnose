#ifndef _CONFIG_H
#define _CONFIG_H

#include <WString.h>

class ConfigurationFile {
    public:
        static bool get(const char * filename, const String &key, String &value);
};


#endif