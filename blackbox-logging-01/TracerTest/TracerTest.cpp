#include "./MyClass.h"
#include "./MyTemplate.h"
#include <Tracer/Trace.h>
#include <stdio.h>
using Gbp::Tra::Tracer;

int main(int argc, char* argv[])
{
	Tracer::InitInstance(10000);

	// This can be set up by the compiler at compile time.
	// No assignment happens at runtime.
	// It is possible to find the segment at program startup and do initialization of storage just once.
	// Section must be defined before use in the translation unit.
	// 	__declspec(allocate(".trace"))  = {__FILE__, __LINE__, __FUNCTION__, 10, "%d", Gbp::Tra::TRACESPEC_MAGIC, 0};
	// 	ts.pTraceDefn->trace(i);
	// Linux:
	// static Gbp::Tra::TraceSpec ts_linenum __attribute__ ((section (SECTION_NAME))) = etc.
	// where SECTION_NAME is like
	//     .instr_obj_MyFile.cpp_linenum

	// Use a macro to hide the details
	//TRACEF(10, "main, using macro");

	{
		__declspec(allocate(".trace"))
			static Gbp::Tra::TraceSpec traceSpec_23 = {
				 __FILE__, // file
				 __LINE__, // line
				 __FUNCTION__, // function
				 10, // freq
				 "main, using macro", // fmt
				 Gbp::Tra::TRACESPEC_MAGIC, // n
				 1, // paramCount
				 0 // TraceDefn*
		   };
		traceSpec_23.pTraceDefn->trace();
	}
	(void)0;

	int i = 99;
	//TRACEF(10, "main: i=%d", i);

	MyClass* m = new MyClass();
	delete m;

	//TRACEF(10, "%d", MyClass());
	std::string s1("abcdef");
	std::string s2("uvxy");
	for(int i = 0; i < 10000; ++i)
	{
		TRACEF(10, "%s xxx %s %d", s1, s2, i);
		TRACEF(10, "%d yyy %s %s", i, s1, s2);
	}

	MyTemplate<int> mt1;
	MyTemplate<size_t> mt2;

	TRACEF(1, "A double: %f", 3.1415926535);

	std::vector<int> v(3);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	TRACEF(1, "A vector of int: %d", v);

	std::string fname((argv[1] == nullptr)? "default" : argv[1]);
	std::string ftname = fname + ".ttra";
#pragma warning(push)
#pragma warning(disable: 4996)
	FILE* ft = ::fopen(ftname.c_str(), "w");
#pragma warning(pop)
	if(ft == 0)
	{
		printf("Could not open tfile.\n");
		return 1;
	}

	std::string fbname = fname + ".btra";
#pragma warning(push)
#pragma warning(disable: 4996)
	FILE* fb = ::fopen(fbname.c_str(), "wb");
#pragma warning(pop)
	if(fb == 0)
	{
		printf("Could not open bfile.\n");
		return 1;
	}

	Tracer::Save(ft, fb);
	::fclose(ft);
	::fclose(fb);

	return 0;
}

