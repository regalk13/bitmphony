{
  description = "Bitmphony Flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in {
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "bitmphony";
          version = "0.1.0";
          src = self;

          nativeBuildInputs = [
            pkgs.cmake
            pkgs.ninja
            pkgs.pkg-config
          ];

          buildInputs = [
            pkgs.SDL2
            pkgs.game-music-emu
          ];

          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
            "-G" "Ninja"
          ];
          configurePhase = ''cmake -S . -B build $cmakeFlags'';
          buildPhase     = ''cmake --build build --parallel'';
          installPhase   = ''
              mkdir -p $out/bin
              cp build/bitmphony $out/bin/
          '';
        };

        devShells.default = pkgs.mkShell {
          nativeBuildInputs = [
            pkgs.cmake
            pkgs.ninja
            pkgs.pkg-config
            pkgs.gcc
          ];
        };

        apps.default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/bitmphony";
        };
      });
}
