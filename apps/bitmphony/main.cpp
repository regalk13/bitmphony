#include <gme/gme.h>
#include <cstdio>
#include <string>
#include <SDL2/SDL.h>

#include <chip/core/AudioSystem.hpp>

class GmeSource : public chip::core::IAudioSource {
public:
    ~GmeSource() override { close(); }

    bool open(const std::string& path, int sample_rate) {
        close();
        if (gme_open_file(path.c_str(), &emu, sample_rate) != 0) {
            std::fprintf(stderr, "gme_open_file failed\n");
            emu = nullptr;
            return false;
        }
        if (gme_start_track(emu, 0) != 0) {
            std::fprintf(stderr, "gme_start_track failed\n");
            close();
            return false;
        }
        return true;
    }

    void close() {
        if (emu) {
            gme_delete(emu);
            emu = nullptr;
        }
    }

    void render(int16_t* out, int frames, int channels) override {
        if (!emu) {
            std::fill(out, out + frames * channels, 0);
            return;
        }
        long samples = static_cast<long>(frames) * channels;
        gme_play(emu, samples, out);
    }

private:
    Music_Emu* emu = nullptr;
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::fprintf(stderr, "usage: bitmphony <file>\n");
        return 1;
    }

    const std::string path = argv[1];

    GmeSource src;
    const int sample_rate = 48000;
    if (!src.open(path, sample_rate)) {
        return 1;
    }

    chip::core::AudioConfig cfg;
    cfg.sample_rate = sample_rate;
    cfg.channels = 2;
    cfg.buffer_frames = 1024;

    chip::core::AudioSystem audio;
    if (!audio.init(cfg, &src)) {
        std::fprintf(stderr, "audio init failed: %s\n",
                     audio.last_error().c_str());
        return 1;
    }

    audio.start();
    std::puts("Playing... Ctrl+C or close window to quit.");

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }
        SDL_Delay(16);
    }

    audio.stop();
    return 0;
}
