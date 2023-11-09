#pragma once
#include <opus.h>

typedef enum 
{
    CellDirectionDown = 0,
    CellDirectionLeft,
    CellDirectionRight,
    CellDirectionUp,
    CellDirectionDownLeft,
    CellDirectionDownRight,
    CellDirectionUpLeft,
    CellDirectionUpRight,
    CellDirectionCount,
} CellDirection;

typedef enum
{
    CellMaterialTypeAir = 0,
    CellMaterialTypeSand,
    CellMaterialTypeWater,
    CellMaterialTypeDirt,
    CellMaterialTypeCount
} CellMaterialType;

typedef enum
{
    MatterStateTypeSolid = 0,
    MatterStateTypeLiquid,
    MatterStateTypeGas,
    MatterStateTypeCount
} MatterStateType;

const Vec4 CellMaterialColors[] =
{
    { .r = 0xc4 / 255., .g = 0xce / 255., .b = 0xd6 / 255., .a = 0xff / 255. }, // Air
    { .r = 0xae / 255., .g = 0x8f / 255., .b = 0x60 / 255., .a = 0xff / 255. }, // Sand
    { .r = 0x80 / 255., .g = 0xc5 / 255., .b = 0xde / 255., .a = 0xff / 255. }, // Water
};

const uint8 MatterDensities[] =
{
    8,  // Air
    20, // Sand
    10, // Water
    20, // Dirt
};

/**                                         D  L  R  U  DL DR UL UR */
const uint8 MatterMovementRulesSolid[]  = { 1, 0, 0, 0,  1, 1, 0, 0 };
const uint8 MatterMovementRulesLiquid[] = { 1, 1, 1, 0,  1, 1, 0, 0 };
const uint8 MatterMovementRulesGas[]    = { 1, 1, 1, 0,  1, 1, 0, 0 };

const uint8 MatterStates[] = 
{
    MatterStateTypeGas,  // Air
    MatterStateTypeSolid, // Sand
    MatterStateTypeLiquid, // Water
    MatterStateTypeSolid, // Dirt
};

typedef struct
{
    uint8 type;
} Cell;