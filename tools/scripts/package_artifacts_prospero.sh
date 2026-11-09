#!/bin/bash
BUILDDIR=build_prospero

cd $BUILDDIR
mkdir release_prospero
mv PPSA92008 release_prospero/PPSA92008
cp ../language.csv release_prospero/language.csv