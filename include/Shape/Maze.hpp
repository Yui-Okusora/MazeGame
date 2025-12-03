#pragma once

#include <Shape/Shape.hpp>

class Maze : public Shape
{
public:
    Maze();
    explicit Maze(const Maze&) noexcept;
    explicit Maze(Maze&&) noexcept;
    Maze(glm::vec2 _pos, glm::vec2 _mazeSize, glm::vec2 _tileSize);

    ~Maze()
    {
        mazeEncode.~vector();
    }

    void render(gl2d::Renderer2D* renderer) override;

    Shape* cloneToArena(Arena& a) const override { return a.make<Maze>(*this); }

    glm::vec2 tileSize = { 64, 64 };
    glm::vec2 mazeSize = { 8, 8 };
    glm::vec2 encodeSize = mazeSize * 2.0f + 1.0f;
    std::vector<int> mazeEncode;
};