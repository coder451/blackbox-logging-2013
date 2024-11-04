#include "./Mutex.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <pthreads.h>
#endif

namespace Gbp { namespace Tra {
#ifdef _WIN32
	class MutexImpl
	{
	public:
		MutexImpl(){::InitializeCriticalSection(&cs_);}
		~MutexImpl(){::DeleteCriticalSection(&cs_);}
		void lock(){::EnterCriticalSection(&cs_);}
		void unlock(){::LeaveCriticalSection(&cs_);}
	private:
		CRITICAL_SECTION cs_;
	};
#else
	class MutexImpl
	{
	public:
		MutexImpl()
		{
			::pthread_mutexattr_init(&attr_);
			::pthread_mutexattr_settype(&attr_, PTHREAD_MUTEX_RECURSIVE);
			::pthread_mutexattr_setpshared(&attr_, PTHREAD_PROCESS_PRIVATE);
			::pthread_mutex_init(&mutex_, &attr_);
		}
		~MutexImpl()
		{
			::pthread_mutex_destroy(&mutex_);
			::pthread_mutexattr_destroy(&attr_);
		}
		void lock()
		{
			::pthread_mutex_lock(&mutex_);
		}
		void unlock()
		{
			::pthread_mutex_unlock(&mutex_);
		}
	private:
		pthread_mutexattr_t attr_;
		pthread_mutex_t mutex_;
	};
#endif

	
	Mutex::Mutex(): pImpl_(new MutexImpl())
	{
	}

	Mutex::~Mutex()
	{
		delete pImpl_;
	}

	void Mutex::lock()
	{
		pImpl_->lock();
	}

	void Mutex::unlock()
	{
		pImpl_->unlock();
	}
}}
