#include <SDL2/SDL.h>
#include <gme/gme.h>
#include <cstdio>
#include <cstring>

static Music_Emu* g_emu = nullptr;

void sdl_audio_cb(void*, Uint8* stream, int len_bytes) {
    gme_play(g_emu, len_bytes / 2, reinterpret_cast<short*>(stream));
}

int main(int argc, char** argv) {
    if (argc < 2) { std::fprintf(stderr, "usage: chipplayer <file>\n"); return 1; }
    if (gme_open_file(argv[1], &g_emu, 48000) != 0) { std::fprintf(stderr, "gme_open_file failed\n"); return 1; }
    gme_start_track(g_emu, 0);

    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
        std::fprintf(stderr, "SDL_Init: %s\n", SDL_GetError()); return 1;
    }

    SDL_AudioSpec want{};
    want.freq = 48000;
    want.format = AUDIO_S16;
    want.channels = 2;
    want.samples = 1024;
    want.callback = sdl_audio_cb;

    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(nullptr, 0, &want, nullptr, 0);
    if (!dev) { std::fprintf(stderr, "SDL_OpenAudioDevice: %s\n", SDL_GetError()); return 1; }

    SDL_PauseAudioDevice(dev, 0);
    std::puts("Playing... press Ctrl+C or close window to quit.");

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }
        SDL_Delay(16);
    }

    SDL_PauseAudioDevice(dev, 1);
    SDL_CloseAudioDevice(dev);
    gme_delete(g_emu);
    SDL_Quit();
    return 0;
}
