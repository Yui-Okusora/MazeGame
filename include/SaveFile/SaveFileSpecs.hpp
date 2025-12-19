#pragma once

#include <Core.hpp>

struct SaveFileSpecs
{
    char magic[4] = { 'M','Z','S','V' };
    int32_t reserve = 0;
    int64_t timestamp = 0;
    uint64_t crc = 0;
};