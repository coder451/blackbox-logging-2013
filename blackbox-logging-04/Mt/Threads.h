#ifndef HEADER_Gbp_Mt_Threads_h
#define HEADER_Gbp_Mt_Threads_h
#pragma once
#include "./ThreadTypes.h"
#include "./Semaphore.h"
#include <Windows.h>
#include <Process.h>
#include <vector>

namespace Gbp { namespace Mt {
	template <typename T>
	class Threads
	{
	public:
		Threads(size_t threadCount, T* pT):
		threadCount_(threadCount),
		pT_(pT)
		{
		}

		~Threads()
		{
		}

		bool create()
		{
			for(size_t i = 0; i < threadCount_; ++i)
			{
				ThreadHandle_t h = (ThreadHandle_t)::_beginthreadex(0, 0, &ThreadFunc, this, CREATE_SUSPENDED, 0);
				if(h == 0) return false;
				threadHandles_.push_back(h);
			}
			return true;
		}

		bool start()
		{
			for(size_t i = 0; i < threadCount_; ++i)
			{
				ResumeThread(threadHandles_[i]);
			}
			sem_.release(threadCount_);
			return true;
		}

		bool wait()
		{
			WaitForMultipleObjects((DWORD)threadCount_, &threadHandles_[0], TRUE, INFINITE);
			return true;
		}
	private:
		size_t threadCount_;
		T* pT_;
		Semaphore sem_;
		typedef std::vector<ThreadHandle_t> ThreadHandles;
		ThreadHandles threadHandles_;
		static ThreadFunctionReturn_t __stdcall ThreadFunc(void* p)
		{
			((Threads*)p)->sem_.wait();
			return ((Threads*)p)->pT_->threadFunc();
		}
	};
}}
#endif // HEADER_Gbp_Mt_Threads_h

