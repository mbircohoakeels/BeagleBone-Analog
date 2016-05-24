//
// Created by Michael Brookes on 20/05/2016.
//

#ifndef ANALOGDEVICE_ANALOGDEVICE_H
#define ANALOGDEVICE_ANALOGDEVICE_H

#include "../Device/IDevice.h"
#include "../Exceptions/ExceptionAid.h"
#include "../Overlays/Analog/AnalogOverlay.h"

#include <unistd.h>

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


        void InitDevice( ) throw( AnalogSetupException& );

        int WriteToDevice( size_t _BufferSize ) throw( AnalogSetupException& ) { return 1; }

    private:

        AnalogOverlay* _AnalogOverlay;

    protected:

        int ConnectToDevice( ) throw( AnalogSetupException& ) { return 1; }

        int OpenDevice( ) throw( AnalogSetupException& ) { return 1; }

        short ReadDevice( ) throw( AnalogSetupException& );

        bool DeviceInitialised;

        int FileHandle;

    };

}
#endif //ANALOGDEVICE_ANALOGDEVICE_H
