//
// Created by Michael Brookes on 26/05/2016.
//

#include "LVMaxSonarEZ.h"


LVMaxSonarEZ::LVMaxSonarEZ( ) {
    this->SetMode( DeviceMode::Cm );
    this->SetStatus( DeviceStatus::Off );
}

void LVMaxSonarEZ::Start( ) {
    this->ThreadRet = pthread_create( &this->SonicReaderThread, NULL, LVMaxSonarEZ::GetValueAsInt, this );

    if( this->ThreadRet == 0 )
        this->SetStatus( DeviceStatus::On );
    else {
        this->SetStatus( DeviceStatus::Error );
        if( this->ThreadRet == EAGAIN )
            this->CurrentError = "Unable to create thread : Resource Limit Reached.";
        else
            this->CurrentError = "Unable to create thread : Unknown Error Occurred.";
    }
}

void LVMaxSonarEZ::Stop( ) {
    this->ThreadRet = pthread_cancel( this->SonicReaderThread );

    if( this->ThreadRet == 0 )
        this->SetStatus( DeviceStatus::Off );
    else {
        this->SetStatus( DeviceStatus::Error );
        if( this->ThreadRet == ESRCH )
            this->CurrentError = "Unable to cancel thread : Unable to locate process.";
        else
            this->CurrentError = "Unable to cancel thread : Unknown Error Occurred.";
    }

}

void *LVMaxSonarEZ::GetValueAsInt( void *static_inst ) {
    LVMaxSonarEZ* SonicInst = (LVMaxSonarEZ*)static_inst;
    while( SonicInst->CurrentStatus == DeviceStatus::On ) {
        SonicInst->Reading = SonicInst->ConvertReadingToDistance( SonicInst->GetReading( ) );
        SonicInst->AddToDataStore( );
        usleep( SONIC_DATATIMER );
    }
}

int LVMaxSonarEZ::GetReading( ) {
    return this->GetCurrentReading( );
}

double LVMaxSonarEZ::ConvertReadingToDistance( int _reading ) {
    switch( this->CurrentMode ) {
        case DeviceMode::Inches :
            return _reading / SONIC_INCH_MODIFYER;
        case DeviceMode::Cm :
            return ( _reading / SONIC_INCH_MODIFYER ) * SONIC_CM_MODIFYER;
        default:
            return _reading / SONIC_INCH_MODIFYER;
    }
}

void LVMaxSonarEZ::SetAverage( int iterations = 50 ) {
    double total_reading = 0;
    int i = 0;
    while (i < iterations) {
        total_reading += this->GetReading( );
        usleep( this->DataTimer );
        i++;
    }
    this->AvgReading = total_reading / iterations;
}

void LVMaxSonarEZ::AddToDataStore( ) {
    for( int i = 0; i < MAX_DATASTORE; i++ )
        this->DataStoredValues[ i ] = this->DataStoredValues[ i + 1 ];

    this->DataStoredValues[ MAX_DATASTORE -1 ] = this->Reading;
}

void LVMaxSonarEZ::SetMode( DeviceMode _mode ) {
    this->CurrentMode = _mode;
}

void LVMaxSonarEZ::SetStatus( DeviceStatus _status ) {
    this->CurrentStatus = _status;
}

double LVMaxSonarEZ::Distance( ) {
    return this->Reading;
}

double LVMaxSonarEZ::AvgDistance( ) {
    this->SetAverage( 50 );
    return this->AvgReading;
}