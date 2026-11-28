#include <nitro.h>
#include <simulator/sim.h>
#include <simulator/sim_gui.hpp>
#include <simulator/imgui/imgui.hpp>

#include "sim_gui_prj.hpp"

#include "port/sim_config_prj.h"

#include "species.h"
#include "gui_createmon_utils.h"

#include <string>
#include <vector>

namespace SIM::GUI {

static std::vector<std::string> sSpeciesNames;
static int sSpeciesNum = 0;
static int sLevel = 1;
static bool sShiny = false;
static bool sPerfectIVs = false;

static constexpr ImVec2 BtnSize = {100, 20};

void CreateMonInit() {
    sSpeciesNames.clear();
    sSpeciesNames.push_back("None");

    for(int i=1; i <= SPECIES_BAD_EGG; i++) {
        const char * pokemonName = GUI_CreateMon_GetSpeciesName(i);
        sSpeciesNames.push_back(std::string(pokemonName));
        delete pokemonName;
    }
}

void CreateMonMain(bool * openState) {
    ImGui::Begin("Gen Pokemon", openState);

    ImGui::Combo("Species", &sSpeciesNum, [](void * vec, int idx){
        std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
        if(idx < 0 || idx > vector->size()) {
            return "";
        }

        return vector->at(idx).c_str();
    }, reinterpret_cast<void*>(&sSpeciesNames), sSpeciesNames.size());

    ImGui::InputScalar("Level", ImGuiDataType_U8, &sLevel, nullptr, nullptr, nullptr, ImGuiInputTextFlags_CharsDecimal);
    if(sLevel < 1) {
        sLevel = 1;
    } else if (sLevel > 100) {
        sLevel = 100;
    }

    ImGui::Checkbox("Shiny", &sShiny);
    ImGui::Checkbox("Perfect IVs", &sPerfectIVs);

    if(ImGui::Button("Gen Pokemon", BtnSize)) {
        GUI_CreateMon_GenPokemon(sSpeciesNum, sLevel, sShiny, sPerfectIVs);
    }

    ImGui::End();
}
}