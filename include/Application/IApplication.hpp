#pragma once
#include <Core.hpp>
#include <RenderData/RenderData.hpp>
#include <InputEvent/InputEvent.hpp>
#include <Window/Window.hpp>
#include <AssetsLoader/AssetsLoader.hpp>
#include <State/State.hpp>
#include <Utils/Utils.hpp>

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

    //Get Mouse position
    virtual MousePos getMousePos() = 0;
    
    //Get keyboard input buffer for consuming
    virtual CircularBuffer<KeyInputEvent>& getKeyInputBuffer() = 0;

    virtual CircularBuffer<MouseEvent>& getMouseKeyBuffer() = 0;

    //Get renderer
    virtual gl2d::Renderer2D& getRenderer() = 0;

    virtual StateStack& getStateStack() = 0;
};