#pragma once
#include <opus.h>

const Vec4 CellMaterialColors[] =
{
    { .r = 0xc4 / 255., .g = 0xce / 255., .b = 0xd6 / 255., .a = 0xff / 255. }, // Air
    { .r = 0xae / 255., .g = 0x8f / 255., .b = 0x60 / 255., .a = 0xff / 255. }, // Sand
    { .r = 0x80 / 255., .g = 0xc5 / 255., .b = 0xde / 255., .a = 0xff / 255. }, // Water
};

typedef enum
{
    CellMaterialTypeAir = 0,
    CellMaterialTypeSand,
    CellMaterialTypeWater,
    CellMaterialTypeCount
} CellMaterialType;

typedef struct
{
    uint8 priority;
    uint8 movement;
    uint8 type;
} Cell;