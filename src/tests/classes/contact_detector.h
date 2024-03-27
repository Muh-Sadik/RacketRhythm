#ifndef CONTACT_DETECTOR_H
#define CONTACT_DETECTOR_H

#include <chrono>
#include <fftw3.h>
#include <portaudio.h> 

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