#pragma once

#include <Core.hpp>
#include <Shape/Shape.hpp>

struct RenderData
{
    RenderData() = default;

    RenderData(const RenderData&) = delete;
    RenderData& operator=(const RenderData&) = delete;

    RenderData(RenderData&&) = default;
    RenderData& operator=(RenderData&&) = default;

    std::vector<Shape*> shapes;

    Arena arena;

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
};

