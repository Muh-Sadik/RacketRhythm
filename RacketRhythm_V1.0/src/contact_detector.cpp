#include "contact_detector.h"
#include "ball_contact_count.h"
#include <portaudio.h>  // Used for audio input/output using PortAudio.
#include <cmath>        // Used for mathematical calculations.
#include <fftw3.h>      // Used for Fast Fourier Transform (FFT) using FFTW.
#include <iostream>     // Used for input/output stream commands.

#define SOUND_THRESHOLD 25

int contactdetector::audioCallback(const void *inputBuffer, void *outputBuffer,  //called by portaudio library
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

   // Define frequency range
    int startFreqIndex = 700 * framesPerBuffer / SAMPLE_RATE; // Convert frequency to index
    int endFreqIndex = 1000 * framesPerBuffer / SAMPLE_RATE; 

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

 // If sound pulse is detected, process it further
    if (soundDetected) {
        BallContactCount BallContactCount; // Instantiate an object of BallContactCount
        BallContactCount.processBallContact(); // Call the processBallContact() method
       
        }

    return paContinue; /* constant that indicates to the PortAudio stream callback function that audio processing should continue.
     if there's an error or the stream should be stopped (paAbort, paComplete, etc.) */
} /* contact_detector.cpp */
    