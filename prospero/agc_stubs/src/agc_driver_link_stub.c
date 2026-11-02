// PS5 OpenGL - OpenGL implementation for PlayStation 5.
// Copyright (C) 2026 BlackBearReloaded
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdint.h>

uint32_t sceAgcDriverGetWaitRenderingPacketSizeInDwords(void) { return 0; }
uint32_t sceAgcDriverWaitUntilSafeForRendering(uint32_t **command,
                                               uint32_t video_handle,
                                               uint32_t buffer_index,
                                               uint32_t generation,
                                               int mode)
{
    return command != 0 || video_handle || buffer_index || generation || mode;
}
int sceAgcDriverSubmitDcb(void *description) { return description ? -1 : 0; }
int sceAgcDriverSetTFRing(uintptr_t address, uint32_t size)
{
    return address && size ? 0 : -1;
}
int sceAgcDriverGetTFRing(uintptr_t *address, uint32_t *size)
{
    return address && size ? 0 : -1;
}
