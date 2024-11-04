#ifndef HEADER_Gbp_Tra_Mutex_h
#define HEADER_Gbp_Tra_Mutex_h
#pragma once
namespace Gbp { namespace Tra {
	class MutexImpl;
	class Mutex
	{
	public:
		Mutex();
		~Mutex();
		void lock();
		void unlock();
	private:
		MutexImpl* pImpl_;
	};

	class MutexMgr
	{
	public:
		MutexMgr(Mutex& m): m_(m)
		{
			m_.lock();
		}
		~MutexMgr()
		{
			m_.unlock();
		}
	private:
		Mutex& m_;
	};
}}
#endif // HEADER_Gbp_Tra_Mutex_h
