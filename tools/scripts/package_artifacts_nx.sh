#!/bin/bash
BUILDDIR=build_nx

cd $BUILDDIR
mkdir mkdir -p release_nx/pokeplatinum
cp firmware.bin release_nx/pokeplatinum/firmware.bin
cp header.bin release_nx/pokeplatinum/header.bin
cp pokeplatinum.nro release_nx/pokeplatinum/pokeplatinum.nro
cp language.csv release_nx/pokeplatinum/language.csv
