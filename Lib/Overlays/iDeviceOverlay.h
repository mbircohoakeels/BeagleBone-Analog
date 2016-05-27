//
// Created by Michael Brookes on 21/05/2016.
//


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

#ifndef ANALOGDEVICE_OVERLAYS_H
#define ANALOGDEVICE_OVERLAYS_H

#define RETRIES 99

#define SLOTS_DIR "/sys/devices/bone_capemgr.9/slots" //!< Full system path to SLOTS, used to export device tree overlays.

using namespace std;

class iDeviceOverlay {
public:
    iDeviceOverlay( );

    bool IsLoaded( const char* SearchFile );

    bool Load( const char* Overlay );

    int FileHandle;

    char ErrMessage[ 1024 ];

    struct stat sb;

};


#endif //ANALOGDEVICE_OVERLAYS_H
