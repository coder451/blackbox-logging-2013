#ifndef HEADER_Gbp_Mt_ThreadTypes_h
#define HEADER_Gbp_Mt_ThreadTypes_h
#pragma once
#ifdef _WIN32
#include <Windows.h>
#else
#include <pthreads.h>
#endif // platform

namespace Gbp { namespace Mt {

#ifdef _WIN32 // platform
	typedef unsigned int ThreadFunctionReturn_t;
	typedef void *ThreadFunctionArgPtr_t;
	typedef HANDLE ThreadHandle_t;
	typedef unsigned ThreadAddress_t;
	typedef DWORD ThreadId_t;
#else
	typedef void *ThreadFunctionReturn_t;
	typedef void *ThreadFunctionArgPtr_t;
	typedef pthread_t ThreadHandle_t;
	// Not used by pthreads
	typedef void *ThreadAddress_t;
	typedef pthread_t ThreadId_t;
#endif // platform

}}
#endif // HEADER_Gbp_Mt_ThreadTypes_h
