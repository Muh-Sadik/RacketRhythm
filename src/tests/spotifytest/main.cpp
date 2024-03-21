#include "ALSACapture.h"
#include <iostream>
 

int main() {
    // Specify the device file paths for capture and playback
    std::string captureDevice = "/dev/snd/pcmC2D0c"; // Replace with the appropriate capture device file
    std::string playbackDevice = "/dev/snd/pcmC1D0p"; // Replace with the appropriate playback device identifier
 
    // Create an instance of ALSACapture with the specified device files
    ALSACapture capture(captureDevice, playbackDevice);

    // Initialize ALSA
    capture.init();

    // Start capturing and playing back audio
    capture.captureAndPlaybackLoop();

    return 0;
}
