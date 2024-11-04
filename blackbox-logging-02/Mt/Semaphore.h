#ifndef HEADER_Gbp_Mt_Semaphore_h
#define HEADER_Gbp_Mt_Semaphore_h
#pragma once
#include <Windows.h>
namespace Gbp { namespace Mt {
	class Semaphore
	{
	public:
		Semaphore();
		~Semaphore();
		void wait();
		void release(size_t n);
	private:
		HANDLE h_;
	};
}}
#endif //HEADER_Gbp_Mt_Semaphore_h
