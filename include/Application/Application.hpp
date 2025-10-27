#pragma once
#include <Core.hpp>
#include <Window/Window.hpp>

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

private:
	ApplicationSpecs m_specs;
	std::shared_ptr<Window> m_window;
	bool m_running = false;
};