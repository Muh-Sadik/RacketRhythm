#include "CONTACT_DETECTOR.h"
#include <iostream>
#include <portaudio.h> // Include portaudio.h here
#include <cmath>
#include <fftw3.h>
#include <chrono>

std::chrono::steady_clock::time_point lastDetectionTime;

#define SOUND_THRESHOLD 25 // Define SOUND_THRESHOLD here if it's not declared elsewhere

int audioCallback(const void *inputBuffer, void *outputBuffer,
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData) {
    const float *input = (const float *)inputBuffer;

    // Perform FFT on the input audio
    fftwf_complex *fft_result;
    fft_result = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * framesPerBuffer);
    fftwf_plan plan = fftwf_plan_dft_r2c_1d(framesPerBuffer, (float *)input, fft_result, FFTW_ESTIMATE);
    fftwf_execute(plan);
    fftwf_destroy_plan(plan);

    // Calculate magnitude and perform sound detection
    bool soundDetected = false;
    for (int i = 0; i < framesPerBuffer; ++i) {
        float magnitude = std::abs(fft_result[i][0]);
        if (magnitude > SOUND_THRESHOLD) {
            soundDetected = true;
            break;
        }
    }
    fftwf_free(fft_result);

    // If sound pulse is detected, process it further
    if (soundDetected) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastDetectionTime).count();
        if (elapsedTime >= 500) {  // Ensure at least 500 milliseconds between detections
            std::cout << "Sound pulse detected!" << std::endl;
            lastDetectionTime = currentTime;
        }
    }

    return paContinue;
}