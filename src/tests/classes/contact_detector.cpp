#include "contact_detector.h"
#include "ball_contact_count.h"
#include <portaudio.h>  
#include <cmath>
#include <fftw3.h>

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
            soundDetected = true;   // Sound pulse is detected, you can process it further
            break;
        }
    }
    fftwf_free(fft_result); //free alocated memory

 // If sound pulse is detected, process it further
    if (soundDetected) {
        processBallContact();
        }

    return paContinue; // constant that indicates to the PortAudio stream callback function that audio processing should continue.
    //if there's an error or the stream should be stopped (paAbort, paComplete, etc.).
}
    