# Pokémon Platinum PC Port

This is an experimental PC port of Pokemon Platinum based on the [pret](https://github.com/pret/pokeplatinum) decompilation project. It is powered by the [libntr](https://github.com/cybervisi0n/libntr) suite, a collection of libraries that replace the NitroSDK to allow for easy porting of Nintendo DS titles.

Feel free to join us on [Discord](https://discord.gg/ZgtPszuBeN) for discussion or support.

It should be possible to play the game from start to finish, but there could be crashes and graphical bugs. 

Generative AI/LLM shortcuts were **not** used in the creation of this project and the libntr libraries. **Pull requests or code suggestions using AI will be rejected.**

## Features
* 60 FPS
* Gamepad and keyboard controls
* Upscale the 3D rendering up to 20x the NDS resolution
* Local multiplayer over LAN or the internet
* WiFi support through the Poke Classic Network (BETA)
* Compile and debug any of the supported build targets from the same source tree
* Settings menu: Press F1 or left stick on a gamepad to open

## Supported Platforms
* Linux x86_64
* Windows x86_64
* PS5
* NX
* Ounce

## Save Data Compatibility
Save data from emulators and DS hardware is compatible with the PC port, and PC port save data is also compatible with emulators and real hardware. On first launch of v0.2.0 or newer, save data from <= v0.1.13 will be upgraded to the new format.

## Setup Guide
To run the [pre-built binaries found in the releases tab](https://github.com/cybervisi0n/pokeplatinum/releases), **you MUST dump and provide your own US ROM of Pokémon Platinum (CPUE01).** On first launch, you will be prompted to select your .nds ROM file as seen in the images below:

![ROM extraction dialog](images/RomExtraction.png)

* After this has been completed once, you can launch the game simply by running main.exe.
* Access in-game settings and debug tools by pressing the grave accent/tilde key or F1 key

### Default Gamepad Controls
* ABXYLR are straightforward
* Dpad or Left stick is bound to the DS Dpad
* Right stick is bound to a virtual touch cursor
* * Clicking the right stick simulates a touch
* Clicking the left stick toggles the menu

### Notes for PS5 Users
Currently, ROM extraction is not supported on the PS5. Play the PC version once to extract your ROM, then upload the extracted files to the PPSA92008 folder on the PS5.

### Playing in other languages
This project only supports the executable code of the US version (CPUE01). However, it is possible to import ROMs from other regions to get the majority of the text in the desired language. The relevant language files will be moved to the correct locations automatically during ROM extraction.

## Forking Guidelines
You are welcome to create forks of this project, but do not publish any releases containing the compiled assets alongside PC port binaries.

If you publish releases, you are responsible for also complying with the MIT license on libntr by including the copyright and disclaimer.

cybervisi0n is not affiliated with any forks of this project.

## Building on Linux
### Dockerized build (Recommended)
This only requires Docker to be installed and setup on your system. The drun.sh script is used to build the docker image and run build commands in it. Build with:
* ./drun.sh make linux

The container image will automatically be built the first time this script is run.

### Non-container build
Required Packages (arch linux):
* nasm
* enet
* arm-none-eabi-gcc (required by the base pret project)
* ninja
* flex
* bison

To build: 
* make linux

Alternatively:
* meson setup build
* cd build
* meson configure -Dbuild_target=linux
* meson compile

You can also build a ROM from the same source tree, just run:
* make

## Building on Windows
From a freshly cloned repo, run the "Install_MSys2.ps1" script in powershell. This will create a portable MSys2 build environment with all dependencies installed in the repo. This only has to be done once per repo.

To build, run "Launch_MSys2.ps1" and it will launch a MSys2 bash shell. From here, run "make win64" to build.

## Notes
Target executable will be in ./build_(platform) directory.
ROM will be built in /build directory

firmware.bin does not contain any real DS firmware, it only contains an offset address and enough space to keep the WiFi config.

You can update your SDKs using "meson subprojects update"

### Tracy Profiling
To enable, run ./meson.sh configure -Dtracy_enable=true {build_folder} from the repo root. This works on Win64, Linux, and NX build targets.

