#pragma once

#include <Core.hpp>
#include <SaveFile/SaveFileSpecs.hpp>
#include <GameMechanics/GameplayData.hpp>
#include <CRC32_64/CRC32_64.hpp>

class SaveLoad
{
public:
    SaveLoad()
    {
        crc.init();
    }

    void save(GameplayData& data)
    {
        crc.reset64();

        

        uint64_t dataCRC = crc.appendCRC64((uint8_t*)&data, sizeof(data));

        crc.reset64();

        SaveFileSpecs header;

        auto now = std::chrono::system_clock::now();
        auto duration_since_epoch = now.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);

        header.timestamp = milliseconds.count();
        
        uint64_t headerCRC = crc.appendCRC64((uint8_t*)&header, sizeof(header));

        crc.getCRC64() = CRC32_64::combineCRC64(headerCRC, dataCRC, sizeof(data));
        crc.finalize64();

        header.crc = crc.getCRC64();

        file.open("save.txt", std::ios::binary | std::ios::out | std::ios::trunc);

        file.write((char*)&header, sizeof(header));
        file.write((char*)&data, sizeof(data));

        file.close();
    }

    GameplayData load()
    {
        crc.reset64();

        file.open("save.txt", std::ios::binary | std::ios::in);

        if (!file.is_open())
        {
            throw std::runtime_error("Invalid save file");
        }

        SaveFileSpecs header;

        file.read((char*)&header, sizeof(header));

        uint64_t allCRC = header.crc;
        header.crc = 0;

        uint64_t headerCRC = crc.appendCRC64((uint8_t*)&header, sizeof(header));

        crc.reset64();

        GameplayData data;

        file.read((char*)&data, sizeof(data));

        uint64_t dataCRC = crc.appendCRC64((uint8_t*)&data, sizeof(data));

        crc.getCRC64() = CRC32_64::combineCRC64(headerCRC, dataCRC, sizeof(data));
        crc.finalize64();

        if (allCRC != crc.getCRC64())
        {
            throw std::runtime_error("Invalid save file");
        }

        file.close();

        return data;
    }

    std::fstream file;
    CRC32_64 crc;
};