#ifndef ALSACAPTURE_H
#define ALSACAPTURE_H

#include <iostream>
#include <alsa/asoundlib.h>
class ALSACapture {
public:
    ALSACapture(const std::string& captureDevice, const std::string& playbackDevice);
    ~ALSACapture();
    void init();
    void captureAndPlaybackLoop();
    bool isSoundDetected(const char* buffer32, const snd_pcm_uframes_t frames);

private:
    std::string captureDevice;
    std::string playbackDevice;
    snd_pcm_t *capture_handle;
    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;

    void errorCallback(const char* error, int errnum);
};

#endif /* ALSACAPTURE_H */