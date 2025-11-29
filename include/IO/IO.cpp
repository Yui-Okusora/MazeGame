#include <IO/IO.hpp>

IO::IO(IApplication* _app) : app(_app)
{

}

void IO::operator()()
{
    while (app->getRunningStat())
    {
        
    }
}