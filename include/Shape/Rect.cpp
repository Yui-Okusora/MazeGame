#include <Shape/Shape.hpp>

Rect::Rect() {}

Rect::Rect(const Rect& src)
    : Shape(src)
{
    color = src.color;
    atlas = src.atlas;
    atlasPos = src.atlasPos;
    flip = src.flip;
    noTexture = src.noTexture;
}

Rect::Rect(gl2d::Rect rect, gl2d::Color4f _color, gl2d::TextureAtlas _atlas)
{
    pos = { rect.x, rect.y };
    size = { rect.z, rect.w };
    color = _color;
    atlas = _atlas;
    noTexture = true;
}

Rect::Rect(gl2d::Rect rect, gl2d::Texture _texture, gl2d::Color4f _color, gl2d::TextureAtlas _atlas)
{
    pos = { rect.x, rect.y };
    size = { rect.z, rect.w };
    color = _color;
    atlas = _atlas;
    texture = _texture;
    noTexture = false;
}

void Rect::render(gl2d::Renderer2D* renderer)
{
    if (!noTexture)
        renderer->renderRectangle({ pos, size }, texture, color, {}, 0, atlas.get(atlasPos.x, atlasPos.y, flip));
    else
        renderer->renderRectangle({ pos, size }, color, {}, 0);
}