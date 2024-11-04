#include "./MyAction.h"
#include <Tracer/Trace.h>
#include <stdio.h>
MyAction::MyAction(): isFinishing_(false)
{
}

MyAction::~MyAction()
{
}

Gbp::Mt::ThreadFunctionReturn_t MyAction::threadFunc()
{
	int i = 0;
	for(;!isFinishing_;)
	{
		Sleep(100);
		printf(".");
		TRACEF(10, "loop counter, i=%d", ++i);
	}
	return 0;
}

