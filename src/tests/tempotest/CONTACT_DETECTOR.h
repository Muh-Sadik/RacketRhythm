#ifndef CONTACT_DETECTOR_H
#define CONTACT_DETECTOR_H

#include <chrono>
#include <fftw3.h>
#include <portaudio.h> // Include portaudio.h here
#include <vector>

// maybe we can Define constants here like, sound thershold
extern std::chrono::steady_clock::time_point lastDetectionTime; // where does this data really belong?

// Define function prototypes
int audioCallback(const void *inputBuffer, void *outputBuffer,
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData);

#endif /* CONTACT_DETECTOR_H */