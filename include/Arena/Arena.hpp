#pragma once

#include <vector>

struct AllocRecord {
    void* ptr;
    void (*destruct)(void*);
};

template<typename T>
void destruct_fn(void* p) {
    static_cast<T*>(p)->~T();
}

class Arena
{
public:
    explicit Arena(size_t capacity = 1 << 20) { // default 1MB
        buf.resize(capacity);
        offset = 0;
    }

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    Arena(Arena&&) = default;
    Arena& operator=(Arena&&) = default;

    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        size_t cur = (size_t)buf.data() + offset;
        size_t aligned = (cur + (alignment - 1)) & ~(alignment - 1);
        size_t newOffset = (aligned - (size_t)buf.data()) + size;

        void* ptr = buf.data() + (aligned - (size_t)buf.data());
        offset = newOffset;
        return ptr;
    }

    void reset() 
    { 
        for (auto& rec : arenaObjects) {
            rec.destruct(rec.ptr);
        }
        arenaObjects.clear();
        offset = 0;
    }

    size_t size() { return offset; }
    size_t capacity() { return buf.capacity(); }

    template<typename T, typename... Args>
    T* make(Args&&... args) {
        void* mem = allocate(sizeof(T), alignof(T));

        T* obj = new (mem) T(std::forward<Args>(args)...);

        arenaObjects.push_back({ obj, &destruct_fn<T> });
        return obj;
    }

private:
    std::vector<uint8_t> buf;
    std::vector<AllocRecord> arenaObjects;
    size_t offset = 0;
};