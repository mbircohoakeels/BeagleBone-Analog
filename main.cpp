//
// Created by Michael Brookes on 19/02/2016.
//

#include "BBBAnalogDevice.h"

using namespace std;

int main ( void ) {

    BBBAnalogDevice QBBBA;
    QBBBA.Analog_SetPort( QBBBA.AIN0 );
    QBBBA.Start( );
    cout << "Device Thread Started" << endl;
    usleep(10000);

    while(1){
        cout << "Sensor Reading = " << QBBBA.GetCurrentReading() << endl;
        //usleep(100000);
    }
}