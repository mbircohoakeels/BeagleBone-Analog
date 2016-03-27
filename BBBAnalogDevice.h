//  BBBAnalogDevice.h
//  BBBAnalogDevice
//
//  Created by Michael Brookes on 04/10/2015.
//  Copyright © 2015 Michael Brookes. All rights reserved.
//

#ifndef BBBAnalogDevice_h
#define BBBAnalogDevice_h

#define ANALOG_LOC            "/sys/devices/ocp.3/" //!< File Path for all the Analog Files.
#define SLOTS_DIR            "/sys/devices/bone_capemgr.9/slots" //!< Full system path to SLOTS, used to export device tree overlays.
#define RETRIES            100 //!< Analog system files have an index appended to the end of the folder name. normally 1 - 99, RETRIES is used to find that index.
#define MAX_BUF            1024 //!< Used in setting the buffer size.
#define MAX_DATASTORE        15 //!< How many iterations to store.
#define SONIC_MODIFYER        9.8

#include <iostream>
#include <exception>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

using namespace std;

/*!
 *  \brief     BBBAnalogDevice provides low level access to the Analog files on the BeagleBone Black.
 *  \author    Michael Brookes
 *  \version   1.1
 *  \date      Oct-2015
 *  \copyright GNU Public License.
 */
class BBBAnalogDevice {

public:

    /**
     \brief Please refer to the documention provided for your BeagleBone Black to ascertain which Analog Port to use.
     */
    enum AnalogPort {
        AIN0 = 0, //!< Analog Port 0 (P9, Pin 39)
                AIN1 = 1, //!< Analog Port 1 (P9, Pin 40)
                AIN2 = 2, //!< Analog Port 2 (P9, Pin 37)
                AIN3 = 3, //!< Analog Port 3 (P9, Pin 38)
                AIN4 = 4, //!< Analog Port 4 (P9, Pin 33)
                AIN5 = 5, //!< Analog Port 5 (P9, Pin 36)
                AIN6 = 6,//!< Analog Port 6 (P9, Pin 35)
    };

    enum AnalogType {
        SONIC = 0,
    };

    /**
	 \fn Public function Analog_GetPort( void ) const
	 \brief Gets the Analog Port that we will use on the BBB.
	 \return <AnalogPort> this->AnalogPortNum
	 */
    AnalogPort Analog_GetPort( void ) const;

    /**
     \fn Public function Analog_GetFileLocation( void ) const
     \brief Gets the Analog Port File Location that we will use on the BBB.
     \return <string> this->AnalogFileLocation
     */
    string Analog_GetFileLocation( void ) const;

    /**
     \fn Public function Analog_SetPort( AnalogPort PortNum )
     \brief Sets the Analog Port that we will use on the BBB.
     \param <AnalogPort> PortNum
     \throws exception : Unable to assign the analog port.
     \return <int> -1 failure to open the bus, 0 success.
     */
    int Analog_SetPort( AnalogPort PortNum );

    /**
     \fn Public function Analog_SetType( AnalogType AType )
     \brief Sets the Analog Sensor Type for reference.
     \param <AnalogType> AType
     \return <void>
     */
    void Analog_SetType( AnalogType AType );

    /**
     \fn Public static function Analog_GetValueAsInt( analog_inst )
     \brief Gets the value from the Analog Port as an integer and assigns it to this->AnalogReading
     \param <void>
     \return <int>
     */
    static void *Analog_GetValueAsInt( void *analog_inst );

    void Start( );

    int GetCurrentReading( );

    void SetAverages( int iterations );

    int DataStoredValues[MAX_DATASTORE];

    int avgReading;

    /**
     \brief BBBAnalogDevice : A low level control of analog devices on the Beaglebone Black.
     \param <void>
     */
    BBBAnalogDevice( );

protected:

    int Analog_FileHandle; //!< Stores the Analog Devices File Handle
    int AnalogValue; //!< AnalogValue contains the value read from the device.
    int ThreadRet; //!< ThreadRet holds the value returned when starting a thread.

    FILE *aFile;
    long lSize;
    char *buffer;

    AnalogPort AnalogPortNum; //!< AnalogPortNum must be a member of the AnalogPort enum.
    AnalogType DeviceType;
    ostringstream AnalogPortNumStr; //!< AnalogPortNumStr converts the port to a string to append to the file location.

    string AnalogFileLocation; //!< AnalogFileLocation contains the full path to the analog file location.

    pthread_t AnalogNotifyer;

    char Analog_Buffer[MAX_BUF]; //!< Used to write to files.
    char Analog_PinOverlayFileName[MAX_BUF]; //!< Stores the Analog folder name
    char Analog_PinOverlay[MAX_BUF]; //!< Stores the Analog device tree overlay name.

    void AddToAnalogDataStore( ); //!< Adds to an array of the last X readings from the device.

    int ConvertAnalogReadingToAltitude( );

    struct stat sb; //!< Used to discover if a folder or file exists already.
    /**
     \fn private function int Analog_PinCheck( void )
     \brief Checks that the Analog Pin files are available for Analog operation on the BeagleBone Black.
     \param <void>
     \return <int> 99 failure load the Analog Pin files, < 99 success.
     */
    int Analog_PinCheck( void );

    /**
     \fn private function int Analog_LoadOverlay( const char* Analog_OverlayFile )
     \brief The BeagleBone Black has Overlay Files to allow operations like Analog Communication. In this function we are attempting to export an overlay.
     \param const <char>* Analog_OverlayFile (just the file name, no path required)
     \return <int> -1 failure load the system files, 1 success.
     */
    int Analog_LoadOverlay( const char *Analog_OverlayFile );

    /**
     \fn private function int Analog_SetFileHandle( const char* Analog_FileName )
     \brief Creates a private FileHandle for use within this class.
     \param const <char>* Analog_FileName
     \throws Exception on failure to open the file.
     \return <int> -1 failure load the system files, > 0 success.
     */
    int Analog_SetFileHandle( const char *Analog_FileName );

    /**
     \fn private function int Analog_WriteToFile( const char* Analog_Buffer, int Analog_BufferLen )
     \brief Writes a value to a file.
     \param const <char>* Analog_Buffer (holds the value to be written)
     \param <int>Analog_BufferLen (length of the buffer to be written)
     \throws Exception on failure to write.
     \return <int> -1 exception occured, 0 failed to write, 1 success.
     */
    int Analog_WriteToFile( const char *Analog_Buffer, int Analog_BufferLen );

    /**
	 \fn Private function Analog_SetFileLocationByPort( void )
	 \brief Sets the File Location for this device from the Analog Port.
	 \param <void>
	 \return <int> -1 failure to open the bus, 0 success.
	 */
    int Analog_SetFileLocationByPort( void );
};

#endif /* BBBAnalogDevice_h */

