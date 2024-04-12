#ifndef ALSACAPTURE_H
#define ALSACAPTURE_H

#include <iostream>         // Used for input/output stream commands.
#include <alsa/asoundlib.h> // Used for audio input/output and sound card control using ALSA (Advanced Linux Sound Architecture).

class ALSACapture {
public:
    ALSACapture(const std::string& captureDevice, const std::string& playbackDevice);
    ~ALSACapture();
    void init();
    void captureAndPlaybackLoop();

private:
    std::string captureDevice;
    std::string playbackDevice;
    snd_pcm_t *capture_handle;
    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_uframes_t capture_buffer_size;
    snd_pcm_uframes_t playback_buffer_size;
    
    int shared_memory_id;
    struct SharedData {
        double tempoValue;
    };
    SharedData *shared_memory;

    void errorCallback(const char* error, int errnum);
    void initSharedMemory();

};

#endif /* ALSACAPTURE_H */