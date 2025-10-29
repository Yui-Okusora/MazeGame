#include <Processor/Processor.hpp>

Processor::Processor(IApplication* _app) : app(_app)
{

}

void Processor::operator()()
{
    float lastTime = app->getTime();

    while (app->getRunningStat())
    {
        float currentTime = app->getTime();
        float timestep = std::clamp(currentTime - lastTime, 0.001f, 0.1f);
        lastTime = currentTime;
        
    }
}