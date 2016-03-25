//
//  BBBAnalogDevice.cpp
//  BBBAnalogDevice
//
//  Created by Michael Brookes on 04/10/2015.
//  Copyright © 2015 Michael Brookes. All rights reserved.
//

#include "BBBAnalogDevice.h"

/**
 \brief BBBAnalogDevice : A low level control of analog devices on the Beaglebone Black.
 \param <void>
 */
BBBAnalogDevice::BBBAnalogDevice( ){

}

/**
 \fn private function int Analog_PinCheck( void )
 \brief Checks that the Analog Pin files are available for Analog operation on the BeagleBone Black.
 \param <void>
 \return <int> 99 failure load the Analog Pin files, < 99 success.
 */
int BBBAnalogDevice::Analog_PinCheck( void ) {
    int i = 0;
    while( i < RETRIES ) {
        i++;
        snprintf( this->Analog_PinOverlayFileName, sizeof( this->Analog_PinOverlayFileName ), "/sys/devices/ocp.3/helper.%d/AIN%d", i, this->Analog_GetPort( ) );
        if ( stat( this->Analog_PinOverlayFileName, &sb ) == 0 && S_ISREG( sb.st_mode ) )
            break;
    }
    return i;
}

/**
 \fn private function int Analog_LoadOverlay( const char* Analog_OverlayFile )
 \brief The BeagleBone Black has Overlay Files to allow operations like Analog Communication. In this function we are attempting to export an overlay.
 \param <const char*> Analog_OverlayFile (just the file name, no path required)
 \return <int> -1 failure load the system files, 1 success.
 */
int BBBAnalogDevice::
Analog_LoadOverlay( const char* Analog_OverlayFile ) {
    if( this->Analog_SetFileHandle( SLOTS_DIR ) < 0 )
        return this->Analog_FileHandle;
    else {
        if( this->Analog_WriteToFile( this->Analog_Buffer, snprintf( this->Analog_Buffer, sizeof( this->Analog_Buffer ), "%s", Analog_OverlayFile ) ) > 0 ) {
            close( this->Analog_FileHandle );
            return 1;
        }
        else
            return -1;
    }
}

/**
 \fn private function int Analog_SetFileHandle( const char* Analog_FileName )
 \brief Creates a private FileHandle for use within this class.
 \param <const char*> Analog_FileName
 \throws Exception on failure to open the file.
 \return <int> -1 failure load the system files, > 0 success.
 */
int BBBAnalogDevice::Analog_SetFileHandle( const char* Analog_FileName ) {
    try {
        this->Analog_FileHandle = open( Analog_FileName, O_WRONLY );
    } catch( exception& e ) {
        cerr << "Error opening file : " << Analog_FileName << " | Error = " << e.what( ) << endl;
        this->Analog_FileHandle = -1;
    }
    return this->Analog_FileHandle;
}

/**
 \fn private function int Analog_WriteToFile( const char* Analog_Buffer, int Analog_BufferLen )
 \brief Writes a value to a file.
 \param <const char*> Analog_Buffer (holds the value to be written)
 \param <int>Analog_BufferLen (length of the buffer to be written)
 \throws Exception on failure to write.
 \return <int> -1 exception occured, 0 failed to write, 1 success.
 */
int BBBAnalogDevice::Analog_WriteToFile( const char* Analog_Buffer, int Analog_BufferLen ) {
    try {
        if( write( this->Analog_FileHandle, Analog_Buffer, Analog_BufferLen ) > 0 ) {
            close( this->Analog_FileHandle );
            return 1;
        }
        else
            return 0;
    } catch( exception& e ) {
        cerr << "Error writing to file. | Error = " << e.what( ) << endl;
        return -1;
    }
}

/**
 \fn Private function Analog_SetPort( AnalogPort PortNum )
 \brief Sets the Analog Port that we will use on the BBB.
 \param <void>
 \throws exception : Unable to assign the analog port.
 \return <int> -1 failure to open the bus, 0 success.
 */
int BBBAnalogDevice::Analog_SetFileLocationByPort( void ) {
    try {
        this->AnalogFileLocation = this->Analog_PinOverlayFileName;
        return 0;
    } catch ( exception& e ) {
        cerr << "Unable to set the analog file location : " << e.what( ) << endl;
        return -1;
    }
}

/**
 \fn Public function Analog_SetPort( AnalogPort PortNum )
 \brief Sets the Analog Port that we will use on the BBB.
 \param <AnalogPort> PortNum
 \throws exception : Unable to assign the analog port.
 \return <int> -1 failure to open the bus, 0 success.
 */
