#ifndef HEADER_Gbp_Tra_SaveValueWrap_h
#define HEADER_Gbp_Tra_SaveValueWrap_h
#pragma once
#include "./TraceTypes.h"

namespace Gbp { namespace Tra {
	

	//////////////////////////////////////////////////////////////////////////
	// std::string
	template <typename T>
	inline 
		typename boost::enable_if_c<
		Gbp::Tra::IsStdStringType<T>::value, 
		void>::type
		SaveValueBegin(Slot_t*& p, const T& t, size_t n)
	{
		size_t m = t.size();
		n *= sizeof(Slot_t);
		if(n > m) n = m;
		::memcpy(p, &t[0], n);
	}

	template <typename T>
	inline 
		typename boost::enable_if_c<
		Gbp::Tra::IsStdStringType<T>::value, 
		void>::type
		SaveValueEnd(Slot_t*& p, const T& t, size_t n)
	{
		// First the length, then the data.
		size_t m = t.size();
		n *= sizeof(Slot_t);
		if(n > m) return;
		size_t c = m - n;
		::memcpy(p, &t[n], c);
		p += c / sizeof(Slot_t);
		if(c % sizeof(Slot_t) != 0) ++p;
	}


	//////////////////////////////////////////////////////////////////////////
	// For everything else
	template <typename T>
	inline
		typename boost::disable_if_c<
			Gbp::Tra::IsStdStringType<T>::value, 
			void>::type
		SaveValueBegin(Slot_t*& p, const T& t, size_t)
	{
	}

	template <typename T>
	inline
		typename boost::disable_if_c<
		Gbp::Tra::IsStdStringType<T>::value, 
		void>::type
		SaveValueEnd(Slot_t*& p, const T& t, size_t)
	{
	}

	//////////////////////////////////////////////////////////////////////////

	template <typename T>
	inline
		typename boost::enable_if_c<
		Gbp::Tra::IsStdStringType<T>::value, 
		size_t>::type
		ValueSize(const T& t)
	{
		return t.size();
	}

	template <typename T>
	inline
		typename boost::disable_if_c<
		Gbp::Tra::IsStdStringType<T>::value, 
		size_t>::type
		ValueSize(const T& t)
	{
		return 0;
	}



}}

#endif // HEADER_Gbp_Tra_SaveValueWrap_h
