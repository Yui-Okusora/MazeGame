#pragma once

#include <Core.hpp>
#include <AppContext/AppContext.hpp>

class IO
{
public:
	IO(AppContext* _ctx);
	void operator()();
private:
	AppContext* ctx = nullptr;
};