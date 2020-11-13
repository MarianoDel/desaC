//----------------------------------------
// Prueba de lib portaudio
// compilador gcc
//----------------------------------------
#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#include "list_devices.h"
#include "play_sine.h"

// Module Functions Declarations -----------------------------------------------
// typedef int PaStreamCallback( const void *input,
//                               void *output,
//                               unsigned long frameCount,
//                               const PaStreamCallbackTimeInfo* timeInfo,
//                               PaStreamCallbackFlags statusFlags,
//                               void *userData );



// Module Functions Definitions ------------------------------------------------
int main (int argc, char * argv[])
{
    // PrintDevices();
    PlaySine();
    return 0;
}




//--- end of file ---//





