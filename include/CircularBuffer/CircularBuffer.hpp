#pragma once

#include <atomic>
#include <vector>
#include <stdexcept>

template<typename T>
class CircularBuffer
{
public:
    explicit CircularBuffer(size_t capacity)
        : buffer(capacity), capacity(capacity), head(0), tail(0), full(false)
    {}

    bool empty() const noexcept { return (!full && head == tail); }
    bool fullBuffer() const noexcept { return full; }

    size_t max_size() const noexcept { return capacity; }

    size_t size() const noexcept
    {
        if (full)
            return capacity;
        if (tail >= head)
            return tail - head;
        return capacity - head + tail;
    }

    void clear() noexcept
    {
        head = tail = 0;
        full = false;
    }

    void push(const T& item)
    {
        buffer[tail] = item;
        advance(tail);
        if (full)
            advance(head);
        full = (head == tail);
    }

    void push(T&& item)
    {
        buffer[tail] = std::move(item);
        advance(tail);
        if (full)
            advance(head);
        full = (head == tail);
    }

    T pop()
    {
        if (empty())
            throw std::runtime_error("CircularBuffer is empty");
        auto val = std::move(buffer[head]);
        advance(head);
        full = false;
        return val;
    }

    const T& front() const
    {
        if (empty())
            throw std::runtime_error("CircularBuffer is empty");
        return buffer[head];
    }

    const T& back() const
    {
        if (empty())
            throw std::runtime_error("CircularBuffer is empty");
        return buffer[(tail + capacity - 1) % capacity];
    }

private:
    void advance(size_t& index) noexcept
    {
        index = (index + 1) % capacity;
    }

private:
    std::vector<T> buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    bool full;
};