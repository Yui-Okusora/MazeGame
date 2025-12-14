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
        SaveFileSpecs header;

        auto now = std::chrono::system_clock::now();
        auto duration_since_epoch = now.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);

        header.timestamp = milliseconds.count();

        crc.reset64();

        uint64_t headerCRC = crc.appendCRC64((uint8_t*)&header, sizeof(header));

        crc.reset64();

        std::vector<uint8_t> buf;
        data.write(buf);
        size_t dataSize = buf.size();
        
        uint64_t dataCRC = crc.appendCRC64((uint8_t*)&dataSize, sizeof(dataSize));

        dataCRC = crc.appendCRC64(buf.data(), dataSize);

        crc.getCRC64() = CRC32_64::combineCRC64(headerCRC, dataCRC, dataSize + sizeof(dataSize));
        crc.finalize64();
        
        header.crc = crc.getCRC64();

        crc.reset64();

        file.open("save.txt", std::ios::binary | std::ios::out | std::ios::trunc);

        file.write((char*)&header, sizeof(header));
        file.write((char*)&dataSize, sizeof(dataSize));
        file.write((char*)buf.data(), dataSize);

        file.close();
    }

    GameplayData load()
    {
        file.open("save.txt", std::ios::binary | std::ios::in);

        if (!file.is_open())
        {
            throw std::runtime_error("Invalid save file");
        }

        SaveFileSpecs header;
        GameplayData data;
        size_t dataSize;
        std::vector<uint8_t> buf;

        file.read((char*)&header, sizeof(header));
        file.read((char*)&dataSize, sizeof(dataSize));

        buf.resize(dataSize);

        file.read((char*)buf.data(), dataSize);

        file.close();

        uint64_t allCRC = header.crc;
        header.crc = 0;

        crc.reset64();

        uint64_t headerCRC = crc.appendCRC64((uint8_t*)&header, sizeof(header));

        crc.reset64();

        uint64_t dataCRC = crc.appendCRC64((uint8_t*)&dataSize, sizeof(dataSize));
        dataCRC = crc.appendCRC64(buf.data(), dataSize);

        data.read(buf);

        crc.getCRC64() = CRC32_64::combineCRC64(headerCRC, dataCRC, dataSize + sizeof(dataSize));
        crc.finalize64();
        
        if (allCRC != crc.getCRC64())
        {
            throw std::runtime_error("Invalid save file");
        }

        crc.reset64();

        return data;
    }

    std::fstream file;
    CRC32_64 crc;
};