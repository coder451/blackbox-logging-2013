#include "./MyClass.h"
#include "./MyTemplate.h"
#include "./MyAction.h"
#include <Tracer/Trace.h>
#include <Mt/Threads.h>
#include <stdio.h>
using Gbp::Tra::Tracer;
using Gbp::Tra::as_ptr;

int main(int argc, char* argv[])
{
	// Check compile for unsupported UDT
	// TRACEF(10, "%d", MyClass());

	Tracer::InitInstance(100000);

	TRACEF(10, "Start");

	// Some threads
	MyAction myAction;
	Gbp::Mt::Threads<MyAction> threads(2, &myAction);
	threads.create();
	threads.start();

 	MyClass* m = new MyClass();
 	delete m;

	Sleep(1000);

	std::string s1("abcdef");
	std::string s2("uvxy");
 	for(int i = 0; i < 100; ++i)
 	{
		TRACEF(10, "Three parameters, int last : %s xxx %s %d", s1, s2, i);
		Sleep(100);
		TRACEF(10, "Three parameters, int first: %d yyy %s %s", i, s1, s2);
 	}

 	MyTemplate<int> mt1;
 	MyTemplate<size_t> mt2;

 	TRACEF(1, "A double: %f", 3.1415926535);

	TRACEF(1, "Pointers: &mt1=0x%p, &mt2=0x%p", as_ptr(&mt1), as_ptr(&mt2));

	myAction.finish();
	threads.wait();

	TRACEF(1, "Finishing.");

	Tracer::Save(argv[1]);

	return 0;
}

