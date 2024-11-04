#include "./TraceDefn.h"
#include "./Tracer.h"
#include <stdio.h>

using Gbp::Tra::Tracer;

namespace Gbp { namespace Tra {
	TraceDefn::TraceDefn(TraceSpec* pTraceSpec, size_t offset, char*& pBuffer):
	pTraceSpec_(pTraceSpec),
	size_(pTraceSpec_->n / sizeof(Slot_t)),
	offset_(offset / sizeof(Slot_t)),
	pBuffer0_((Slot_t*)pBuffer),
	pos_(0),
	prevPos_(0),
	oldestIndex_(0),
	newestIndex_(-(__int64)size_),
	formatParser_(pTraceSpec->paramCount),
	count_(0),
	hasArrays_(false),
	minLength_(0),
	mutex_()
	{
		printf("%s(%d) in %s: paramCount=%d\n", 
			pTraceSpec->file, pTraceSpec->line, pTraceSpec->function, pTraceSpec->paramCount);

		pBuffer += pTraceSpec_->n;
		formatParser_.parse(pTraceSpec_->fmt);
	}

	TraceDefn::~TraceDefn()
	{
	}

	void TraceDefn::checkFormatSpec(size_t length, int hasArrays, size_t paramCount, const int* paramTypes, const size_t* paramSizes)
	{
		minLength_ = length;
		hasArrays_ = hasArrays != 0;
		for(size_t i = 0; i < paramCount; ++i)
		{
			formatParser_.checkFormatSpec(i, paramTypes[i], paramSizes[i]);
		}
	}

	bool TraceDefn::save(FILE* f)
	{
		MutexMgr m(mutex_);
		int r = 0;

#pragma warning(push)
#pragma warning(disable: 4996)
		// Format is at the end as it may contain any characters.
		r = fprintf(f, "TraceDefn offset=%u sz=%u last=%u oldestIndex=%d newestIndex=%d count=%I64u length=%u hasArrays=%d file \"%s\" line=%d function \"%s\" format=%s\n",
			offset_, size_, prevPos_, (int)oldestIndex_ % size_, (int)(newestIndex_ + size_) % size_,
			count_, minLength_, (hasArrays_? 1 : 0),
			pTraceSpec_->file, pTraceSpec_->line, pTraceSpec_->function,
			pTraceSpec_->fmt);
#pragma warning(pop)

		if(r < 0) return false;
		r = formatParser_.save(f);
		if(r < 0) return false;
		return true;
	}

	Sequence_t TraceDefn::SequenceNumber()
	{
		return Tracer::SequenceNumber();
	}
}}