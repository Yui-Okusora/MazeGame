#include <Shape/Shape.hpp>

Maze::Maze() {}

Maze::Maze(const Maze& src) : Shape(src)
{
    tileSize = src.tileSize;
    mazeSize = src.mazeSize;
    encodeSize = src.encodeSize;
    mazeEncode.resize(src.mazeEncode.size());
    std::copy(src.mazeEncode.begin(), src.mazeEncode.begin() + src.mazeEncode.size(), mazeEncode.begin());
}

Maze::Maze(glm::vec2 _pos, glm::vec2 _mazeSize, glm::vec2 _tileSize)
    : mazeSize(_mazeSize), tileSize(_tileSize)
{
    pos = _pos;
    size = { tileSize.x * mazeSize.x, tileSize.y * mazeSize.y };
    atlas = { 15, 1 };
}

void Maze::render(gl2d::Renderer2D* renderer)
{
    for (int i = 0; i < mazeSize.y; ++i)
    {
        for (int j = 0; j < mazeSize.x; ++j)
        {
            glm::ivec2 encodePos = glm::ivec2(j, i) * 2 + 1;
            int encodeWidth = mazeSize.x * 2 + 1;
            int tileType = 0;
            tileType += mazeEncode[encodePos.x + encodeWidth * (encodePos.y - 1)] * 8;
            tileType += mazeEncode[encodePos.x + encodeWidth * encodePos.y - 1] * 4;
            tileType += mazeEncode[encodePos.x + encodeWidth * (encodePos.y + 1)] * 2;
            tileType += mazeEncode[encodePos.x + encodeWidth * encodePos.y + 1];

            auto vec = atlas.get(tileType - 1, 0);

            renderer->renderRectangle({ pos + (tileSize * glm::vec2(j, i)), tileSize }, texture, Colors_White, {}, 0, vec);
        }
    }
}