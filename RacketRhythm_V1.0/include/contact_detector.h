#ifndef CONTACT_DETECTOR_H
#define CONTACT_DETECTOR_H

#include <chrono>      // For time-related operations, we are using it for measuring durations between ball contacts.
#include <fftw3.h>     // Used for Fast Fourier Transform (FFT) using FFTW.
#include <portaudio.h> // Used for audio input/output using PortAudio.

class contactdetector {
    public:
    static int audioCallback(const void *inputBuffer, void *outputBuffer,
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData);
private:

};

#endif /* CONTACT_DETECTOR_H */