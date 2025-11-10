#pragma once
#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

namespace chip::core {

struct AudioConfig {
    int sample_rate   = 48000;
    int channels      = 2;
    int buffer_frames = 1024;
    std::string device_name;
};

class IAudioSource {
public:
    virtual ~IAudioSource() = default;
    virtual void render(int16_t* out, int frames, int channels) = 0;
};

class AudioSystem {
public:
    AudioSystem() = default;
    ~AudioSystem();

    bool init(const AudioConfig& cfg, IAudioSource* source);
    bool valid() const { return m_device != 0; }

    void start();
    void stop();

    int sample_rate()   const { return m_sample_rate; }
    int channels()      const { return m_channels; }
    int buffer_frames() const { return m_buffer_frames; }

    uint64_t frames_played() const;
    double   time_sec() const;

    const std::string& last_error() const { return m_last_error; }

private:
    SDL_AudioDeviceID m_device = 0;
    IAudioSource*     m_source = nullptr;
    int               m_sample_rate = 0;
    int               m_channels = 0;
    int               m_buffer_frames = 0;
    mutable uint64_t  m_frames_played = 0;
    std::string       m_last_error;

    static void sdl_callback(void* userdata, Uint8* stream, int len_bytes);
};

}
