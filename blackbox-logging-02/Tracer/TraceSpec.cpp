#include "./TraceSpec.h"
#include <limits>

namespace Gbp { namespace Tra {

	TraceSpec* TraceSpecStart(const char* pn)
	{
		TraceSpec ts;
		ptrdiff_t d = (const char*)&ts.n - (const char*)&ts;
		return (TraceSpec*)(pn - d);
	}

	bool EqualBytes(unsigned char *a, unsigned char *b, size_t n)
	{
		for(size_t i = 0; i < n; ++i)
		{
			if(*a++ != *b++)
			{
				return false;
			}
		}
		return true;
	}

}}
