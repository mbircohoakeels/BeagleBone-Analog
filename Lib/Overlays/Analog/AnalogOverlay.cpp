//
// Created by Michael Brookes on 22/05/2016.
//

#include "AnalogOverlay.h"

namespace abAnalog {

    AnalogOverlay::AnalogOverlay( ) throw( AnalogSetupException& ) {
        this->FindHelperDir( );
        if( !this->Settings.OverlayLoaded )
            this->Settings.OverlayLoaded = this->Load( this->Settings.Overlay );
        if( !this->Settings.OverlayLoaded ) {
            snprintf( this->ErrMessage, sizeof( this->ErrMessage ),
                      "Fatal Analog Error - Unable to load overlay : %s",
                      this->Settings.Overlay );
            throw AnalogSetupException( this->ErrMessage );
        }
    }

    void AnalogOverlay::FindHelperDir( ) {
        for( int i = 1; i <= RETRIES; i++ ) {
            snprintf( this->AnalogFilePath,
                      sizeof( this->AnalogFilePath ),
                      "%shelper.%d/%s",
                      this->Settings.HelperParentDir,
                      i,
                      this->Settings.SearchFile );

            if( this->IsLoaded( this->AnalogFilePath )) {
                this->Settings.OverlayLoaded = true;

                snprintf( this->Settings.HelperPath,
                          sizeof( this->Settings.HelperPath ),
                          "%shelper.%d/",
                          this->Settings.HelperParentDir,
                          i );
                break;
            }
            else if( i == RETRIES )
                this->Settings.OverlayLoaded = false;
        }
    }

}