#ifndef HEADER_Gbp_Tra_TraceHelpers_h
#define HEADER_Gbp_Tra_TraceHelpers_h
#pragma once

// Compute length of array items. Do not include the length entry - just count the data.

#include "./TraceTypes.h"
namespace Gbp { namespace Tra {

	inline size_t SlotCount(size_t n)
	{
		size_t r = n % sizeof(Slot_t);
		if(r == 0) return n / sizeof(Slot_t);
		return (n + sizeof(Slot_t) - r) / sizeof(Slot_t);
	}

	// Applies to every type that does not represent a variable length.
	// Not used at runtime.
	template <typename T> inline 
		typename boost::disable_if_c<
		Gbp::Tra::ParamType<T>::hasVarLen != 0, 
		size_t>::type 
		LengthOf(const T& t)
	{
		return 0;
	}

	template <typename T> inline 
		typename boost::enable_if_c<
		Gbp::Tra::ParamType<T>::isStdString != 0, 
		size_t>::type 
		LengthOf(const T& t)
	{
		return SlotCount(t.size());
	}

	template <typename T> inline 
		typename boost::enable_if_c<
		Gbp::Tra::ParamType<T>::isStdWstring != 0, 
		size_t>::type 
		LengthOf(const T& t)
	{
		return SlotCount(2 * t.size());
	}

	template <typename T> inline 
		typename boost::enable_if_c<
		Gbp::Tra::ParamType<T>::isStdVector != 0, 
		size_t>::type 
		LengthOf(const T& t)
	{
		return SlotCount(t.size() * sizeof(t::value_type));
	}

	template <typename T> inline 
		typename boost::enable_if_c<
		Gbp::Tra::ParamType<T>::isSzStringWrapper != 0, 
		size_t>::type 
		LengthOf(const T& t)
	{
		return SlotCount(t.size());
	}

	template <typename T> inline 
		typename boost::enable_if_c<
		Gbp::Tra::ParamType<T>::isSzWStringWrapper != 0, 
		size_t>::type 
		LengthOf(const T& t)
	{
		return SlotCount(2 * t.size());
	}
}}


#endif // HEADER_Gbp_Tra_TraceHelpers_h
