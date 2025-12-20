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
    std::string label = "";
    bool suspendInput = false;
    bool suspendUpdate = false;
    bool suspendRender = false;
};

class StateStack {
public:
    void resizeStack(size_t size) { m_activeState.resize(size); }

    void addTrack(std::unique_ptr<State> s)
    {
        s->onEnter();
        for (auto& a : m_activeState)
        {
            if (a == nullptr)
            {
                a = std::move(s);
                return;
            }
        }
    }

    void addInactive(std::unique_ptr<State> s)
    {
        //s->onEnter();
        m_inactiveState[s->label] = std::move(s);
    }

    void handleInput(const KeyInputState& in);

    void update(double dt);

    void render();

    State* findActiveState(std::string label)
    {
        for (auto& a : m_activeState)
        {
            if (!a) break;
            if (a->label == label) return a.get();
        }
        return nullptr;
    }

    State* findInactiveState(std::string label)
    {
        if(m_inactiveState.contains(label))
            return m_inactiveState[label].get();
        return nullptr;
    }

    void queueTransit(State* current, std::string label)
    {
        queue.push_back({ current, label });
    }

    void queueTransit(std::string label1, std::string label2)
    {
        auto state = findActiveState(label1);
        if (state)
            queue.push_back({ state, label2 });
    }

    void queueSuspend(State* current, bool _update = false, bool _render = false, bool _input = false)
    {
        if (current == nullptr) return;
        unsigned int mask = 0;
        mask |= (_update ? 1 : 0) << 2;
        mask |= (_render ? 1 : 0) << 1;
        mask |= (_input ? 1 : 0);
        suspendQueue.push_back({ current, mask });
    }

    void queueSuspend(std::string label, bool _update = false, bool _render = false, bool _input = false)
    {
        unsigned int mask = 0;
        mask |= (_update ? 1 : 0) << 2;
        mask |= (_render ? 1 : 0) << 1;
        mask |= (_input ? 1 : 0);
        State* state = findActiveState(label);
        if (!state) state = findInactiveState(label);
        if (state)
            suspendQueue.push_back({ state, mask });
    }

    void processTransit();

    void processSuspend();
private:
    std::vector<std::unique_ptr<State>> m_activeState;
    std::vector<std::pair<State*, std::string>> queue;
    std::vector<std::pair<State*, unsigned int>> suspendQueue;
    std::unordered_map<std::string, std::unique_ptr<State>> m_inactiveState;
    std::mutex inputMutex;
    std::mutex updateMutex;
    std::mutex renderMutex;
};