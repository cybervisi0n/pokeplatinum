#!/bin/bash

# Usage: download_prospero_lib.sh github-url release-file-name

GithubRepoUrl=$1
ReleaseFileName=$2

url=$GithubRepoUrl/releases/latest/download/$ReleaseFileName

rm -f temp.zip

wget $url -O temp.zip
unzip -o ./temp.zip -d prospero-deps

rm -f temp.zip
