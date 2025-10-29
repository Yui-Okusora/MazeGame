#pragma once
#include <Core.hpp>
#include <GameplayData/GameplayData.hpp>
#include <RenderData/RenderData.hpp>
#include <InputEvent/InputEvent.hpp>
#include <Window/Window.hpp>

class IApplication
{
public:
    virtual ~IApplication() = default;

    virtual glm::vec2 getFramebufferSize() const = 0;

    virtual bool getRunningStat() = 0;

    virtual float getTime() = 0;
    virtual void stop() = 0;
    virtual CircularBuffer<InputEvent>& getInputBuffer() = 0;

    virtual DoubleBuffer<GameplayData>& getRenderBuffer() = 0;
};