#ifndef HEADER_Gbp_Tra_Traceable_h
#define HEADER_Gbp_Tra_Traceable_h
#pragma once
#include "./TraceTypes.h"
#include <type_traits>

namespace Gbp { namespace Tra {

	// Detect a traceable type.
	template <typename T>
	struct IsTraceable
	{
		static const bool value = (ParamType<T>::value != PARAM_NONE);
	};

	// This template can take from 1 to 20 parameters. All the default parameters work no matter what, as
	// char, the default, is an traceable type. If any Ti is not traceable, the type obtained from 
	// enable_if_c is not present and the compilation fails. Doing it like this makes it possible to check 20 parameters 
	// in a single line of code.
	// If there is a parameter that is not traceable, you will see  first a compiler message like:
	// error C2039: 'type' : is not a member of 'std::enable_if<B,T>'
	// and it will say that B is false and T is a type. This is the type that was wrong - for example, MyClass.
	// Subsequent error messages will identify the actual program line that caused the error.

	template <typename T1 = char, typename T2 = char, typename T3 = char, typename T4 = char, typename T5 = char, 
		typename T6 = char, typename T7 = char, typename T8 = char, typename T9 = char, typename T10 = char, 
		typename T11 = char, typename T12 = char, typename T13 = char, typename T14 = char, typename T15 = char, 
		typename T16 = char, typename T17 = char, typename T18 = char, typename T19 = char, typename T20 = char>
	struct AreTraceable
	{
		static const size_t n1 = sizeof(std::enable_if<IsTraceable<T1>::value, char>::type);
		static const size_t n2 = sizeof(std::enable_if<IsTraceable<T2>::value, char>::type);
		static const size_t n3 = sizeof(std::enable_if<IsTraceable<T3>::value, char>::type);
		static const size_t n4 = sizeof(std::enable_if<IsTraceable<T4>::value, char>::type);
		static const size_t n5 = sizeof(std::enable_if<IsTraceable<T5>::value, char>::type);
		static const size_t n6 = sizeof(std::enable_if<IsTraceable<T6>::value, char>::type);
		static const size_t n7 = sizeof(std::enable_if<IsTraceable<T7>::value, char>::type);
		static const size_t n8 = sizeof(std::enable_if<IsTraceable<T8>::value, char>::type);
		static const size_t n9 = sizeof(std::enable_if<IsTraceable<T9>::value, char>::type);
		static const size_t n10 = sizeof(std::enable_if<IsTraceable<T10>::value, char>::type);
		static const size_t n11 = sizeof(std::enable_if<IsTraceable<T11>::value, char>::type);
		static const size_t n12 = sizeof(std::enable_if<IsTraceable<T12>::value, char>::type);
		static const size_t n13 = sizeof(std::enable_if<IsTraceable<T13>::value, char>::type);
		static const size_t n14 = sizeof(std::enable_if<IsTraceable<T14>::value, char>::type);
		static const size_t n15 = sizeof(std::enable_if<IsTraceable<T15>::value, char>::type);
		static const size_t n16 = sizeof(std::enable_if<IsTraceable<T16>::value, char>::type);
		static const size_t n17 = sizeof(std::enable_if<IsTraceable<T17>::value, char>::type);
		static const size_t n18 = sizeof(std::enable_if<IsTraceable<T18>::value, char>::type);
		static const size_t n19 = sizeof(std::enable_if<IsTraceable<T19>::value, char>::type);
		static const size_t n20 = sizeof(std::enable_if<IsTraceable<T20>::value, char>::type);
	};
}}

#endif // HEADER_Gbp_Tra_Traceable_h
