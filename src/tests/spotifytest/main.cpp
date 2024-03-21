#include "ALSACapture.h"
#include <iostream>
 

int main() {
    // Specify the device file paths for capture and playback
    std::string captureDevice = "hw:2,0"; // Replace with the appropriate capture device file
    std::string playbackDevice = "hw:2,0"; // Replace with the appropriate playback device identifier
 
    // Create an instance of ALSACapture with the specified device files
    ALSACapture capture(captureDevice, playbackDevice);

    // Initialize ALSA
    capture.init();

    // Start capturing and playing back audio
    capture.captureAndPlaybackLoop();

    return 0;
}
