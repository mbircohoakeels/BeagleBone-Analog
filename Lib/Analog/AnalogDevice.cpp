//
// Created by Michael Brookes on 20/05/2016.
//

#include <vector>
#include "AnalogDevice.h"

namespace abAnalog{

    AnalogDevice::AnalogDevice( ) {

        this->SetDevicePort( AnalogPort::PORT0 );
        this->InitDevice( );
        if( this->_AnalogOverlay.Settings.OverlayLoaded ) {
            this->SetDeviceFilePath( );
        }
        else {
            snprintf( this->_AnalogOverlay.ErrMessage, sizeof( this->_AnalogOverlay.ErrMessage ),
                      "Fatal Analog Error - Unable to initialise the Analog Device." );
            throw AnalogSetupException( this->_AnalogOverlay.ErrMessage );
        }
    }

    AnalogDevice::~AnalogDevice( ) { }

    void AnalogDevice::InitDevice( ) throw( AnalogSetupException& ) {
        if( !this->_AnalogOverlay.Settings.OverlayLoaded ) {
            snprintf( this->_AnalogOverlay.ErrMessage, sizeof( this->_AnalogOverlay.ErrMessage ),
                      "Fatal Analog Error - Unable to load overlay : %s",
                      this->_AnalogOverlay.Settings.Overlay );
            throw AnalogSetupException( this->_AnalogOverlay.ErrMessage );
        }
    }

    int AnalogDevice::GetCurrentReading( ) {
        this->ReadDevice( 1024 );
        if( this->CurrentReading.length() > 0 ){
            return stoi( this->CurrentReading );
        }
        else {
            return 1;
        }
    }

    short AnalogDevice::ReadDevice( size_t _BufferSize ) {
        if( !this->DeviceFile.is_open( ) ) {
            if( this->OpenDevice( )) {
                try {
                    getline( this->DeviceFile, this->CurrentReading );
                    this->DeviceFile.close( );
                }
                catch( ifstream::failure e ) {
                    cout << "Exception caught handling binary file : " << e.what( ) << endl;
                }
            }
            return 0;
        }
    }

    int AnalogDevice::OpenDevice( ) throw( AnalogSetupException& ) {
        try {
            this->DeviceFile.open( this->DeviceFilePath.c_str( ), ios::binary | ios::in );
            return 1;
        } catch ( exception& e ) {
            snprintf( this->_AnalogOverlay.ErrMessage, sizeof( this->_AnalogOverlay.ErrMessage ),
                      "Fatal Analog Error - Unable to create FileHandle : %s",
                      this->DeviceFilePath.c_str( ) );
            throw AnalogSetupException( this->_AnalogOverlay.ErrMessage );
        }
    }

    void AnalogDevice::SetDevicePort( AnalogPort DevicePort ) {
        this->DevicePort = DevicePort;
    }

    void AnalogDevice::SetDeviceFilePath( ) {
        ostringstream oss;
        this->DeviceFilePath = this->_AnalogOverlay.Settings.HelperPath;
        this->DeviceFilePath.append( this->_AnalogOverlay.Settings.FileType );
        oss << this->DevicePort;
        this->DeviceFilePath.append( oss.str( ) );
    }

}