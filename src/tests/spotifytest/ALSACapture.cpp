#include "ALSACapture.h"
//#include <iostream> // For std::cerr
//#include <cstdlib>  // For exit()

#define SAMPLE_RATE 48000U
#define NUM_CHANNELS 1
#define FRAMES_PER_BUFFER 256

ALSACapture::ALSACapture(const std::string& captureDevice, const std::string& playbackDevice) : captureDevice(captureDevice), playbackDevice(playbackDevice), capture_handle(nullptr), playback_handle(nullptr), hw_params(nullptr) {}

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
    if ((rc = snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S32_LE)) < 0) {
        errorCallback("cannot set sample format (capture)", rc);
    }
    if ((rc = snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &sampleRate , 0)) < 0) {
        errorCallback("cannot set sample rate (capture)", rc);
    }
    if ((rc = snd_pcm_hw_params_set_channels(capture_handle, hw_params, NUM_CHANNELS)) < 0) {
        errorCallback("cannot set channel count (capture)", rc);
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
    if ((rc = snd_pcm_hw_params_set_channels(playback_handle, hw_params, NUM_CHANNELS)) < 0) {
        errorCallback("cannot set channel count (playback)", rc);
    }

    // Apply hardware parameters for playback
    if ((rc = snd_pcm_hw_params(playback_handle, hw_params)) < 0) {
        errorCallback("cannot apply hardware parameters (playback)", rc);
    }
}

void ALSACapture::captureAndPlaybackLoop() {
    while (true) {
        char buffer32[FRAMES_PER_BUFFER * 4]; // 32-bit per sample
        char buffer16[FRAMES_PER_BUFFER * 2]; // 16-bit per sample
        int rc;

        // Capture audio
        if ((rc = snd_pcm_readi(capture_handle, buffer32, FRAMES_PER_BUFFER)) < 0) {
            if (rc == -EPIPE) {
                errorCallback("overrun occurred (capture)", rc);
                snd_pcm_prepare(capture_handle);
            } else {
                errorCallback("error reading from PCM capture device", rc);
            }
        }

        // Check if sound is detected
        if (isSoundDetected(buffer32, FRAMES_PER_BUFFER)) {
            // Sound detected, do something
            std::cout << "sounddetected" << std::endl;
        }

        // Playback captured audio
        if ((rc = snd_pcm_writei(playback_handle, buffer16, FRAMES_PER_BUFFER)) < 0) {
            if (rc == -EPIPE) {
                errorCallback("underrun occurred (playback)", rc);
                snd_pcm_prepare(playback_handle);
            } else {
                errorCallback("error writing to PCM playback device", rc);
            }
        }
    }
}

bool ALSACapture::isSoundDetected(const char* buffer32, const snd_pcm_uframes_t frames) {
    // Calculate the average amplitude of the audio samples in the buffer
    double sum = 0.0;
    for (int i = 0; i < frames * NUM_CHANNELS * 4; i += 4) { // Assuming 32-bit per sample
        int sample = (buffer32[i + 3] << 24) | (buffer32[i + 2] << 16) | (buffer32[i + 1] << 8) | (buffer32[i] & 0xff); // Little-endian conversion
        double amplitude = sample / 2147483648.0; // Normalize to [-1, 1]
        sum += std::abs(amplitude);
    }
    double averageAmplitude = sum / (frames * NUM_CHANNELS);

    // Set a threshold for sound detection
    const double threshold = 0.2;

    // Compare average amplitude with the threshold
    return averageAmplitude > threshold;
}


void ALSACapture::errorCallback(const char* error, int errnum) {
    std::cerr << "ALSA error (" << errnum << "): " << error << std::endl;
    exit(1);
} /* ALSACapture.cpp */