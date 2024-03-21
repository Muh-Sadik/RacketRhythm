#include "CONTACT_DETECTOR.h"
#include <iostream>
#include <portaudio.h> // Include portaudio.h here
#include <cmath>
#include <fftw3.h>
#include <chrono>
#include <deque> // Include deque for storing timestamps - double-ended queue container to store a fixed number of timestamps
  
// Define a deque to store timestamps of detections
std::deque<std::chrono::steady_clock::time_point> detectionTimestamps;

// Define a function to process tempo based on time differences
void playingTempo(const std::chrono::steady_clock::duration& timeDifference) {
    // Calculate tempo based on the time difference between ball contact detections
    double tempo = 60.0 / std::chrono::duration<double>(timeDifference).count(); // Beats per minutes calculation
    std::cout << "Tempo(Game Rhythm): " << tempo << " BPM" << std::endl;
}

std::chrono::steady_clock::time_point lastDetectionTime; // where does this data really belong?

#define SOUND_THRESHOLD 25 // Define SOUND_THRESHOLD here if it's not declared elsewhere

int audioCallback(const void *inputBuffer, void *outputBuffer,  //called by portaudio library
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData) {
    const float *input = (const float *)inputBuffer; //convert inputBuffer from const void to const float and stores it in input

    // Perform FFT on the input audio
   fftwf_complex *fft_result;
   fft_result = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * framesPerBuffer);
   fftwf_plan plan = fftwf_plan_dft_r2c_1d(framesPerBuffer, (float *)input, fft_result, FFTW_ESTIMATE);
   fftwf_execute(plan);

    // Calculate magnitude and perform sound detection
    bool soundDetected = false;
    for (int i = 0; i < framesPerBuffer; ++i) {
        float magnitude = std::abs(fft_result[i][0]);// For each frequency component, calculate the magnitude using the absolute value of the real part 
        if (magnitude > SOUND_THRESHOLD) {
            soundDetected = true;
            std::cout << "Detected signal amplitude is:" << magnitude << std::endl;
            break;
        }
    }
    fftwf_free(fft_result); //free alocated memory

    // If sound pulse is detected, process it further
    if (soundDetected) {
        auto currentTime = std::chrono::steady_clock::now();
        detectionTimestamps.push_back(currentTime); // Store timestamp of detection
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastDetectionTime).count();
        if (elapsedTime >= 500) {  // Ensure at least 500 milliseconds between detections
            std::cout << "Sound pulse detected!" << std::endl;
            lastDetectionTime = currentTime;
        }
    }

//For tempo detection
// Calculate time difference between consecutive detections
    if (detectionTimestamps.size() >= 2) { // Here checks if there is detection and at least two detections needed.
        auto timeDifference = detectionTimestamps.back() - detectionTimestamps.front();
        // Pass time difference to function for further processing
        playingTempo(timeDifference);
        // Remove oldest timestamp
        auto it = detectionTimestamps.begin();
        detectionTimestamps.erase(it);
       std::advance(it, 1);  // Advance iterator to the next element

    }

    return paContinue; // constant that indicates to the PortAudio stream callback function that audio processing should continue.
    //if there's an error or the stream should be stopped (paAbort, paComplete, etc.).
}
    