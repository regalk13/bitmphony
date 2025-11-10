# TODO

## Play chip files

- [ ] `libs/core`: Solid `AudioSystem` wrapper around SDL2
  - [ ] RAII init/teardown
  - [ ] Configurable sample rate / channels / buffer size
  - [ ] `IAudioSource` interface
- [ ] `libs/core`: `GmePlayer` (or `GmeSource`) wrapper around gme
  - [ ] Open file, start track, render into buffer
  - [ ] Basic error reporting
- [ ] `apps/bitmphony`: minimal `--play <file>` using `AudioSystem` + `GmePlayer`

## Flags

- [ ] `libs/cli`: small parser that returns a `Command` struct
  - [ ] `--play <file>`
  - [ ] `--compile <script.lua> -o <out>`
  - [ ] `--watch <script.lua> -o <out>`
  - [ ] `--live <script.lua>`
- [ ] `apps/bitmphony`: wire commands to core/scripting/watch logic
- [ ] Help usage output (`bitmphony --help`)


## Scripting/Compile

- [ ] `libs/scripting`: Lua-based compiler interface
  - [ ] Load Lua script
  - [ ] Emit simple internal song format (notes/events)
- [ ] `--compile`: script → file (even if format is temporary)
- [ ] `--play` that can play compiled format too (through a source implementation)

## Watch Mode

- [ ] `libs/util` or similar: polling-based file watcher (no heavy deps)
- [ ] `--watch`: on change → recompile script
- [ ] `--live`: compile + play + hot-reload on change
  - [ ] Smoothly swap active source (no crash / pop if possible)

## Visualization

- [ ] `libs/viz`: simple visualization API
  - [ ] Consume samples from an `IAudioSource` or tap
  - [ ] Expose levels / bars / minimal spectrum
- [ ] `apps/bitmphony`: `--visualize` mode
  - [ ] TUI/text visualization first
  - [ ] Works with both file playback and live compile

## Build/Tooling

- [ ] CMake
  - [ ] Proper `libs/core`, `libs/scripting`, `libs/cli`, `libs/viz`, `apps/bitmphony` targets
  - [ ] Use `install(TARGETS ...)` instead of manual `cp`
- [ ] Nix
  - [ ] Keep flake working
  - [ ] Consider flake-parts once layout is stable
- [ ] Portability
  - [ ] Confirm builds on non-Nix Linux and other platforms
  - [ ] Check SDL2 + gme discovery via pkg-config and fallback
