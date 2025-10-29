#pragma once
#include <Core.hpp>
#include <State/State.hpp>

class IApplication
{
public:
    virtual ~IApplication() = default;

    virtual glm::vec2 getFramebufferSize() const = 0;

    virtual bool getRunningStat() = 0;

    virtual float getTime() = 0;

    virtual DoubleBuffer<RenderData>& getRenderBuffer() = 0;
    virtual std::vector<std::unique_ptr<State>>& getStateStack() = 0;
};