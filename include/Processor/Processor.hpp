#pragma once

#include <Core.hpp>
#include <Application/IApplication.hpp>

class Processor
{
public:
    Processor(IApplication* _app);
    void operator()();
private:
    IApplication* app = nullptr;
};