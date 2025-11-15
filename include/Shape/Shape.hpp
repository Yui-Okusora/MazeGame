#pragma once

#include <Core.hpp>

class Shape
{
public:
    virtual void render(gl2d::Renderer2D* renderer) = 0;
    glm::vec2 pos = { 0, 0 };
    glm::vec2 size = { 1, 1 };
    gl2d::Texture texture;
};

class Rect : public Shape
{
public:
    Rect(gl2d::Rect rect, gl2d::TextureAtlas _atlas) : atlas(_atlas)
    {
        pos = { rect.x, rect.y };
        size = { rect.z, rect.w };
    }
    void render(gl2d::Renderer2D* renderer)
    {
        renderer->renderRectangle({pos, size}, texture, Colors_White, {}, 0, atlas.get(atlasPos.x, atlasPos.y, flip));
    }

    gl2d::TextureAtlas atlas;
    glm::uvec2 atlasPos = { 0, 0 };
    bool flip = false;
    bool noTexture = false;
};

class Maze : public Shape
{
public:
    Maze() : atlas(15, 1)
    {
        pos = { 0, 0 };
        size = { tileSize.x * mazeSize.x, tileSize.y * mazeSize.y };
    }
    void render(gl2d::Renderer2D* renderer)
    {
        for (int i = 0; i < mazeSize.y; ++i)
        {
            for (int j = 0; j < mazeSize.x; ++j)
            {
                glm::ivec2 encodePos = glm::ivec2(j, i) * 2 + 1;
                int encodeWidth = mazeSize.x * 2 + 1;
                int tileType = 0;
                tileType += mazeEncode[encodePos.x + encodeWidth * (encodePos.y - 1)] * 8;
                tileType += mazeEncode[encodePos.x + encodeWidth * encodePos.y - 1]   * 4;
                tileType += mazeEncode[encodePos.x + encodeWidth * (encodePos.y + 1)] * 2;
                tileType += mazeEncode[encodePos.x + encodeWidth * encodePos.y + 1];

                auto vec = atlas.get(tileType - 1, 0);

                renderer->renderRectangle({pos + (tileSize * glm::vec2(j, i)), tileSize}, texture, Colors_White, {}, 0, vec);
            }
        }
    }

    glm::vec2 tileSize = { 64, 64 };
    glm::vec2 mazeSize = { 8, 8 };
    gl2d::TextureAtlas atlas;
    std::vector<int> mazeEncode;
};