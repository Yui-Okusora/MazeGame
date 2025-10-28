#pragma once

#include <atomic>
#include <array>

template<typename T>
class DoubleBuffer
{
public:
    T& getReadBuffer()
    {
        int i = readIndex.load(std::memory_order_acquire);
        return buffers[i];
    }

    T& getWriteBuffer()
    {
        int i = readIndex.load(std::memory_order_acquire);
        return buffers[1 - i];
    }

    void swap()
    {
        int i = readIndex.load(std::memory_order_relaxed);
        readIndex.store(1 - i, std::memory_order_release);
    }

private:
    std::atomic<int> readIndex{ 0 };
    std::array<T, 2> buffers{};
};