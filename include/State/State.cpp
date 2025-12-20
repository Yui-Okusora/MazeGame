#include <State/State.hpp>

void StateStack::handleInput(const KeyInputState& in)
{
    std::lock_guard<std::mutex> lock(inputMutex);
    for (auto& t : m_activeState)
    {
        if (!t) continue;
        if (!t->suspendInput) t->handleInput(in);
    }
}

void StateStack::update(double dt)
{
    std::lock_guard<std::mutex> lock(updateMutex);
    for (auto& t : m_activeState)
    {
        if (!t) continue;
        if (t->suspendUpdate) continue;
        t->update(dt);
    }
}

void StateStack::render()
{
    std::lock_guard<std::mutex> lock(renderMutex);
    for (auto& t : m_activeState)
    {
        if (!t) continue;
        if (t->suspendRender) continue;
        t->render();
    }
}

void StateStack::processTransit()
{
    if (queue.empty()) return;

    std::lock_guard<std::mutex> lock1(inputMutex);
    std::lock_guard<std::mutex> lock2(updateMutex);
    std::lock_guard<std::mutex> lock3(renderMutex);

    for (auto& query : queue) // query (State*, target label)
    {
        auto it = m_activeState.begin();

        while (it->get() != query.first)
        {
            ++it;
            if (it == m_activeState.end()) break;
        }
        if (it == m_activeState.end()) continue;

        if (*it)
        {
            (*it)->onExit();

            m_inactiveState[(*it)->label] = std::move(*it);
        }
        
        if (query.second != "")
        {
            *it = std::move(m_inactiveState[query.second]);

            (*it)->onEnter();

            m_inactiveState.erase(query.second);
        }
    }

    queue.clear();
}

void StateStack::processSuspend()
{
    if (suspendQueue.empty()) return;

    std::lock_guard<std::mutex> lock1(inputMutex);
    std::lock_guard<std::mutex> lock2(updateMutex);
    std::lock_guard<std::mutex> lock3(renderMutex);

    for (auto& query : suspendQueue)
    {
        query.first->suspendUpdate = query.second & (1 << 2);
        query.first->suspendRender = query.second & (1 << 1);
        query.first->suspendInput = query.second & 1;
    }

    suspendQueue.clear();
}