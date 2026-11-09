#include "port/savedata_upgrade.h"

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include <nitro/card/backup.h>

#include "constants/savedata/savedata.h"
#include "constants/savedata/save_table.h"
#include "math_util.h"
#include "pc_boxes.h"
#include "savedata.h"
#include "struct_defs/battle_recording.h"
#include "system_data.h"

#include "savedata_structs_v0.h"

#define SAVE_FLASH_SIZE (512*1024)


u32 v0BlockSizes[] = {
    104, 44, 1424, 1908, 940, 280, 160, 804, 488, 2176, 1932,
    5120, 2484, 32, 2100, 1120, 800, 264, 1120, 1568, 444, 368,
    1004, 5672, 14, 340, 248, 3016, 3744, 2240, 4088, 4904,
    1672, 40, 8, 64, 8, 74192
};

u32 v1BlockSizes[] = {
    92, 44, 1424, 1908, 940, 280, 160, 804, 480, 2176, 1932,
    5120, 2484, 32, 2100, 1120, 800, 264, 1120, 1568, 444, 368,
    1004, 5672, 14, 340, 248, 3016, 3744, 2240, 4088, 4904,
    1672, 40, 8, 64, 8, 74184,
};

static void UpgradeSaveFile(u8 * savePtr) {
    u8 * oldSaveData = malloc(sizeof(u8) * SAVE_SECTOR_SIZE * BACKUP_SECTOR_START);
    memcpy(oldSaveData, savePtr, SAVE_SECTOR_SIZE * BACKUP_SECTOR_START);

    u32 newSaveDataOffset = 0;
    u32 oldSaveDataOffset = 0;

    
    for(int block = 0; block < SAVE_TABLE_ENTRY_PC_BOXES; block++) {
        if(v0BlockSizes[block] == v1BlockSizes[block]) {
            // Old save struct and new save struct are the same size
            // Perform direct copy
            memcpy(savePtr + newSaveDataOffset, oldSaveData + oldSaveDataOffset, v0BlockSizes[block]);
            
        } else {
            // Perform struct conversion based on the block number
            if(block == 0) {
                // Convert SystemData_v0 to SystemData
                SystemData_v0 * oldSystemData = (SystemData_v0*)(oldSaveData + oldSaveDataOffset);
                SystemData * newSystemData = (SystemData *)(savePtr + newSaveDataOffset);

                newSystemData->rtcOffset = oldSystemData->rtcOffset;
                memcpy(newSystemData->macAddress, oldSystemData->macAddress, sizeof(newSystemData->macAddress));
                newSystemData->ownerBirthMonth = oldSystemData->ownerBirthMonth;
                newSystemData->ownerBirthDayOfMonth = oldSystemData->ownerBirthDayOfMonth;
                newSystemData->gameTime.canary = oldSystemData->gameTime.canary;
                newSystemData->gameTime.date.year = oldSystemData->gameTime.date.year;
                newSystemData->gameTime.date.month = oldSystemData->gameTime.date.month;
                newSystemData->gameTime.date.day = oldSystemData->gameTime.date.day;
                newSystemData->gameTime.date.week = oldSystemData->gameTime.date.week;
                newSystemData->gameTime.time.hour = oldSystemData->gameTime.time.hour;
                newSystemData->gameTime.time.minute = oldSystemData->gameTime.time.minute;
                newSystemData->gameTime.time.second = oldSystemData->gameTime.time.second;
                newSystemData->gameTime.day = oldSystemData->gameTime.day;
                newSystemData->gameTime.startTimestamp = oldSystemData->gameTime.startTimestamp;
                newSystemData->gameTime.firstCompletionTimestamp = oldSystemData->gameTime.firstCompletionTimestamp;
                newSystemData->gameTime.penaltyInMinutes = oldSystemData->gameTime.penaltyInMinutes;
                newSystemData->isMysteryGiftUnlocked = oldSystemData->isMysteryGiftUnlocked;
                newSystemData->dwcProfileId = oldSystemData->dwcProfileId;
            } else if(block == 8) {
                // Convert Daycare save
                Daycare_v0 * oldDaycare = (Daycare_v0 *)(oldSaveData + oldSaveDataOffset);
                Daycare * newDaycare = (Daycare *)(savePtr + newSaveDataOffset);

                memcpy(&newDaycare->mons[0].boxMon, &oldDaycare->mons[0].boxMon, sizeof(BoxPokemon));
                memcpy(&newDaycare->mons[0].daycareMail, &oldDaycare->mons[0].daycareMail, sizeof(DaycareMail));
                newDaycare->mons[0].steps = oldDaycare->mons[0].steps; 
                memcpy(&newDaycare->mons[1].boxMon, &oldDaycare->mons[1].boxMon, sizeof(BoxPokemon));
                memcpy(&newDaycare->mons[1].daycareMail, &oldDaycare->mons[1].daycareMail, sizeof(DaycareMail));
                newDaycare->mons[1].steps = oldDaycare->mons[1].steps; 
                newDaycare->offspringPersonality = oldDaycare->offspringPersonality;
                newDaycare->stepCounter = oldDaycare->stepCounter;
            } else {
                // Should not get here
                printf("FATAL: unexpected save data struct size mismatch\n");
            }
        }

        oldSaveDataOffset += v0BlockSizes[block];
        newSaveDataOffset += v1BlockSizes[block];
    }

    // Add Footer for first 36 blocks
    SaveBlockFooter *newFooter = (SaveBlockFooter*)(savePtr + 0xcf18);
    SaveBlockFooter *oldFooter = (SaveBlockFooter*)(oldSaveData + 0xcf2c);

    newFooter->saveBlockID = oldFooter->saveBlockID;
    newFooter->saveCounter = oldFooter->saveCounter;
    newFooter->blockCounter = oldFooter->blockCounter;
    newFooter->size = 0xcf2c;
    newFooter->signature = SECTOR_SIGNATURE;

    // Get checksum
    newFooter->checksum = CalcCRC16Checksum(savePtr, 53016);

    // Load Boxes block
    PCBoxes_v0 * oldBoxes = (PCBoxes_v0 *)(oldSaveData + 0xCF40);
    PCBoxes * newBoxes = (PCBoxes *)(savePtr + 0xCF2C);

    newBoxes->currentBoxID = oldBoxes->currentBoxID;
    memcpy(newBoxes->boxMons, oldBoxes->boxMons, sizeof(BoxPokemon) * MAX_PC_BOXES * MAX_MONS_PER_BOX);
    memcpy(newBoxes->names, oldBoxes->names, sizeof(BoxPokemon) * MAX_PC_BOXES * MAX_MONS_PER_BOX);
    memcpy(newBoxes->wallpapers, oldBoxes->wallpapers, sizeof(oldBoxes->wallpapers));
    newBoxes->unlockedWallpapers = oldBoxes->unlockedWallpapers;

    // Make the PC Boxes footer
    newFooter = (SaveBlockFooter*)(savePtr + 0x1F0FC);
    oldFooter = (SaveBlockFooter*)(oldSaveData + 0x1F118);

    newFooter->saveBlockID = oldFooter->saveBlockID;
    newFooter->saveCounter = oldFooter->saveCounter;
    newFooter->blockCounter = oldFooter->blockCounter;
    newFooter->size = 0x121e4;
    newFooter->signature = SECTOR_SIGNATURE;
    newFooter->checksum = CalcCRC16Checksum(savePtr + 0xCF2C, 74192);

    // Convert Battle Videos
    const int NumVideoPages = 4;
    const int videoPages[] = {36, 38, 40, 42};

    for(int i=0; i < NumVideoPages; i++) {
        u32 offset = videoPages[i] * 0x1000;
        BattleRecording_v0 * oldRecording = (BattleRecording_v0*)(oldSaveData + offset);
        BattleRecording * newRecording = (BattleRecording *)(savePtr + offset);

        newRecording->unk_00 = oldRecording->unk_00;
        memcpy(&newRecording->unk_04, &oldRecording->unk_04, sizeof(UnkStruct_02030A80));

        memcpy(newRecording->unk_84.unk_00, oldRecording->unk_84.unk_00, sizeof(newRecording->unk_84.unk_00));
        memcpy(newRecording->unk_84.unk_18, oldRecording->unk_84.unk_18, sizeof(newRecording->unk_84.unk_18));
        newRecording->unk_84.unk_24 = oldRecording->unk_84.unk_24;
        newRecording->unk_84.unk_26 = oldRecording->unk_84.unk_26;
        newRecording->unk_84.unk_27 = oldRecording->unk_84.unk_27;
        memcpy(&newRecording->unk_84.battleRegulation, &oldRecording->unk_84.battleRegulation, sizeof(BattleRegulation));
        newRecording->unk_84.unk_48 = oldRecording->unk_84.unk_48;
        memcpy(newRecording->unk_84.unk_4A, oldRecording->unk_84.unk_4A, sizeof(newRecording->unk_84.unk_4A));
        newRecording->unk_84.unk_58 = oldRecording->unk_84.unk_58;
        newRecording->unk_84.unk_60.unk_00 = CalcCRC16Checksum(&newRecording->unk_84, sizeof(UnkStruct_0202F41C) - (sizeof(UnkStruct_0202F298_sub1)) - (sizeof(u64)));
        newRecording->unk_84.unk_60.unk_02 = oldRecording->unk_84.unk_60.unk_02;
        
        memcpy(&newRecording->unk_E8, &oldRecording->unk_E8, sizeof(UnkStruct_0202F298));


        SaveCheckFooter * newVideoFooter = (SaveCheckFooter *)((uPtr)newRecording + sizeof(BattleRecording));
        SaveCheckFooter * oldVideoFooter = (SaveCheckFooter *)((uPtr)oldRecording + sizeof(BattleRecording_v0));

        newVideoFooter->signature = oldVideoFooter->signature;
        newVideoFooter->saveCounter = oldVideoFooter->saveCounter;
        newVideoFooter->size = sizeof(BattleRecording);
        newVideoFooter->id = oldVideoFooter->id;
        newVideoFooter->checksum = CalcCRC16Checksum(newRecording, sizeof(BattleRecording) + sizeof(SaveCheckFooter) - 2);
    }

    // Copy hall of fame and battle frontier
    memcpy(savePtr + (0x1000 * 32), oldSaveData + (0x1000 * 32), 0x1000 * 4);


    free(oldSaveData);
}

