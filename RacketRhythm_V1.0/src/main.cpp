#include "contact_detector.h"  
#include "ball_contact_count.h"
#include "tempo.h"  
#include "ALSACapture.h"
#include <iostream>         // Used for input/output stream commands.
#include <portaudio.h>      // Used for audio input/output using PortAudio.
#include <alsa/asoundlib.h> // Used for audio input/output and sound card control using ALSA (Advanced Linux Sound Architecture).
#include <thread>           // Used for threading functions.

#define SAMPLE_RATE 48000 // Replace with the appropriate value for your device.
#define NUM_CHANNELS 2    // 2 for stereo input //
#define FRAMES_PER_BUFFER 512

// Function to run ball contact counting and tempo-related operations
void contactcounting_and_tempo() {

    BallContactCount count;          // Create an object of the BallContactCount class
    count.processBallContact();      // Call processBallContact

    // Declare a variable to hold elapsed time
    std::chrono::steady_clock::duration elapsedTime;

    tempo tempo;                     // Create an object of the tempo class
    tempo.playingTempo(elapsedTime); // Call playingTempo, and pass elapsedTime
}

// Function to run ALSA capture and playback
void runALSACapture(const std::string& captureDevice, const std::string& playbackDevice) {
    ALSACapture musicCapture(captureDevice, playbackDevice);
    musicCapture.init();
    musicCapture.captureAndPlaybackLoop();
}

int main() {
    // Initialize PortAudio library
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio initialization failed: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // Specify the ALSA PCM device name
    const char *alsaDevice = "hw:1,0"; // Replace hw:1,0 with the appropriate card and device within your system

    // Get the total number of available audio devices
    int numDevices = Pa_GetDeviceCount();

    // Check for errors
    if (numDevices < 0) {
        std::cerr << "Error getting device count: " << Pa_GetErrorText(numDevices) << std::endl;
        Pa_Terminate();
        return 1;
    }

    // Create a variable to store the ID of the desired alsaDevice device
    int desiredDeviceID = -1;
    for (int i = 0; i < numDevices; ++i) {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo && std::string(deviceInfo->name).find(alsaDevice) != std::string::npos) {
            desiredDeviceID = i;         // set desiredDeviceID to the ID of the desired alsaDevice device
            break;
        }
    }

    // Check for errors finding the device
    if (desiredDeviceID == -1) {
        std::cerr << "Desired device not found." << std::endl;
        Pa_Terminate();
        return 1;
    }

    // Identify streaming device input parameters
    PaStreamParameters inputParameters;
    inputParameters.device = desiredDeviceID;
    inputParameters.channelCount = NUM_CHANNELS;      
    inputParameters.sampleFormat = paFloat32;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    // Open audio input stream with the specified device
    PaStream *stream;
    err = Pa_OpenStream(&stream,
                        &inputParameters,  // Input parameters
                        NULL,              // The number of output channels (Null for input-only streams).
                        SAMPLE_RATE, 
                        FRAMES_PER_BUFFER, 
                        0,                 // paClipOff
                        contactdetector::audioCallback,
                        NULL);             // no callback, so no callback userData 
    
    // Check if there was an error opening the PortAudio stream
    if (err != paNoError) {
        std::cerr << "PortAudio stream opening failed: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return 1;
    }

    // Start audio stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio stream starting failed: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }

    // Run ballcontactcount and tempo in a different thread
    std::thread ballcounting(contactcounting_and_tempo);

    // Specify the device file paths for capture and playback
    std::string captureDevice = "hw:2,0";  // Replace with the appropriate capture device file within your system
    std::string playbackDevice = "hw:2,0"; // Replace with the appropriate playback device identifier within your system

    // Start capturing and playing back audio in a separate thread
    std::thread musicCaptureThread(runALSACapture, captureDevice, playbackDevice);

    // Join the threads
    ballcounting.join();
    musicCaptureThread.join();
   
/*  // Wait for user input to stop the program
    std::cout << "Press Enter to stop..." << std::endl;
    std::cin.get();
*/ // debugging statement 

    // Stop the audio stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio stream stopping failed: " << Pa_GetErrorText(err) << std::endl;
    }
    // close the audio stream
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio stream closing failed: " << Pa_GetErrorText(err) << std::endl;
    }

    // Terminate PortAudio
    Pa_Terminate();

    return 0;
    
} /* main.cpp */