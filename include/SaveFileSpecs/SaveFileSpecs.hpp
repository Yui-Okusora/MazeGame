#pragma once

#include <Core.hpp>
#include <GameplayData/GameplayData.hpp>

struct SaveData
{
    GameplayData data;
    int32_t crc;
};

struct SaveFileSpecs
{
    char magic[4] = { 'M','Z','S','V' };
    int32_t reserve;
    int64_t timestamp;
    SaveData data;
    int32_t crc
};