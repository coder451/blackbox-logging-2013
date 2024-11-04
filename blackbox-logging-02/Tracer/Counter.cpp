#include "./Counter.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#error platform
#endif

namespace Gbp { namespace Tra {

	Counter::Counter(): a_(0){}
	Counter::~Counter(){}

	Sequence_t Counter::increment()
	{
#ifdef _WIN32
		return (Sequence_t)::InterlockedIncrement64((LONGLONG volatile*)&a_);
#else
#error platform
#endif
	}

}}