//
// Created by Michael Brookes on 22/05/2016.
//

#ifndef ANALOGDEVICE_ANALOGOVERLAY_H
#define ANALOGDEVICE_ANALOGOVERLAY_H

#define MAX_BUF 1024

#include "../iDeviceOverlay.h"
#include "../../Exceptions/ExceptionAid.h"

namespace abAnalog {

    class AnalogOverlay : public iDeviceOverlay {

    public:
        AnalogOverlay( ) throw( AnalogSetupException& );

        struct OverlaySettings {
            const char *Overlay = "cape-bone-iio";
            const char *SearchFile = "AIN0";
            const char *FileType = "AIN";
            const char *HelperParentDir = "/sys/devices/ocp.3/";
            char HelperPath[ MAX_BUF ];
            bool OverlayLoaded;
        } Settings;

        char AnalogFilePath[ MAX_BUF ];

    private:

        void FindHelperDir( );

    };

}
#endif //ANALOGDEVICE_ANALOGOVERLAY_H
