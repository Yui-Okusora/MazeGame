#pragma once

#include <Core.hpp>

struct GameplayData
{
    GameplayData(){}

    double time = 0;
    uint64_t timestamp = 0;
    int score = 0;
    int maxScore = 0;
    int levelMax = 0;
    int level = 0;
    int difficulty = 0; // 0 normal 1 dfs
    int wlState = 0; // 0 normal 1 win 2 lose

    glm::vec2 mazeSize = {8, 8};
    glm::vec2 playerPos = {};
    glm::vec2 startPos = {};
    glm::vec2 goalPos = {4, 7};

    std::vector<int> mazeEncode;

    std::vector<int> enemyMaxStep;
    std::vector<int> enemyStep;

    std::vector<glm::vec2> enemyPos = {};
    std::vector<glm::vec2> enemyStartPos = {};
    std::vector<std::vector<std::pair<int, int>>> enemyPath;

    std::vector<glm::vec2> stepHistory;
    std::vector<std::vector<std::vector<glm::vec2>>> enemyStepHistory;

    std::string name = "";
    bool changed = false;
    char saveloadMode = 0;

    size_t write(std::vector<uint8_t>& buf) const
    {
        size_t size = 0;
        size += writeValue(buf, time);
        size += writeValue(buf, timestamp);
        size += writeValue(buf, score);
        size += writeValue(buf, maxScore);
        size += writeValue(buf, levelMax);
        size += writeValue(buf, level);
        size += writeValue(buf, difficulty);
        size += writeValue(buf, wlState);

        size += writeValue(buf, mazeSize);
        size += writeValue(buf, playerPos);
        size += writeValue(buf, startPos);
        size += writeValue(buf, goalPos);

        size += writeValue(buf, mazeEncode);
        size += writeValue(buf, enemyMaxStep);
        size += writeValue(buf, enemyStep);

        size += writeValue(buf, enemyPos);
        size += writeValue(buf, enemyStartPos);
        size += writeValue(buf, enemyPath);
        size += writeValue(buf, stepHistory);
        size += writeValue(buf, enemyStepHistory);

        size += writeValue(buf, name);
        size += writeValue(buf, changed);
        return size;
    }
    
    void read(std::vector<uint8_t>& buf)
    {
        size_t offset = 0;
        readValue(buf, offset, time);
        readValue(buf, offset, timestamp);
        readValue(buf, offset, score);
        readValue(buf, offset, maxScore);
        readValue(buf, offset, levelMax);
        readValue(buf, offset, level);
        readValue(buf, offset, difficulty);
        readValue(buf, offset, wlState);

        readValue(buf, offset, mazeSize);
        readValue(buf, offset, playerPos);
        readValue(buf, offset, startPos);
        readValue(buf, offset, goalPos);

        readValue(buf, offset, mazeEncode);
        readValue(buf, offset, enemyMaxStep);
        readValue(buf, offset, enemyStep);

        readValue(buf, offset, enemyPos);
        readValue(buf, offset, enemyStartPos);
        readValue(buf, offset, enemyPath);
        readValue(buf, offset, stepHistory);
        readValue(buf, offset, enemyStepHistory);

        readValue(buf, offset, name);
        readValue(buf, offset, changed);
    }
private:
    template<typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, size_t>
    writeValue(std::vector<uint8_t>& buf, const T& val) const
    {
        const uint8_t* p = reinterpret_cast<const uint8_t*>(&val);
        buf.insert(buf.end(), p, p + sizeof(T));
        return sizeof(T);
    }

    template<typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, size_t>
    writeValue(std::vector<uint8_t>& buf, const std::vector<T>& val) const
    {
        size_t size = val.size();
        writeValue(buf, size);

        if (size > 0)
        {
            const uint8_t* p = reinterpret_cast<const uint8_t*>(val.data());
            buf.insert(buf.end(), p, p + sizeof(T) * size);
        }

        return sizeof(T) * size + sizeof(size);
    }

    template<typename T>
    std::enable_if_t<!std::is_trivially_copyable_v<T>, size_t>
    writeValue(std::vector<uint8_t>& buf, const std::vector<T>& val) const
    {
        size_t size = val.size();
        size_t outputSize = writeValue(buf, size);

        for (int i = 0; i < size; ++i)
            outputSize += writeValue(buf, val[i]);

        return outputSize;
    }

    size_t writeValue(std::vector<uint8_t>& buf, const std::string& str) const
    {
        size_t size = str.size();
        writeValue(buf, size);

        if (size > 0)
        {
            const uint8_t* p = reinterpret_cast<const uint8_t*>(str.data());
            buf.insert(buf.end(), p, p + size);
        }

        return size + sizeof(size);
    }

    template<typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, void>
    readValue(const std::vector<uint8_t>& buf, size_t& offset, T& val)
    {
        if (offset + sizeof(T) > buf.size()) throw std::runtime_error("Buffer underflow");
        std::memcpy(&val, buf.data() + offset, sizeof(T));
        offset += sizeof(T);
    }

    template<typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, void>
    readValue(const std::vector<uint8_t>& buf, size_t& offset, std::vector<T>& val)
    {
        size_t size;
        readValue(buf, offset, size);

        if (offset + sizeof(T) * size > buf.size()) throw std::runtime_error("Buffer underflow");

        val.resize(size);
        std::memcpy(val.data(), buf.data() + offset, sizeof(T) * size);
        offset += sizeof(T) * size;
    }

    template<typename T>
    std::enable_if_t<!std::is_trivially_copyable_v<T>, void>
    readValue(const std::vector<uint8_t>& buf, size_t& offset, std::vector<T>& val)
    {
        size_t outerSize;
        readValue(buf, offset, outerSize);

        val.resize(outerSize);

        for (size_t i = 0; i < outerSize; ++i)
        {
            readValue(buf, offset, val[i]);
        }
    }

    void readValue(const std::vector<uint8_t>& buf, size_t& offset, std::string& str)
    {
        size_t size;
        readValue(buf, offset, size);

        if (offset + size > buf.size()) throw std::runtime_error("Buffer underflow");

        str.resize(size);
        std::memcpy(str.data(), buf.data() + offset, size);
        offset += size;
    }
};