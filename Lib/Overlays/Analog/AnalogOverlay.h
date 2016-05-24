//
// Created by Michael Brookes on 22/05/2016.
//

#ifndef ANALOGDEVICE_ANALOGOVERLAY_H
#define ANALOGDEVICE_ANALOGOVERLAY_H

#define MAX_BUF 1024

#include "../iDeviceOverlay.h"
#include "../../Exceptions/ExceptionAid.h"

class AnalogOverlay: public iDeviceOverlay {

public:
    AnalogOverlay( );

    static struct OverlaySettings {
        const char *Overlay = "cape-bone-iio";
        const char *SearchFile = "AIN0";
        const char *HelperParentDir = "/sys/devices/ocp.3/";
        char *HelperPath;
        bool OverlayLoaded = false;
    } Settings;

private:

    void FindHelperDir( );

    char AnalogFilePath[MAX_BUF];

};


#endif //ANALOGDEVICE_ANALOGOVERLAY_H
