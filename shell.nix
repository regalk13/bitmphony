{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  name = "chipplayer-shell";
  buildInputs = with pkgs; [
    cmake
    pkg-config
    clang
    SDL2
    libgme
  ];
}
