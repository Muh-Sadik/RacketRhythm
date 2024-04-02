#include "ALSACapture.h"
#include <iostream>
#include <thread>
 
 void runALSACapture(const std::string& captureDevice, const std::string& playbackDevice) {
    ALSACapture musicCapture(captureDevice, playbackDevice);
    musicCapture.init();
    musicCapture.captureAndPlaybackLoop();
}

int main(){
    
    // Specify the device file paths for capture and playback
    std::string captureDevice = "hw:1,0"; // Replace with the appropriate capture device file
    std::string playbackDevice = "hw:1,0"; // Replace with the appropriate playback device identifier
 
    // Create an instance of ALSACapture with the specified device files
    ALSACapture capture(captureDevice, playbackDevice);

    // Initialize ALSA
    capture.init();

    // Start capturing and playing back audio
    capture.captureAndPlaybackLoop();

    std::thread musicCaptureThread(runALSACapture, captureDevice, playbackDevice);

    musicCaptureThread.join();

    return 0;
}