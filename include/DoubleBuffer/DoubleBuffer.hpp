#pragma once

#include <atomic>
#include <array>

template<typename T>
class DoubleBuffer
{
public:
    T& getReadBuffer() noexcept
    {
        return buffers[readIdx.load(std::memory_order_acquire)];
    }

    T& getWriteBuffer() noexcept
    {
        return buffers[1 - readIdx.load(std::memory_order_acquire)];
    }

    void swap() noexcept
    {
        readIdx.store(1 - readIdx.load(std::memory_order_acquire), std::memory_order_release);
    }

private:
    std::atomic<int> readIdx{ 0 };
    std::array<T, 2> buffers{};
};