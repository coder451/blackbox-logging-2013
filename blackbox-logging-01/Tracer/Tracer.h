#ifndef HEADER_Gbp_Tr_Tracer_h
#define HEADER_Gbp_Tr_Tracer_h
#pragma once
#include "./TraceSpec.h"
#include "./Counter.h"
#include "./TraceMacros.h"
#include <vector>
#include <stdio.h>

// The main tracing object

namespace Gbp { namespace Tra {
	class Tracer
	{
	public:
		static Tracer* InitInstance(size_t size);
		static void DestroyInstance();
		static Sequence_t SequenceNumber(){return pInstance_->sequenceNumber();}
		static bool Save(FILE* ft, FILE* fb);
		~Tracer();
	private:
		size_t size_;
		std::vector<char> buffer_;
		typedef std::vector<TraceSpec*> TraceSpecVector;
		TraceSpecVector traceSpecVector_;

		// Provides a sequence number for each trace call
		Counter seq_;

		Tracer(size_t size);
		static Tracer* pInstance_;

		Sequence_t sequenceNumber(){return seq_.increment();}
		bool save(FILE* ft, FILE* fb);
	};
}}
#endif // HEADER_Gbp_Tr_Tracer_h
