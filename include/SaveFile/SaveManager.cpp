#include <SaveFile/SaveManager.hpp>

SaveManager::SaveManager()
{
    crc.init();
}

uint64_t SaveManager::computeHeaderCRC(SaveFileSpecs header)
{
    crc.reset64();
    return crc.appendCRC64(
        reinterpret_cast<uint8_t*>(&header),
        sizeof(header)
    );
}

uint64_t SaveManager::computeDataCRC(const std::vector<uint8_t>& buf)
{
    size_t size = buf.size();

    crc.reset64();
    uint64_t c = crc.appendCRC64(
        reinterpret_cast<uint8_t*>(&size),
        sizeof(size)
    );
    c = crc.appendCRC64(buf.data(), size);
    return c;
}