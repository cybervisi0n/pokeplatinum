#include <nitro.h>


#include "gui_createmon_utils.h"

#include "party.h"
#include "pokemon.h"
#include "savedata.h"
#include "save_player.h"
#include "species.h"
#include "message_util.h"

#include "charcode_convert.h"

#include <stdlib.h>

static inline u32 GenerateShinyPersonality(u32 monOTID)
{
    u16 tid = monOTID & 0xFFFF;
    u16 sid = monOTID >> 16;

    return ((u32)sid << 16) | tid;
}

const char * GUI_CreateMon_GetSpeciesName(int speciesNo) {
    String * string = MessageUtil_SpeciesName(speciesNo, HEAP_ID_SYSTEM);
    char * nameBuf = malloc(sizeof(char) * string->size + 1);
    CharCode_ToAsciiString(string->data, nameBuf, string->size);
    nameBuf[string->size] = 0;
    String_Free(string);

    return nameBuf;
}

int GUI_CreateMon_GenPokemon(int speciesNo, int level, BOOL shiny, BOOL perfectIV) {
        Pokemon myPoke;

        SaveData * mySaveData = SaveData_Ptr();
        TrainerInfo * trainer = SaveData_GetTrainerInfo(mySaveData);
        String *playerName = TrainerInfo_NameNewString(trainer, HEAP_ID_FIELD3);
        u32 playerID = TrainerInfo_ID(trainer);

        Pokemon_InitWith(&myPoke, 
                         speciesNo, 
                         level, 
                         perfectIV ? 31 : INIT_IVS_RANDOM, 
                         shiny, 
                         shiny ? GenerateShinyPersonality(playerID) : 0, 
                         OTID_NOT_SET, 
                         0);

        Pokemon_SetValue(&myPoke, MON_DATA_OT_NAME_STRING, playerName);
        Pokemon_SetValue(&myPoke, MON_DATA_OT_ID, &playerID);

        String_Free(playerName);
        
        Party * myParty = SaveData_GetParty(mySaveData);
        if(myParty->currentCount >= 6) {
            return FALSE;
        } else {
            Party_AddPokemon(myParty, &myPoke);
        }

        return TRUE;
        
}