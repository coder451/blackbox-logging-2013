#ifndef HEADER_Gbp_Tra_SaveValue_h
#define HEADER_Gbp_Tra_SaveValue_h
#pragma once
// These provide efficient ways of assigning each fixed-size type to a 64-bit quantity
#include "./TraceTypes.h"

namespace Gbp { namespace Tra {
	
	//////////////////////////////////////////////////////////////////////////
	// Arithmetic types: assigned directly, except for float
	template <typename T>
	inline
		typename std::enable_if<
		std::is_arithmetic<T>::value && !std::is_floating_point<T>::value, 
		void>::type
	SaveValue(Slot_t*& p, const T& t)
	{
		*p++ = t;
	}

	//////////////////////////////////////////////////////////////////////////
	// Float: change to a double
	template <typename T>
	inline
		typename std::enable_if<
		std::is_floating_point<T>::value, 
		void>::type
		SaveValue(Slot_t*& p, const T& t)
	{
		double* q = (double*)p;
		*q = (double)t;
		++p;
	}

	//////////////////////////////////////////////////////////////////////////
	// Pointer wrappers: assign the pointer directly
	template <typename T>
	inline 
		typename std::enable_if<
		Gbp::Tra::IsPtrWrapper<T>::value, 
		void>::type
	SaveValue(Slot_t*& p, const T& t)
	{
		*p++ = (Slot_t)t.ptr_value();

	}

	//////////////////////////////////////////////////////////////////////////
	// std::string
	template <typename T>
	inline 
		typename std::enable_if<
		Gbp::Tra::IsStdStringType<T>::value, 
		void>::type
		SaveValue(Slot_t*& p, const T& t)
	{
		// First the length, then the data.
		size_t n = t.size();
		*p++ = n;
		::memcpy(p, &t[0], n);
		p += n / sizeof(Slot_t);
		if(n % sizeof(Slot_t) != 0) ++p;
	}

	//////////////////////////////////////////////////////////////////////////
	// For everything else
	template <typename T>
	inline
		typename std::enable_if<
			!(std::is_arithmetic<T>::value || 
			Gbp::Tra::IsPtrWrapper<T>::value ||
			Gbp::Tra::IsStdStringType<T>::value), 
			void>::type
		SaveValue(Slot_t*& p, const T& t)
	{
	}

}}

#endif // HEADER_Gbp_Tra_SaveValue_h
