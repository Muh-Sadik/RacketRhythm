#include <iostream>
#include <portaudio.h>  
#include <cmath>
#include <fftw3.h>
#include <chrono>
  
// Define a counter to store the number of ball contacts
int ballContactCount = 0;

// Define variables to store the times of the last two ball contacts
std::chrono::steady_clock::time_point lastContactTime;
std::chrono::steady_clock::time_point prevContactTime;

// Define a function to calculate and display the playing tempo
void playingTempo(const std::chrono::steady_clock::duration &elapsedTime) {
    // Calculate tempo based on the time difference between ball contacts
    double tempo = 60.0 / std::chrono::duration<double>(elapsedTime).count(); // Beats per minute calculation
    std::cout << "Tempo(Game Rhythm): " << tempo << " BPM" << std::endl;
}

// Function to handle ball contact event
void processBallContact() {
    auto currentTime = std::chrono::steady_clock::now();    // Get the current time
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - prevContactTime).count();   // elapsedTime converted to miliseconds
if (elapsedTime >= 500) {  // Ensure at least 500 milliseconds between detections
            ++ballContactCount;     // Increment the ball contact count
            std::cout << "Sound pulse detected!" << std::endl;
        }

    // Calculate tempo if there have been at least two ball contacts
    if (ballContactCount == 2) {
    // Calculate time difference from the last contact
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - prevContactTime);   // elapsedTime converted to miliseconds
        std::cout << "Time between contacts: " << elapsedTime.count() << " miliseconds" << std::endl;
        playingTempo(elapsedTime);
        ballContactCount =0 ;
    }
    // Update variables for next contact
    prevContactTime = lastContactTime;
    lastContactTime = currentTime;
}


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
            soundDetected = true;   // Sound pulse is detected, process it further
            //std::cout << "Sound pulse detected!" << std::endl;
            //std::cout << "Detected signal amplitude is:" << magnitude << std::endl;
           // processBallContact();
            break;
        }
    }
    fftwf_free(fft_result); //free alocated memory

 // If sound pulse is detected, process it further
    if (soundDetected) {
        processBallContact();
        }


    return paContinue;
}
    