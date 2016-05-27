//
// Created by Michael Brookes on 21/05/2016.
//

#include "iDeviceOverlay.h"

iDeviceOverlay::iDeviceOverlay( ) {
}

bool iDeviceOverlay::IsLoaded( const char* SearchFile ) {
    if( stat( SearchFile, &sb ) == 0 && S_ISREG( sb.st_mode ) )
        return true;
    return false;
}

bool iDeviceOverlay::Load( const char* Overlay ) {
    try {
        this->FileHandle = open( SLOTS_DIR, O_WRONLY );
        if( write( this->FileHandle, Overlay, 1024 ) > 0 ) {
            close( this->FileHandle );
            return true;
        }
        else
            return false;
    } catch( exception& e ) {
        cerr << "Exception : " << e.what() << endl;
    }
}