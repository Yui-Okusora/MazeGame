#pragma once
#include <Core.hpp>
#include <Window/Window.hpp>
#include <State/State.hpp>

struct ApplicationSpecs
{
	std::string appTitle = "Game";
	WindowSpecs windowsSpecs;
};

class Application
{
public:
	Application(const ApplicationSpecs& specs = ApplicationSpecs());
	~Application();

	void run();
	void stop();

	template<typename T>
		requires(std::is_base_of_v<State, T>)
	void pushState()
	{
		m_stateStack.push_back(std::make_unique<T>());
	}

	glm::vec2 getFramebufferSize() const;

	float getTime();

private:
	ApplicationSpecs m_specs;
	std::shared_ptr<Window> m_window;
	bool m_running = false;

	std::chrono::steady_clock::time_point m_startTimePoint;

	std::vector<std::unique_ptr<State>> m_stateStack;
};