int BBBAnalogDevice::Analog_SetPort( AnalogPort PortNum ) {
    try {
        this->AnalogPortNum = PortNum;
        this->Analog_SetFileLocationByPort( );
        return 0;
    } catch ( exception& e ) {
        cerr << "Unable to assign the analog port : " << e.what( ) << endl;
        return -1;
    }
}

/**
 \fn Public function Analog_GetPort( void ) const
 \brief Gets the Analog Port that we will use on the BBB.
 \param <void>
 \return <AnalogPort> this->AnalogPortNum
 */
BBBAnalogDevice::AnalogPort BBBAnalogDevice::Analog_GetPort( void ) const {
    return this->AnalogPortNum;
}


/**
 \fn Public function Analog_GetFileLocation( void ) const
 \brief Gets the Analog Port File Location that we will use on the BBB.
 \param <void>
 \return <string> this->AnalogFileLocation
 */
string BBBAnalogDevice::Analog_GetFileLocation( void ) const {
    return this->AnalogFileLocation;
}

void BBBAnalogDevice::Start( ) {
    if( this->Analog_PinCheck( ) >= RETRIES ) {
        snprintf( this->Analog_PinOverlay, sizeof( this->Analog_PinOverlay ), "cape-bone-iio" );
        if( this->Analog_LoadOverlay( this->Analog_PinOverlay ) < 0 ) {
            cerr << "Critical Error 1 : Unable to setup Analog communication on your BeagleBone Black, sys error - unable to export :" << this->Analog_PinOverlay << endl;
            exit( 1 );
        }
    }
    else
        this->ThreadRet = pthread_create( &this->AnalogNotifyer, NULL, BBBAnalogDevice::Analog_GetValueAsInt, this );

    //if( this->Analog_PinCheck( ) >= RETRIES ) {
    //    cerr << "Critical Error 2 : Unable to setup Analog communication on your BeagleBone Black, sys error - unable to export :" << this->Analog_PinOverlay << endl;
    //    exit( 1 );
    //}
}

/**
 \fn Public static function Analog_GetValueAsInt( this )
 \brief Analog_GetValueAsInt - Reads the value from the analog file and assigns it to this->AnalogReading.
 \param <this>
 \throws Exception on unable to open Analog File.
 \return <int>
 */
void* BBBAnalogDevice::Analog_GetValueAsInt( void *analog_inst ) {
    BBBAnalogDevice* _analog = (BBBAnalogDevice*)analog_inst;
    while( 1 ) {
        _analog->buffer = 0;
        try {
            //_analog->aFile = fopen( _analog->AnalogFileLocation.c_str(), "rb");
            _analog->aFile = fopen( _analog->Analog_PinOverlayFileName, "rb");
            if (_analog->aFile == NULL){
                cerr << "1. Unable to open Analog File : " << _analog->Analog_PinOverlayFileName << endl;
                exit(1);
            }
        } catch ( exception& e ) {
            cerr << "2. Unable to open Analog File : " << _analog->Analog_PinOverlayFileName << e.what() << endl;
        }

        fseek(_analog->aFile, 0, SEEK_END);

        _analog->lSize = ftell(_analog->aFile);

        rewind(_analog->aFile);

        _analog->buffer = (char*)malloc(_analog->lSize);

        if(_analog->buffer == NULL) {fputs("Memory error", stderr); exit(2);}

        fread(_analog->buffer, 1, _analog->lSize, _analog->aFile);

        fclose(_analog->aFile);

        _analog->AnalogValue = atoi(_analog->buffer);

        _analog->AddToAnalogDataStore( );

        free(_analog->buffer);

        usleep( 5000 );

    }

    return 0;
}

void BBBAnalogDevice::AddToAnalogDataStore( ) {
    for( int i = 0; i < MAX_DATASTORE; i++ )
        this->DataStoredValues[ i ] = this->DataStoredValues[ i + 1 ];

    this->DataStoredValues[ MAX_DATASTORE ] = this->ConvertAnalogReadingToAltitude( );
}

int BBBAnalogDevice::ConvertAnalogReadingToAltitude( ) {
    return this->GetCurrentReading( ) / SONIC_MODIFYER;
}

int BBBAnalogDevice::GetCurrentReading( ) {
    return this->AnalogValue;
}

void BBBAnalogDevice::SetAverages( int iterations = 50 ) {
    int i = 0, total_reading = 0;
    while (i < iterations) {
        total_reading += GetCurrentReading( );
        usleep( 5000 );
        i++;
    }
    this->avgReading = total_reading / iterations;
}

