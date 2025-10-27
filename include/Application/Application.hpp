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
	Application(const ApplicationSpecs& specification = ApplicationSpecs());
	~Application();

	void run();
	void stop();
private:
	ApplicationSpecs m_Specs;
	std::shared_ptr<Window> m_Window;
	bool m_Running = false;

	//std::vector<std::unique_ptr<Layer>> m_LayerStack;
};