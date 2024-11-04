#include "./MyClass.h"
#include <Tracer/Trace.h>

MyClass::MyClass(): n_(0)
{
	n_ = 1;
	TRACEF(10, "create");
}

MyClass::~MyClass()
{
	TRACEF(10, "destroy");
	n_ = 0;
}
