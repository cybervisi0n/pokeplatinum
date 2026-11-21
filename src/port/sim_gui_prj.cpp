#include <nitro.h>
#include <simulator/sim.h>
#include <simulator/sim_gui.hpp>
#include <simulator/imgui/imgui.hpp>
#include "sim_gui_prj.hpp"
#include "port/debug_field.h"

namespace SIM::GUI {

static bool sCreateMonOpen = false;
static bool sCreateItemOpen = false;
static bool sConfigPrjOpen = false;
static bool sCheatsOpen = false;
static bool sMapJumpOpen = false;

void PrjMain() {
    //ImGui::Begin("pokeplatinum");

    ImGui::Text("Git rev: %s", SIM_GetProjectGitHash());

    AppButton("Cheats", &sCheatsOpen, nullptr, CheatsMain);
    AppButton("Config", &sConfigPrjOpen, nullptr, ConfigPrjMain);
    AppButton("Gen Pokemon", &sCreateMonOpen, CreateMonInit, CreateMonMain);
    AppButton("Gen Items", &sCreateItemOpen, CreateItemInit, CreateItemMain);

    if(DEBUG_GetFieldSystem() != nullptr) {
        AppButton("Jump to Map", &sMapJumpOpen, MapJumpInit, MapJumpMain);
    }

    //ImGui::End();
}
}