static BOOL ProcessSaveRegion(u8 * savePtr) {
    // Look for a SECTOR_SIGNATURE at offset 0xCF38
    u32 * savePtr32 = (u32 *)(savePtr + 0xCF38);
    if(*savePtr32 == SECTOR_SIGNATURE) {
        // Most likely this is a save file from <= 0.1.13
        // Proceed with upgrade
        UpgradeSaveFile(savePtr);
        return TRUE;
    }

    // No save data found or this is not a save file from <= 0.1.13
    return FALSE;
}



BOOL PCPORT_ProcessSaveDataUpgrades() {
    // Read entire save file from CARD
    u8 * saveBuf = malloc(sizeof(u8) * SAVE_FLASH_SIZE);
    memset(saveBuf, 0, SAVE_FLASH_SIZE);

    CARD_ReadBackup(0, saveBuf, SAVE_FLASH_SIZE);

    // Backup old save
    const char * bakPath = 
    #ifdef SDK_BUILD_PROSPERO
    "/app0/save.bin.bak"
    #else
    "save.bin.bak"
    #endif
    ;

    FILE * bakFile = fopen(bakPath, "wb");
    if(bakFile) {
        fwrite(saveBuf, 1, sizeof(u8) * SAVE_FLASH_SIZE, bakFile);
        fclose(bakFile);
    }

    // Process for main save and backup save
    BOOL mainUpgraded = ProcessSaveRegion(saveBuf + (SAVE_SECTOR_SIZE * PRIMARY_SECTOR_START));
    BOOL backupUpgraded = ProcessSaveRegion(saveBuf + (SAVE_SECTOR_SIZE * BACKUP_SECTOR_START));

    if(mainUpgraded || backupUpgraded) {
        // Write new save to CARD
        CARD_WriteBackup(0, saveBuf, SAVE_FLASH_SIZE);

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "pokeplatinum", "Save data has been upgraded to the new format. It is now fully compatible with emulators and real hardware. The game will now soft reset.", NULL);
    }

    free(saveBuf);

    return (mainUpgraded || backupUpgraded);
}