/*
Copyright (C) 2016 Michael Brookes

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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