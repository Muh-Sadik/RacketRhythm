#include "ALSACapture.h"
//#include <iostream> // For std::cerr
//#include <cstdlib>  // For exit()

#define SAMPLE_RATE 48000U
#define NUM_CHANNELS_output 2
#define NUM_CHANNELS_input 1
#define FRAMES_PER_BUFFER 2000

ALSACapture::ALSACapture(const std::string& captureDevice, const std::string& playbackDevice) : captureDevice(captureDevice), playbackDevice(playbackDevice), capture_handle(nullptr), playback_handle(nullptr), hw_params(nullptr), capture_buffer_size(FRAMES_PER_BUFFER), playback_buffer_size(FRAMES_PER_BUFFER) {}

ALSACapture::~ALSACapture() {
    
    if (capture_handle) {
        snd_pcm_close(capture_handle);
    }
    if (playback_handle) {
        snd_pcm_close(playback_handle);
    }
    if (hw_params) {
        snd_pcm_hw_params_free(hw_params);
    }
}

void ALSACapture::init() {
    int rc;
    unsigned int sampleRate = SAMPLE_RATE; // need to declare a separate unsigned int variable to be used inside rc 

    // Open PCM device for recording (capture)
    if ((rc = snd_pcm_open(&capture_handle, captureDevice.c_str(), SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        errorCallback("unable to open PCM capture device", rc);
    }

    // Open PCM device for playback
    if ((rc = snd_pcm_open(&playback_handle, playbackDevice.c_str(), SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        errorCallback("unable to open PCM playback device", rc);
    }

    // Allocate hardware parameters structure
    if ((rc = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        errorCallback("cannot allocate hardware parameter structure", rc);
    }

    // Initialize hardware parameters for capture
    if ((rc = snd_pcm_hw_params_any(capture_handle, hw_params)) < 0) {
        errorCallback("cannot initialize hardware parameter structure (capture)", rc);
    }

    // Set hardware parameters for capture
    if ((rc = snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        errorCallback("cannot set access type (capture)", rc);
    }
    if ((rc = snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
        errorCallback("cannot set sample format (capture)", rc);
    }
    if ((rc = snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &sampleRate , 0)) < 0) {
        errorCallback("cannot set sample rate (capture)", rc);
    }
    if ((rc = snd_pcm_hw_params_set_channels(capture_handle, hw_params, NUM_CHANNELS_input)) < 0) {
        errorCallback("cannot set channel count (capture)", rc);
    }


    // Set buffer size for capture
    if ((rc = snd_pcm_hw_params_set_buffer_size_near(capture_handle, hw_params, &capture_buffer_size)) < 0) {
        errorCallback("cannot set buffer size (capture)", rc);
    }

    // Apply hardware parameters for capture
    if ((rc = snd_pcm_hw_params(capture_handle, hw_params)) < 0) {
        errorCallback("cannot apply hardware parameters (capture)", rc);
    }

    // Initialize hardware parameters for playback
    if ((rc = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0) {
        errorCallback("cannot initialize hardware parameter structure (playback)", rc);
    }

    // Set hardware parameters for playback
    if ((rc = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        errorCallback("cannot set access type (playback)", rc);
    }
    if ((rc = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
        errorCallback("cannot set sample format (playback)", rc);
    }
    if ((rc = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &sampleRate, 0)) < 0) {
        errorCallback("cannot set sample rate (playback)", rc);
    }
    if ((rc = snd_pcm_hw_params_set_channels(playback_handle, hw_params, NUM_CHANNELS_output)) < 0) {
        errorCallback("cannot set channel count (playback)", rc);
    }
    
    // Set buffer size for playback
    if ((rc = snd_pcm_hw_params_set_buffer_size_near(playback_handle, hw_params, &playback_buffer_size)) < 0) {
        errorCallback("cannot set buffer size (playback)", rc);
    }

    // Apply hardware parameters for playback
    if ((rc = snd_pcm_hw_params(playback_handle, hw_params)) < 0) {
        errorCallback("cannot apply hardware parameters (playback)", rc);
    }
}


void ALSACapture::captureAndPlaybackLoop() {


    while (true) {
        short capture_buffer[capture_buffer_size]; // 16-bit per sample, 1 channel
        short playback_buffer[playback_buffer_size * NUM_CHANNELS_output]; // 16-bit per sample, 2 channels
        int rc;

        // Capture audio
        if ((rc = snd_pcm_readi(capture_handle, capture_buffer, capture_buffer_size)) < 0) {
            if (rc == -EPIPE) {
                errorCallback("overrun occurred (capture)", rc);
                snd_pcm_prepare(capture_handle);
            } else {
                errorCallback("error reading from PCM capture device", rc);
            }
        }


        // Duplicate mono audio to stereo
        for (size_t i = 0; i < capture_buffer_size; ++i) {
            playback_buffer[i * NUM_CHANNELS_output] = capture_buffer[i];
            playback_buffer[i * NUM_CHANNELS_output + 1] = capture_buffer[i];

            // Reduce the volume by 50% (attenuation)
            playback_buffer[i * NUM_CHANNELS_output] /= 3;
            playback_buffer[i * NUM_CHANNELS_output + 1] /= 3;
        }

        // Playback captured audio (stereo)
        if ((rc = snd_pcm_writei(playback_handle, playback_buffer, playback_buffer_size)) < 0) {
            if (rc == -EPIPE) {
                errorCallback("underrun occurred (playback)", rc);
                snd_pcm_prepare(playback_handle);
            } else {
                errorCallback("error writing to PCM playback device", rc);
            }
        }
    }
}



void ALSACapture::errorCallback(const char* error, int errnum) {
    std::cerr << "ALSA error (" << errnum << "): " << error << std::endl;
    exit(1);
} /* ALSACapture.cpp */