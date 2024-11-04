#include "./Tracer.h"
#include "./TraceSpec.h"
#include "./TraceSpecs.h"
#include "./TraceDefn.h"
#include "./FileMgr.h"
#include <Mt/Threads.h>
#include <stdio.h>

namespace Gbp { namespace Tra {
	Tracer* Tracer::pInstance_ = 0;

	Tracer::Tracer(size_t size): 
	size_(size), 
	buffer_(),
	traceSpecVector_(),
	seq_(),
	ticker_(),
	tickerThread_(1)
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
			int n = 1;
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

		// Start the ticker
		tickerThread_.setAction(&ticker_);
		tickerThread_.create();
		tickerThread_.start();
	}

	Tracer::~Tracer()
	{
		for(TraceSpecVector::iterator i = traceSpecVector_.begin(); i != traceSpecVector_.end(); ++i)
		{
			delete (*i)->pTraceDefn;
		}
	}


	bool Tracer::save(const std::string& baseName)
	{
		{
			FileMgr tfile(baseName, ".ttra", "w");
			if(!tfile.isValid())
			{
				printf("Could not open ttra file.\n");
				return false;
			}

			fprintf(tfile.f(), "Tracer BufferSize=%zu DefnCount=%zu\n", size_ / sizeof(Slot_t), traceSpecVector_.size());
			for(TraceSpecVector::iterator i = traceSpecVector_.begin(); i != traceSpecVector_.end(); ++i)
			{
				bool r = (*i)->pTraceDefn->save(tfile.f());
				if(!r) 
				{
					return false;
				}
			}
		}

		FileMgr bfile(baseName, ".btra", "wb");
		if(!bfile.isValid())
		{
			printf("Could not open btra file.\n");
			return false;
		}

		size_t r = fwrite(&buffer_[0], 1, buffer_.size(), bfile.f());
		return r == buffer_.size();
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

	bool Tracer::Save(const std::string& baseName)
	{
		return pInstance_->save(baseName);
	}

}}