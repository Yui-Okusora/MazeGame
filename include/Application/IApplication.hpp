#pragma once
#include <Core.hpp>
#include <GameplayData/GameplayData.hpp>
#include <RenderData/RenderData.hpp>
#include <InputEvent/InputEvent.hpp>
#include <Window/Window.hpp>
#include <AssetsLoader/AssetsLoader.hpp>

class IApplication
{
public:
    using clock = std::chrono::steady_clock;

    virtual ~IApplication() = default;

    //Command application to stop
    virtual void stop() = 0;

    //Get window client size
    virtual glm::vec2 getFramebufferSize() const = 0;

    //Get flag for main loop running
    virtual bool getRunningStat() = 0;

    //Get time point
    virtual double getTime() = 0;
    
    //Get input buffer for consuming
    virtual CircularBuffer<InputEvent>& getInputBuffer() = 0;

    //Get render buffer for consuming
    virtual DoubleBuffer<GameplayData>& getRenderBuffer() = 0;
};