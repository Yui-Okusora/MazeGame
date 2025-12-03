#pragma once

#include <Core.hpp>
#include <Shape/Shape.hpp>

struct RenderData
{
    RenderData() = default;

    RenderData(const RenderData& src)
    {
        cloneFrom(src);
    }

    RenderData& operator=(const RenderData& src)
    {
        cloneFrom(src);
        return *this;
    }

    RenderData(RenderData&&) = delete;
    RenderData& operator=(RenderData&&) = delete;

    template<typename T, typename... Args>
    T* addShape(Args&&... args)
    {
        T* ptr = arena.make<T>(std::forward<Args>(args)...);
        shapes.push_back(ptr);
        return ptr;
    }

    void cloneFrom(const RenderData& src) {
        shapes.clear();
        shapes.reserve(src.shapes.size());
        arena.reset();
        for (Shape* s : src.shapes)
        {
            shapes.push_back(s->cloneToArena(arena));
        }
    }

    std::vector<Shape*> shapes;

    Arena arena;
};

