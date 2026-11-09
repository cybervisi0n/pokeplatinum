#!/bin/bash
BUILDDIR=build_linux

cd $BUILDDIR
mkdir release_linux
cp firmware.bin release_linux/firmware.bin
cp header.bin release_linux/header.bin
cp main release_linux/main
cp pokeplatinum.AppImage release_linux/pokeplatinum.AppImage
cp language.csv release_linux/language.csv