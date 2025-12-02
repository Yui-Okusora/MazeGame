#pragma once

#include <Core.hpp>
#include <InputEvent/InputEvent.hpp>

class IApplication;

class State {
public:
    virtual ~State() = default;

    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void handleInput(const KeyInputState& in) {}
    virtual void update(double dt) {}
    virtual void render() {}
public:
    IApplication* app = nullptr;
    bool suspendUpdate = false;
    bool suspendRender = false;
    std::string label = "";
};

class StateStack {
public:
    void resizeStack(size_t size) { m_activeState.reserve(size); }

    void addTrack(std::unique_ptr<State> s)
    {
        s->onEnter();
        m_activeState.push_back(std::move(s));
    }

    void addInactive(std::unique_ptr<State> s)
    {
        s->onEnter();
        m_inactiveState[s->label] = std::move(s);
    }

    void handleInput(const KeyInputState& in)
    {
        for (auto& t : m_activeState)
        {
            if (!t) continue;
            t->handleInput(in);
        }
    }

    void update(double dt);

    void render();

    void queueTransit(State* current, std::string label)
    {
        queue.push_back({ current, label });
    }

    void processTransit();
private:
    std::vector<std::unique_ptr<State>> m_activeState;
    std::vector<std::pair<State*, std::string>> queue;
    std::unordered_map<std::string, std::unique_ptr<State>> m_inactiveState;
    std::mutex updateMutex;
    std::mutex renderMutex;
};