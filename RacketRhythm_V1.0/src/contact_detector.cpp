#include "contact_detector.h"
#include "ball_contact_count.h"
#include <portaudio.h>  // Used for audio input/output using PortAudio.
#include <cmath>        // Used for mathematical calculations.
#include <fftw3.h>      // Used for Fast Fourier Transform (FFT) using FFTW.
#include <iostream>     // Used for input/output stream commands.

#define SOUND_THRESHOLD 1.2
#define SAMPLE_RATE 48000

bool threeseconds = false;

static std::chrono::steady_clock::time_point lastContactTime = std::chrono::steady_clock::now();

int contactdetector::audioCallback(const void *inputBuffer, void *outputBuffer,  //called by portaudio library
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData) {
    const float *input = (const float *)inputBuffer; //convert inputBuffer from const void to const float and stores it in input
    static std::chrono::steady_clock::time_point prevContactTime = std::chrono::steady_clock::now();

    // Perform FFT on the input audio
   fftwf_complex *fft_result;
   fft_result = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * framesPerBuffer);
   fftwf_plan plan = fftwf_plan_dft_r2c_1d(framesPerBuffer, (float *)input, fft_result, FFTW_ESTIMATE);
   fftwf_execute(plan);

   // Define frequency range (800 - 1200 Hz)
    int startFreqIndex = 800 * framesPerBuffer / SAMPLE_RATE; // Convert frequency to index
    int endFreqIndex = 900 * framesPerBuffer / SAMPLE_RATE; // Convert frequency to index

    // Calculate magnitude and perform sound detection
    bool soundDetected = false;
    for (int i = startFreqIndex; i <= endFreqIndex; ++i) {
        float magnitude = std::abs(fft_result[i][0]);// For each frequency component, calculate the magnitude using the absolute value of the real part 
        if (magnitude > SOUND_THRESHOLD) {
            soundDetected = true;   // Sound pulse is detected, you can process it further
        //  std::cout << "Sound detected at frame " << i << std::endl; // Debugging statement, uncomment when needed.
            break;
        }
    }
    fftwf_free(fft_result); //free alocated memory
    BallContactCount BallContactCount; // Instantiate an object of BallContactCount

 // If sound pulse is detected, process it further
    if (soundDetected) {
        // Update the time of the last contact when a contact is detected
        lastContactTime = std::chrono::steady_clock::now();
        // Call the processBallContact() method
        BallContactCount.processBallContact();
    } else {
        // Calculate the time difference between the current time and the time of the last contact
        auto currentTime = std::chrono::steady_clock::now();
        auto timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastContactTime).count();

        // Check if the time difference is more than three seconds
        if (timeDifference > 2000) {
            threeseconds = true;
        } else {
            threeseconds = false;
        }
    }

    return paContinue; /* constant that indicates to the PortAudio stream callback function that audio processing should continue.
     if there's an error or the stream should be stopped (paAbort, paComplete, etc.) */
} /* contact_detector.cpp */
    