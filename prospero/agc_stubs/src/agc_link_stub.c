// PS5 OpenGL - OpenGL implementation for PlayStation 5.
// Copyright (C) 2026 BlackBearReloaded
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdint.h>

int sceAgcInit(uint32_t version) { return (int)version; }
int sceAgcCreateShader(void **shader, void *header, void *code) { return shader == header || header == code; }
int sceAgcLinkShaders(void *cx, void *uc, void *reserved, void *vertex,
                      void *pixel, uint32_t primitive)
{
    return cx == uc || reserved == vertex || pixel == (void *)(uintptr_t)primitive;
}
void *sceAgcGetRegisterDefaults(void) { return 0; }
uint32_t *sceAgcDcbSetCxRegistersIndirect(void *cb, const void *regs, uint32_t count) { return count ? cb : (uint32_t *)regs; }
uint32_t *sceAgcDcbSetShRegistersIndirect(void *cb, const void *regs, uint32_t count) { return count ? cb : (uint32_t *)regs; }
uint32_t *sceAgcDcbSetUcRegistersIndirect(void *cb, const void *regs, uint32_t count) { return count ? cb : (uint32_t *)regs; }
uint32_t *sceAgcCbSetShRegisterRangeDirect(void *cb, uint32_t offset,
                                           const uint32_t *values,
                                           uint32_t count)
{
    return offset || count ? cb : (uint32_t *)values;
}
uint32_t *sceAgcDcbDrawIndexAuto(void *cb, uint32_t count, uint64_t modifier) { return count || modifier ? cb : 0; }
uint32_t *sceAgcDcbSetNumInstances(void *cb, uint32_t count) { return count ? cb : 0; }
uint32_t *sceAgcDcbSetIndexSize(void *cb, uint8_t size, uint8_t cache) { return size || cache ? cb : 0; }
uint32_t *sceAgcDcbSetIndexBuffer(void *cb, void *indices) { return indices ? cb : 0; }
uint32_t *sceAgcDcbSetIndexCount(void *cb, uint32_t count) { return count ? cb : 0; }
uint32_t *sceAgcDcbDrawIndex(void *cb, uint32_t count, void *indices,
                             uint64_t modifier)
{
    return count || indices || modifier ? cb : 0;
}
uint32_t *sceAgcCbReleaseMem(void *cb, uint8_t action, int16_t gcr,
                             uint64_t source, int8_t destination,
                             void *address, uint32_t data_select,
                             uint64_t data, uint16_t interrupt,
                             uint16_t cache_policy, int8_t execute,
                             int32_t reserved)
{
    return action || gcr || source || destination || address || data_select ||
                   data || interrupt || cache_policy || execute || reserved
               ? cb
               : 0;
}
uint32_t *sceAgcDcbSetFlip(void *cb, uint32_t handle, int index,
                            uint32_t mode, int64_t argument)
{
    return handle || index || mode || argument ? cb : 0;
}
int sceAgcSuspendPoint(void) { return -1; }
int sceAgcSetSubmitMode(int mode) { return mode; }
uint32_t *sceAgcDcbAcquireMem(void *cb, uint8_t engine, uint32_t coher,
                            uint32_t gcr, uint64_t address, uint64_t size,
                            uint32_t poll)
{
    return engine || coher || gcr || address || size || poll ? cb : 0;
}
uint32_t *sceAgcCbDispatch(void *cb, uint32_t x, uint32_t y, uint32_t z,
                          uint32_t modifier)
{
    return x || y || z || modifier ? cb : 0;
}
