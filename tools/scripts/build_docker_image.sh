#!/bin/bash
PROJECT=pokeplatinum

FIRSTARG=1
INTERACTIVE=0
DOCKERFILENAME=tools/docker/Dockerfile

IMAGENAME=$PROJECT-build

echo "Building container image $IMAGENAME"
docker build -t $IMAGENAME -f $DOCKERFILENAME .
