#!/bin/bash

# Generates ShadowMount+ compatible app folder
# Usage build_app_folder.sh {repo root} {eboot self} {libc prx} {sce_sys source dir} {output}

REPO_ROOT=$1
EBOOT_SELF=$2
LIBC_PRX=$3
SCE_SYS_SOURCE_DIR=$4
OUTPUT_DIR=$5

# Eboot
mkdir -p $OUTPUT_DIR
cp $EBOOT_SELF $OUTPUT_DIR/eboot.bin

# libc.prx
mkdir -p $OUTPUT_DIR/sce_module
cp $LIBC_PRX $OUTPUT_DIR/sce_module/libc.prx

# sce_sys
mkdir -p $OUTPUT_DIR/sce_sys
cp $SCE_SYS_SOURCE_DIR/icon0.png $OUTPUT_DIR/sce_sys/icon0.png
cp $SCE_SYS_SOURCE_DIR/param.json $OUTPUT_DIR/sce_sys/param.json
#TODO check if these files exist, I believe they are optional
if [ -f $SCE_SYS_SOURCE_DIR/pic0.png ]; then
    magick convert $SCE_SYS_SOURCE_DIR/pic0.png $OUTPUT_DIR/sce_sys/pic0.dds
fi

if [ -f $SCE_SYS_SOURCE_DIR/pic1.png ]; then
    magick convert $SCE_SYS_SOURCE_DIR/pic1.png $OUTPUT_DIR/sce_sys/pic1.dds
fi

# AT9 file will just be copied for now
# TODO: find better atrac9 conversion tool for linux
if [ -f $SCE_SYS_SOURCE_DIR/snd0.at9 ]; then
    cp $SCE_SYS_SOURCE_DIR/snd0.at9 $OUTPUT_DIR/sce_sys/snd0.at9
fi
