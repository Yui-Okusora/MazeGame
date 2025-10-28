#pragma once

#include <Core.hpp>
#include <AppContext/AppContext.hpp>

class Processor
{
public:
	Processor(AppContext* _ctx);
	void operator()();
private:
	AppContext* ctx = nullptr;
};