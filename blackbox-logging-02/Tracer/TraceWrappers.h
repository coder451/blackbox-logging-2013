#ifndef HEADER_Gbp_Tra_TraceWrappers_h
#define HEADER_Gbp_Tra_TraceWrappers_h
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>

namespace Gbp { namespace Tra {

	//////////////////////////////////////////////////////////////////////////
	// Two functions that can be used to tell the compiler that a null-terminated string is being output.

	template <typename T>
	class basic_cstring
	{
		const T* pT_;
	public:
		basic_cstring(const T* pT): pT_(pT){}
		const T* ptr_value() const {return pT_;}
#pragma warning(push)
#pragma warning(disable: 4996)
		size_t size() const {return ::strlen(pT_);}
#pragma warning(pop)
	};
	typedef basic_cstring<char> sz_string;
	typedef basic_cstring<wchar_t> sz_wstring;

	inline 
	const sz_string 
	as_sz(const char* psz)
	{
		return sz_string(psz);
	}

	inline 
	const sz_wstring 
	as_sz(const wchar_t* pszw)
	{
		return sz_wstring(pszw);
	}

	//////////////////////////////////////////////////////////////////////////
	// Wrapper for pointer, when documenting the value of the pointer.
	template <typename T>
	class ptr_wrapper
	{
		const T* pT_;
	public:
		ptr_wrapper(const T* pT): pT_(pT){}
		const T* ptr_value() const {return pT_;}
		const size_t* ptr_addr() const {return (size_t*)&pT_;}
	};
	

	// Factory function so there is no need for a template parameter in the instrumentation call.
	// Use
	//    lfPrintf("%p", as_ptr(p));
	template <typename T> 
	inline 
	const ptr_wrapper<T>
	as_ptr(const T* pT)
	{
		return ptr_wrapper<T>(pT);
	}

	//////////////////////////////////////////////////////////////////////////
	// Wrapper for array count
	class array_count
	{
		int n_;
	public:
		array_count(int n): n_((int)n){}
		int count() const {return n_;}
	};

	// Wrapper for counted array
	template <typename T>
	class counted_array
	{
		const T* pT_;
	public:
		counted_array(const T* pT): pT_(pT){}
		const T* ptr_value() const {return pT_;}
	};

	template <typename T> 
	inline 
	const counted_array<T>
	as_car(const T* pT)
	{
		return counted_array<T>(pT);
	}
}}
#endif // HEADER_Gbp_Tra_TraceWrappers_h
