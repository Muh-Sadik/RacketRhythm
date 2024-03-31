#include "contact_detector.h"  
#include "ball_contact_count.h"
#include "tempo.h"  
#include <iostream>
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define NUM_CHANNELS 2  // 2 for stereo input //
#define FRAMES_PER_BUFFER 512

int main() {
    // Initializing PortAudio librarysudo
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio initialization failed: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // Identify streaming device parameters
    PaStreamParameters inputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice(); // default input device //
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        return 1;
    }
    inputParameters.channelCount = NUM_CHANNELS;      
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    // Open audio input stream
    PaStream *stream;
    err = Pa_OpenStream(&stream,
                        &inputParameters, // Input parameters
                        NULL,             // The number of output channels (Null for input-only streams).
                        SAMPLE_RATE, 
                        FRAMES_PER_BUFFER, 
                        0, // paClipOff
                        contactdetector::audioCallback,
                        NULL);

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
    // Wait for user input to stop the program
    std::cout << "Press Enter to stop..." << std::endl;
    std::cin.get();

    // Stop and close the audio stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio stream stopping failed: " << Pa_GetErrorText(err) << std::endl;
    }
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio stream closing failed: " << Pa_GetErrorText(err) << std::endl;
    }

    // Terminate PortAudio
    Pa_Terminate();

    return 0;
}