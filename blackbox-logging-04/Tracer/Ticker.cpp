#include "./Ticker.h"

namespace Gbp { namespace Tra {

	Ticker::Ticker(): isFinishing_(false), t_(0)
	{
	}

	Ticker::~Ticker()
	{
	}

	Gbp::Mt::ThreadFunctionReturn_t Ticker::threadFunc()
	{
		for(;!isFinishing_;)
		{
			::Sleep(1);
			++t_;
		}
		return 0;
	}

}}
