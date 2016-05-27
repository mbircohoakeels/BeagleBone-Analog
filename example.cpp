#include <iostream>

using namespace std;

#include "Lib/LVMaxSonarEZ/LVMaxSonarEZ.h"

int main( ) {
    using namespace abAnalog;

    LVMaxSonarEZ SonicSensor;
    SonicSensor.Start( );
    usleep( 1000000 );
    SonicSensor.SetMode( LVMaxSonarEZ::DeviceMode::Cm );

    while( SonicSensor.CurrentStatus == LVMaxSonarEZ::DeviceStatus::On ) {
        SonicSensor.SetMode( LVMaxSonarEZ::DeviceMode::Inches );
        cout << "Current Distance In Inches : " << SonicSensor.Distance( ) << endl;
        SonicSensor.SetMode( LVMaxSonarEZ::DeviceMode::Cm );
        cout << "Current Distance In Cm : " << SonicSensor.Distance( ) << endl;
        usleep( SonicSensor.DataTimer );
    }
    while( SonicSensor.CurrentStatus == LVMaxSonarEZ::DeviceStatus::Error ) {
        cout << "Error : " << SonicSensor.CurrentError << " | Status = " << SonicSensor.CurrentStatus << " | Mode = " << SonicSensor.CurrentMode << endl;
        usleep( 1000000 );
    }
}