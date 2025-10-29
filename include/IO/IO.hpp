#pragma once

#include <Core.hpp>
#include <Application/IApplication.hpp>

class IO
{
public:
    IO(IApplication* _app);
    void operator()();
private:
    IApplication* app = nullptr;
};