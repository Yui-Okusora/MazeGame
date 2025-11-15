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
};

class StateStack {
    std::vector<std::unique_ptr<State>> tracks;
public:
    void addTrack(std::unique_ptr<State> s) {
        s->onEnter();
        tracks.push_back(std::move(s));
    }

    void handleInput(const KeyInputState& in) {
        for (auto& t : tracks) t->handleInput(in);
    }

    void update(double dt) {
        for (auto& t : tracks) {
            if (t->suspendUpdate) continue;
            t->update(dt);
        }
    }

    void render() {
        for (auto& t : tracks) {
            if (t->suspendRender) continue;
            t->render();
        }
    }

    // utilities to replace/remove tracks, etc.
};