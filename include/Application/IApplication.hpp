#pragma once
#include <Core.hpp>


class IApplication
{
public:
    virtual ~IApplication() = default;

    virtual glm::vec2 getFramebufferSize() const = 0;

    virtual bool getRunningStat() = 0;

    virtual float getTime() = 0;

    virtual DoubleBuffer<GameplayData>& getRenderBuffer() = 0;
};