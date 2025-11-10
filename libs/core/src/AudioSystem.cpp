#include <chip/core/AudioSystem.hpp>
#include <cstring>
#include <algorithm>

namespace chip::core {

bool AudioSystem::init(const AudioConfig& cfg, IAudioSource* source) {
    m_source = source;
    m_last_error.clear();
    m_frames_played = 0;

    if ((SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO) == 0) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
            m_last_error = SDL_GetError();
            return false;
        }
    }

    SDL_AudioSpec want{};
    want.freq = cfg.sample_rate;
    want.format = AUDIO_S16;
    want.channels = cfg.channels;
    want.samples = cfg.buffer_frames;
    want.callback = &AudioSystem::sdl_callback;
    want.userdata = this;

    const char* dev_name =
        cfg.device_name.empty() ? nullptr : cfg.device_name.c_str();

    SDL_AudioDeviceID dev =
        SDL_OpenAudioDevice(dev_name, 0, &want, nullptr, 0);
    if (dev == 0) {
        m_last_error = SDL_GetError();
        return false;
    }

    m_device = dev;
    m_sample_rate = want.freq;
    m_channels = want.channels;
    m_buffer_frames = want.samples;

    return true;
}

void AudioSystem::start() {
    if (m_device) SDL_PauseAudioDevice(m_device, 0);
}

void AudioSystem::stop() {
    if (m_device) SDL_PauseAudioDevice(m_device, 1);
}

uint64_t AudioSystem::frames_played() const {
    return m_frames_played;
}

double AudioSystem::time_sec() const {
    return m_sample_rate > 0
        ? static_cast<double>(m_frames_played) / m_sample_rate
        : 0.0;
}

AudioSystem::~AudioSystem() {
    if (m_device) {
        SDL_CloseAudioDevice(m_device);
        m_device = 0;
    }
    if (SDL_WasInit(SDL_INIT_AUDIO) == SDL_INIT_AUDIO) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}

void AudioSystem::sdl_callback(void* userdata, Uint8* stream, int len_bytes) {
    auto* self = static_cast<AudioSystem*>(userdata);
    if (!self || !self->m_source || self->m_channels <= 0) {
        std::memset(stream, 0, len_bytes);
        return;
    }

    auto* out = reinterpret_cast<int16_t*>(stream);
    const int total_samples = len_bytes / sizeof(int16_t);
    const int channels = self->m_channels;
    const int frames = total_samples / channels;

    self->m_source->render(out, frames, channels);
    self->m_frames_played += frames;
}

}
