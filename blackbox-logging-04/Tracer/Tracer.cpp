#include "./Tracer.h"
#include "./TraceSpec.h"
#include "./TraceSpecs.h"
#include "./TraceDefn.h"
#include "./TracerEx.h"
#include <stdio.h>

namespace Gbp { namespace Tra {
	Tracer* Tracer::pInstance_ = 0;

	Tracer::Tracer(size_t size): size_(size), buffer_()
	{
		TraceSpecs traceSpecs;
		if(!traceSpecs.isValid()) return;

		// Count the traceSpecs and their frequencies
		size_t count = 0;
		size_t freqTotal = 0;
		for(TraceSpecs::iterator i = traceSpecs.begin(); i != traceSpecs.end(); ++i)
		{
			++count;
			freqTotal += i->freq;
		}
		if(count == 0 || freqTotal == 0) return;


		// Put the pointers in a vector for easier access than traversing the PE.
		traceSpecVector_.reserve(count);
		for(TraceSpecs::iterator i = traceSpecs.begin(); i != traceSpecs.end(); ++i)
		{
			TraceSpec& ts = *i;
			traceSpecVector_.push_back(&ts);
		}

		// Set up the TraceDefns and allocate space to each. Each amount is rounded up to a power
		// of two to make wrapping calculations easier.
		size_t N = 0;
		for(TraceSpecVector::iterator i = traceSpecVector_.begin(); i != traceSpecVector_.end(); ++i)
		{
			TraceSpec& ts = **i;
			size_t m = ts.freq * size_ / freqTotal;
			
			// no tiny buffers
			if(m < 1024) m = 1024;
			
			// power of two buffers to help with alignment
			size_t n = 1;
			while(n < m) n *= 2;

			N += n;
			ts.n = n;
		}

		// Allocate all the storage
		size_ = N;
		buffer_.resize(size_, (char)0xFF);

		// Apportion the storage to each TraceSpec.
		char* p = &buffer_[0];
		size_t offset = 0;
		for(TraceSpecVector::iterator i = traceSpecVector_.begin(); i != traceSpecVector_.end(); ++i)
		{
			TraceSpec& ts = **i;
			ts.pTraceDefn = new TraceDefn(&ts, offset, p);
			offset += ts.n;
		}
	}

	Tracer::~Tracer()
	{
		for(TraceSpecVector::iterator i = traceSpecVector_.begin(); i != traceSpecVector_.end(); ++i)
		{
			delete (*i)->pTraceDefn;
		}
	}

	bool Tracer::save(FILE* ft, FILE* fb)
	{
		fprintf(ft, "Tracer BufferSize=%u DefnCount=%u\n", size_ / sizeof(Slot_t), traceSpecVector_.size());
		for(TraceSpecVector::iterator i = traceSpecVector_.begin(); i != traceSpecVector_.end(); ++i)
		{
			bool r = (*i)->pTraceDefn->save(ft);
			if(!r) 
			{
				return false;
			}
		}
		int r = fwrite(&buffer_[0], 1, buffer_.size(), fb);
		if(r != buffer_.size())
		{
			return false;
		}
		return true;
	}

	// Set up a singleton.
	Tracer* Tracer::InitInstance(size_t size)
	{
		if(!pInstance_)
		{
			pInstance_ = new Tracer(size);
		}
		return pInstance_;
	}

	void Tracer::DestroyInstance()
	{
		delete pInstance_;
		pInstance_ = 0;
	}

	bool Tracer::Save(FILE* ft, FILE* fb)
	{
		return pInstance_->save(ft, fb);
	}

}}