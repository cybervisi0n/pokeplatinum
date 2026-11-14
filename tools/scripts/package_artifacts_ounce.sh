#!/bin/bash
BUILDDIR=build_ounce

cd $BUILDDIR
mkdir mkdir -p release_ounce/pokeplatinum
cp firmware.bin release_ounce/pokeplatinum/firmware.bin
cp header.bin release_ounce/pokeplatinum/header.bin
cp pokeplatinum.nro release_ounce/pokeplatinum/pokeplatinum.nro
cp language.csv release_ounce/pokeplatinum/language.csv
