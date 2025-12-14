#pragma once

#include <vector>

struct Destructor {
    void (*destruct)(void*);
    void* ptr;
};

template<typename T>
void destruct_fn(void* p) {
    static_cast<T*>(p)->~T();
}

class Arena
{
public:
    explicit Arena(size_t _capacity = 1 << 20) : capacity(_capacity)
    { // default 1MB
        buf.resize(capacity);
        offset = 0;
    }

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    Arena(Arena&&) = delete;
    Arena& operator=(Arena&& src) noexcept
    {
        if (this != &src)
        {
            reset();
            buf = std::move(src.buf);
            destructors = std::move(src.destructors);
            offset = src.offset;
            capacity = src.capacity;

            src.offset = 0;
            src.capacity = 0;
        }
        return *this;
    }

    ~Arena() { reset(); }

    void clear()
    {
        reset();
        buf.~vector();
    }

    void resize(size_t size)
    {
        buf.resize(size);
    }

    void reset()
    {
        runDestructors();
        offset = 0;
    }

    size_t size() { return offset; }

    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        size_t cur = (size_t)buf.data() + offset;
        size_t aligned = (cur + (alignment - 1)) & ~(alignment - 1);
        size_t newOffset = (aligned - (size_t)buf.data()) + size;

        void* ptr = buf.data() + (aligned - (size_t)buf.data());
        offset = newOffset;
        return ptr;
    }

    template<typename T, typename... Args>
    T* make(Args&&... args) {
        void* mem = allocate(sizeof(T), alignof(T));

        T* obj = new (mem) T(std::forward<Args>(args)...);

        registerDestr(obj);
        return obj;
    }

    template<typename T>
    void registerDestr(T* obj)
    {
        destructors.push_back({ [](void* p) {
            static_cast<T*>(p)->~T();
            }, obj
         });
    }

private:
    void runDestructors()
    {
        for (auto it = destructors.rbegin(); it != destructors.rend(); ++it)
        {
            it->destruct(it->ptr);
        }
        destructors.clear();
    }

    std::vector<uint8_t> buf;
    std::vector<Destructor> destructors;
    size_t offset = 0;
    size_t capacity = 0;
};