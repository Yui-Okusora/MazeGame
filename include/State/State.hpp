#pragma once

#include <Core.hpp>
#include <Event/Event.hpp>

class State
{
public:
	virtual ~State() = default;

	virtual void OnEvent(Event& event) {}

	virtual void OnUpdate(float ts) {}
	virtual void OnRender() {}
};