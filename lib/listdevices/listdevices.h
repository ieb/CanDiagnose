#ifndef LISTDEVICES_H
#define LISTDEVICES_H

#include <NMEA2000.h>
#include <N2kMessages.h>
#include <N2kDeviceList.h>


class ListDevices: public tN2kDeviceList {
    public:
        ListDevices(tNMEA2000 *_pNMEA2000, Stream *outputStream);
        void list(bool force = false);

    private:
        Stream *OutputStream;
        void printUlongList(const char *prefix, const unsigned long * List);
        void printText(const char *Text, bool AddLineFeed=true);
        void printDevice(const tNMEA2000::tDevice *pDevice);

};

#endif