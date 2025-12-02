#pragma once

#include <Core.hpp>

class Shape
{
public:
    Shape() = default;
    Shape(const Shape&) = default;

    virtual void render(gl2d::Renderer2D* renderer) = 0;
    virtual Shape* cloneToArena(Arena& a) const = 0;
    glm::vec2 pos = { 0, 0 };
    glm::vec2 size = { 1, 1 };
    gl2d::Texture texture;
    gl2d::Color4f color = Colors_White;
    gl2d::TextureAtlas atlas = { 1, 1 };
    glm::uvec2 atlasPos = { 0, 0 };
    bool flip = false;
    bool noTexture = false;
};

class Rect : public Shape
{
public:
    Rect();
    Rect(const Rect& src);
    Rect(gl2d::Rect rect, gl2d::Color4f _color = Colors_White, gl2d::TextureAtlas _atlas = { 1, 1 });
    Rect(gl2d::Rect rect, gl2d::Texture _texture, gl2d::Color4f _color = Colors_White, gl2d::TextureAtlas _atlas = { 1, 1 });

    void render(gl2d::Renderer2D* renderer) override;

    Shape* cloneToArena(Arena& a) const override { return a.make<Rect>(*this); }
};

class Maze : public Shape
{
public:
    Maze();
    Maze(const Maze& src);
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