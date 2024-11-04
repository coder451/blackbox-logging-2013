#include "./Semaphore.h"
namespace Gbp { namespace Mt {

	Semaphore::Semaphore(): h_(0)
	{
		h_ = ::CreateSemaphore(0, 0, 64, 0);
	}

	Semaphore::~Semaphore()
	{
		if(h_ != NULL) CloseHandle(h_);
	}

	void Semaphore::wait()
	{
		::WaitForSingleObject(h_, INFINITE);
	}

	void Semaphore::release(size_t n)
	{
		::ReleaseSemaphore(h_, (DWORD)n, 0);
	}

}}