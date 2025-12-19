#include <State/State.hpp>

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

    std::lock_guard<std::mutex> lock1(updateMutex);
    std::lock_guard<std::mutex> lock2(renderMutex);

    for (auto& query : queue)
    {
        auto it = m_activeState.begin();

        while (it->get() != query.first)
        {
            ++it;
            if (it == m_activeState.end()) break;
        }
        if (it == m_activeState.end()) continue;

        (*it)->onExit();

        m_inactiveState[(*it)->label] = std::move(*it);

        *it = std::move(m_inactiveState[query.second]);

        (*it)->onEnter();

        m_inactiveState.erase(query.second);
    }

    queue.clear();
}