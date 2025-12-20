#pragma once

#include <Core.hpp>
#include <SaveFile/SaveFileSpecs.hpp>
#include <GameMechanics/GameplayData.hpp>
#include <CRC32_64/CRC32_64.hpp>

class SaveManager
{
public:
    SaveManager();

    template<typename T>
    void save(const std::string& path, T& data)
    {
        SaveFileSpecs header{};

        // timestamp
        auto now = std::chrono::system_clock::now();
        header.timestamp =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()
            ).count();
        data.timestamp = header.timestamp;

        // serialize data
        std::vector<uint8_t> buf;
        data.write(buf);

        // CRC
        header.crc = 0;

        uint64_t headerCRC = computeHeaderCRC(header);
        uint64_t dataCRC = computeDataCRC(buf);

        crc.reset64();
        crc.getCRC64() = CRC32_64::combineCRC64(
            headerCRC,
            dataCRC,
            sizeof(size_t) + buf.size()
        );
        crc.finalize64();

        header.crc = crc.getCRC64();

        // write file
        std::ofstream file;
        file.open(path, std::ios::binary | std::ios::trunc);
        if (!file)
            throw std::runtime_error("Failed to open save file");

        size_t size = buf.size();
        file.write((char*)&header, sizeof(header));
        file.write((char*)&size, sizeof(size));
        file.write((char*)buf.data(), size);
    }

    template<typename T>
    std::optional<T> load(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
            return std::nullopt;

        SaveFileSpecs header;
        size_t size;
        std::vector<uint8_t> buf;

        file.read((char*)&header, sizeof(header));
        file.read((char*)&size, sizeof(size));

        buf.resize(size);
        file.read((char*)buf.data(), size);

        // CRC validation
        uint64_t storedCRC = header.crc;
        header.crc = 0;

        uint64_t headerCRC = computeHeaderCRC(header);
        uint64_t dataCRC = computeDataCRC(buf);

        crc.reset64();
        crc.getCRC64() = CRC32_64::combineCRC64(
            headerCRC,
            dataCRC,
            sizeof(size) + size
        );
        crc.finalize64();

        if (crc.getCRC64() != storedCRC)
            return std::nullopt;

        // deserialize
        T data;
        data.read(buf);
        return data;
    }

private:
    CRC32_64 crc;

    uint64_t computeHeaderCRC(SaveFileSpecs header);
    uint64_t computeDataCRC(const std::vector<uint8_t>& buf);
};