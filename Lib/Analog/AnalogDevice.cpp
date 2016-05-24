//
// Created by Michael Brookes on 20/05/2016.
//

#include "AnalogDevice.h"

namespace abAnalog{

    AnalogDevice::AnalogDevice( ) {
        _AnalogOverlay = new AnalogOverlay( );
    }

    AnalogDevice::~AnalogDevice( ) { }

    void AnalogDevice::InitDevice( ) throw( AnalogSetupException& ) {
        if( _AnalogOverlay->Settings.OverlayLoaded )
            this->DeviceInitialised = true;
        else {
            snprintf( _AnalogOverlay->ErrMessage, sizeof( _AnalogOverlay->ErrMessage ),
                      "Fatal Analog Error - Unable to load overlay : %s",
                      _AnalogOverlay->Settings.Overlay );
            throw AnalogSetupException( _AnalogOverlay->ErrMessage );
        }

    }

    short AnalogDevice::ReadDevice( ) throw( AnalogSetupException& ) {
        unsigned char buff[ 1024 ];
        this->FileHandle = open( _AnalogOverlay->Settings.HelperPath, O_WRONLY );
        if( read( this->FileHandle, buff, sizeof( buff ) ) != 0 ) {

        }
    }

    int AnalogDevice::OpenDevice( ) throw( AnalogSetupException& ) {

    }

}