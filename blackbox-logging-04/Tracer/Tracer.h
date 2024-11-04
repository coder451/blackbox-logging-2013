#ifndef HEADER_Gbp_Tra_Tracer_h
#define HEADER_Gbp_Tra_Tracer_h
#pragma once
#include "./TraceSpec.h"
#include "./Counter.h"
#include "./Ticker.h"
#include <Mt/Threads.h>
#include <vector>
#include <stdio.h>
#include <string>

// The main tracing object

namespace Gbp { namespace Tra {
	class Tracer
	{
	public:
		static Tracer* InitInstance(size_t size);
		static void DestroyInstance();
		static Sequence_t SequenceNumber(){return pInstance_->sequenceNumber();}
		static Time_t TickCount(){return pInstance_->tickCount();}
		static bool Save(const std::string& baseName);
		~Tracer();
	private:
		size_t size_;
		std::vector<char> buffer_;
		typedef std::vector<TraceSpec*> TraceSpecVector;
		TraceSpecVector traceSpecVector_;

		// Provides a sequence number for each trace call
		Counter seq_;

		// Provides a simple time surrogate
		Ticker ticker_;
		Gbp::Mt::Threads<Ticker> tickerThread_;

		Tracer(size_t size);
		static Tracer* pInstance_;

		Sequence_t sequenceNumber(){return seq_.increment();}
		Time_t tickCount() const {return ticker_.t();}
		bool save(const std::string& baseName);
	};
}}
#endif // HEADER_Gbp_Tra_Tracer_h
