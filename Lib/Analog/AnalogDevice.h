//
// Created by Michael Brookes on 20/05/2016.
//

#ifndef ANALOGDEVICE_ANALOGDEVICE_H
#define ANALOGDEVICE_ANALOGDEVICE_H

#include "../Device/IDevice.h"
#include "../Exceptions/ExceptionAid.h"
#include "../Overlays/Analog/AnalogOverlay.h"

#include <unistd.h>
#include <fstream>
#include <string>

namespace abAnalog {

    using namespace std;

    class AnalogDevice : public abIDevice::IDevice {

    public:
        /**
         \fn Public Destructor
         */
        ~AnalogDevice( );

        /**
         \fn Public Constructor
         */
        AnalogDevice( );

        enum AnalogPort {
            PORT0 = 0, //!< Analog Port 0 (P9, Pin 39)
            PORT1 = 1, //!< Analog Port 1 (P9, Pin 40)
            PORT2 = 2, //!< Analog Port 2 (P9, Pin 37)
            PORT3 = 3, //!< Analog Port 3 (P9, Pin 38)
            PORT4 = 4, //!< Analog Port 4 (P9, Pin 33)
            PORT5 = 5, //!< Analog Port 5 (P9, Pin 36)
            PORT6 = 6  //!< Analog Port 6 (P9, Pin 35)
        };

        short ReadDevice( size_t _BufferSize );

        int GetCurrentReading( );

        void SetDevicePort( AnalogPort _AP );

    private:

        AnalogOverlay _AnalogOverlay;

        AnalogPort DevicePort;

        ifstream DeviceFile;

        string DeviceFilePath;

        string CurrentReading;

        string EmptyString;

        void InitDevice( ) throw( AnalogSetupException& );

        void SetDeviceFilePath( );

        int ConnectToDevice( ) throw( AnalogSetupException& ) { return 1; }

        int WriteToDevice( size_t _BufferSize ) throw( AnalogSetupException& ) { return 1; }

        int OpenDevice( ) throw( AnalogSetupException& );

        int FileHandle;

    };

}
#endif //ANALOGDEVICE_ANALOGDEVICE_H
