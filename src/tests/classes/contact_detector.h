#ifndef CONTACT_DETECTOR_H
#define CONTACT_DETECTOR_H

#include <chrono>
#include <fftw3.h>
#include <portaudio.h> 

extern const int SOUND_THRESHOLD;

// Define function prototypes
int audioCallback(const void *inputBuffer, void *outputBuffer,
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData);

#endif /* CONTACT_DETECTOR_H */