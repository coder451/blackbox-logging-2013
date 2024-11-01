#include "./MyClass.h"
#include <Tracer/Trace.h>

MyClass::MyClass(): n_(0)
{
	n_ = 1;
// 	__declspec(allocate(".trace")) static Gbp::Tra::TraceSpec ts1 = {__FILE__, __LINE__, __FUNCTION__, 10, "%d", Gbp::Tra::TRACESPEC_MAGIC, 0};
// 	ts1.pTraceDefn->trace(1);
	TRACEF(10, "MyClass ctor");
}

MyClass::~MyClass()
{
// 	__declspec(allocate(".trace")) static Gbp::Tra::TraceSpec ts2 = {__FILE__, __LINE__, __FUNCTION__, 10, "%d", Gbp::Tra::TRACESPEC_MAGIC, 0};
// 	ts2.pTraceDefn->trace(1);
	TRACEF(10, "MyClass dtor");
	n_ = 0;
}
