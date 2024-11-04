#pragma once
#include <Mt/ThreadTypes.h>
class MyAction
{
public:
	MyAction();
	~MyAction();
	void finish(){isFinishing_ = true;}
	Gbp::Mt::ThreadFunctionReturn_t threadFunc();
private:
	bool isFinishing_;
};
