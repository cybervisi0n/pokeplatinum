#ifndef SAVEDATA_STRUCTS_V0_H
#define SAVEDATA_STRUCTS_V0_H

#include <nitro/types.h>
#include <nitro/rtc.h>

#include "struct_defs/daycare.h"
#include "struct_defs/struct_0202F298.h"
#include "struct_defs/struct_0202F41C.h"
#include "struct_defs/struct_02030A80.h"
#include "pc_boxes.h"

typedef struct 
GameTime_v0 {
    BOOL canary;
    RTCDate date;
    RTCTime time;
    u32 day;
    s64 startTimestamp;
    s64 firstCompletionTimestamp; // the first time the hall of fame was entered
    u32 penaltyInMinutes;
} GameTime_v0;

typedef struct
SystemData_v0 {
    s64 rtcOffset;
    u8 macAddress[6];
    u8 ownerBirthMonth;
    u8 ownerBirthDayOfMonth;
    GameTime_v0 gameTime;
    u8 isMysteryGiftUnlocked;
    u8 padding_49[3];
    s32 dwcProfileId;
    u32 unused_50[3];
} SystemData_v0;

typedef struct 
DaycareMon_v0 {
    BoxPokemon boxMon;
    DaycareMail daycareMail;
    u32 steps;
} DaycareMon_v0;

typedef struct Daycare_v0 {
    DaycareMon_v0 mons[NUM_DAYCARE_MONS];
    u32 offspringPersonality;
    u8 stepCounter;
} Daycare_v0;

typedef struct 
PCBoxes_v0 {
    u32 currentBoxID;
    BoxPokemon boxMons[MAX_PC_BOXES][MAX_MONS_PER_BOX];
    u16 names[MAX_PC_BOXES][PC_BOX_NAME_BUFFER_LEN];
    u8 wallpapers[MAX_PC_BOXES];
    u8 unlockedWallpapers;
} PCBoxes_v0;

typedef struct 
UnkStruct_0202F41C_t_v0 {
    u16 unk_00[12];
    u8 unk_18[12];
    u16 unk_24;
    u8 unk_26;
    u8 unk_27;
    BattleRegulation battleRegulation;
    u16 unk_48;
    u8 unk_4A[14];
    u64 unk_58;
    UnkStruct_0202F298_sub1 unk_60;
} UnkStruct_0202F41C_v0;

typedef struct 
BattleRecording_v0 {
    u32 unk_00;
    UnkStruct_02030A80 unk_04;
    UnkStruct_0202F41C_v0 unk_84;
    UnkStruct_0202F298 unk_E8;
} BattleRecording_v0;

#endif