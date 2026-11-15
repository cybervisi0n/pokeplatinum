#!/bin/bash

# Run me from the root of the repo!

PAYLOAD_SDK_URL=https://github.com/ps5-payload-dev/sdk/releases/latest/download/ps5-payload-sdk.zip
PACBREW_RELEASE_URL=https://github.com/ps5-payload-dev/pacbrew-repo/releases/latest/download/ps5-payload-dev.tar.gz

mkdir -p prospero-deps/

wget $PAYLOAD_SDK_URL
unzip -o ./ps5-payload-sdk.zip -d prospero-deps
rm -f ps5-payload-sdk.zip

mkdir -p prospero-deps/include
mkdir -p prospero-deps/lib

# Forcibly remove the ps5 payload runtime from the SDK since this is a native app
sed -i 's|^LIBS_CRT="${PS5_PAYLOAD_SDK}/target/lib/crt1\.o"$|LIBS_CRT=""|' prospero-deps/ps5-payload-sdk/bin/prospero-clang++

# Download OpenGL libraries
./tools/scripts/download_prospero_lib.sh  https://github.com/cybervisi0n/ps5-mesa-radv ps5-mesa_release.zip

# Download ps5-sdl2
./tools/scripts/download_prospero_lib.sh https://github.com/cybervisi0n/ps5-sdl2 ps5-sdl2_release.zip
