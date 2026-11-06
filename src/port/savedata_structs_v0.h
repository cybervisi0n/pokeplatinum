#ifndef SAVEDATA_STRUCTS_V0_H
#define SAVEDATA_STRUCTS_V0_H

#include <nitro/types.h>
#include <nitro/rtc.h>

#include "struct_defs/daycare.h"

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

#endif