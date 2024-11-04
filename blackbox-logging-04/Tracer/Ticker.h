#ifndef HEADER_Gbp_Tra_Ticker_h
#define HEADER_Gbp_Tra_Ticker_h
#pragma once
#include <Mt/ThreadTypes.h>
namespace Gbp { namespace Tra {
	typedef unsigned __int64 Time_t;
	class Ticker
	{
	public:
		Ticker();
		~Ticker();
		void finish(){isFinishing_ = true;}
		Gbp::Mt::ThreadFunctionReturn_t threadFunc();
		Time_t t() const {return t_;}
	private:
		bool isFinishing_;
		Time_t t_;
	};
}}
#endif // HEADER_Gbp_Tra_Ticker_h
