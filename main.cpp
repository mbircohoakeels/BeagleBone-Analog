//
// Created by Michael Brookes on 19/02/2016.
//

#include "BBBAnalogDevice.h"

using namespace std;

int main ( void ) {

    BBBAnalogDevice QBBBA;
    QBBBA.Analog_SetPort( QBBBA.AIN0 );
    QBBBA.Start( );
    QBBBA.Analog_SetType( QBBBA.SONIC );
    cout << "Device Thread Started" << endl;
    usleep(10000);

    while(1){
        cout << "Sensor Reading = " << QBBBA.GetCurrentReading() << endl;
        for( int i = 0; i < MAX_DATASTORE; ++i ) {
            cout << "Altitude Stored Val " << i << " = " << QBBBA.DataStoredValues[ i ] << endl;
        }
        usleep(100000);
    }
}