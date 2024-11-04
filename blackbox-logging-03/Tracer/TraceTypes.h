#ifndef HEADER_Gbp_Tra_TraceTypes_h
#define HEADER_Gbp_Tra_TraceTypes_h
#pragma once
#include "./TraceWrappers.h"
#include <type_traits>
#include <vector>
#include <string>

namespace Gbp { namespace Tra {

	typedef __int64 int64;
	typedef unsigned __int64 uint64;
	typedef uint64 Slot_t;

	enum ParamTypeIndexValues
	{
		PARAM_NONE                  = 0,
		PARAM_BOOL                  = 0x00000001,
		PARAM_ENUM                  = 0x00000002,
		PARAM_CHAR                  = 0x00000004,
		PARAM_UCHAR                 = 0x00000008,
		PARAM_WCHAR                 = 0x00000010, //       16,
		PARAM_INT16                 = 0x00000020, //       32, 
		PARAM_INT32                 = 0x00000040, //       64, 
		PARAM_INT64                 = 0x00000080, //      128, 
		PARAM_UINT16                = 0x00000100, //      256,
		PARAM_UINT32                = 0x00000200, //      512,
		PARAM_UINT64                = 0x00000400, //     1024,
		PARAM_FLOAT32               = 0x00000800, //     2048,
		PARAM_FLOAT64               = 0x00001000, //     4096,
		PARAM_CHAR_PTR              = 0x00002000, //     8192,
		PARAM_CHAR_ARRAY            = 0x00004000, //    16384,
		PARAM_WCHAR_PTR             = 0x00008000, //    32768,
		PARAM_WCHAR_ARRAY           = 0x00010000, //    65536,
		PARAM_STD_STRING            = 0x00020000, //   131072,
		PARAM_STD_WSTRING           = 0x00040000, //   262144,
		PARAM_STD_VECTOR            = 0x00080000, //   524288,
		PARAM_POINTER               = 0x00100000, //  1048576,
		PARAM_ARRAY                 = 0x00200000, //  2097152,
		PARAM_SZ_STRING_WRAPPER     = 0x00400000, //  4194304,
		PARAM_SZ_WSTRING_WRAPPER    = 0x00800000, //  8388608,
		PARAM_PTR_WRAPPER           = 0x01000000, // 16777216,
		PARAM_COUNTED_ARRAY_WRAPPER = 0x02000000, // 33554432,
	};

	//////////////////////////////////////////////////////////////////////////
	// Parameter type detection tools

	// Detect char and wchar_t as "BasicChar".
	template <typename T> struct IsBasicChar: public std::false_type{};
	template <> struct IsBasicChar<char>: public std::true_type{};
	template <> struct IsBasicChar<wchar_t>: public std::true_type{};

	// Detect various flavors of pointers to char and wchar_t, as "BasicCharPtr".
	template <typename T> struct IsBasicCharPtr: public std::false_type{};
	template <> struct IsBasicCharPtr<const char*>: public std::true_type{};
	template <> struct IsBasicCharPtr<const wchar_t*>: public std::true_type{};
	template <> struct IsBasicCharPtr<char*>: public std::true_type{};
	template <> struct IsBasicCharPtr<wchar_t*>: public std::true_type{};

	// Detect a pointer that is not a BasicCharPtr.
	template <typename T>
	struct IsPtrNotBasicCharPtr
	{
	private:
		static const bool c1 = std::is_pointer<T>::value;
		static const bool c2 = !(IsBasicCharPtr<T>::value);
	public:
		static const bool value = c1 && c2;
	};

	// Detect std::string and std::wstring.
	template <typename T> struct IsStdStringType: public std::false_type {};
	template <> struct IsStdStringType<std::string>: public std::true_type{};
	template <> struct IsStdStringType<std::wstring>: public std::true_type{};

	// Detect null-terminated string wrapper for narrow and wide characters.
	template <typename T> struct IsSzStringWrapper: public std::false_type{};
	template <> struct IsSzStringWrapper<sz_string>: public std::true_type{};
	template <> struct IsSzStringWrapper<sz_wstring>: public std::true_type{};

	// Detect a pointer wrapper parameter.
	template <typename T> struct IsPtrWrapper: public std::false_type{};
	template <typename P> struct IsPtrWrapper<ptr_wrapper<P> >: public std::true_type{};

	// Detect a counted_array wrapper
	template <typename T> struct IsCountedArrayWrapper: public std::false_type{};
	template <typename P> struct IsCountedArrayWrapper<counted_array<P> >: public std::true_type{};
	template <typename P> struct IsCountedArrayWrapper<counted_array<const P> >: public std::true_type{};

	template <typename T>
	struct IsChar
	{
		static const bool c1 = std::is_same<T, char>::value;
		static const bool c2 = std::is_same<T, const char>::value;
		static const bool value = c1 || c2;
	};

	template <typename T>
	struct IsUchar
	{
		static const bool c1 = std::is_same<T, unsigned char>::value;
		static const bool c2 = std::is_same<T, const unsigned char>::value;
		static const bool value = c1 || c2;
	};

	template <typename T>
	struct IsNonvolatileInt
	{
		static const bool c1 = std::is_same<T, short>::value;
		static const bool c2 = std::is_same<T, int>::value;
		static const bool c3 = std::is_same<T, long>::value;
		static const bool c4 = std::is_same<T, int64>::value;
		static const bool value = c1 || c2 || c3 || c4;
	};
	template <typename T>
	struct IsVolatileInt
	{
		static const bool c1 = std::is_same<T, volatile short>::value;
		static const bool c2 = std::is_same<T, volatile int>::value;
		static const bool c3 = std::is_same<T, volatile long>::value;
		static const bool c4 = std::is_same<T, volatile int64>::value;
		static const bool value = c1 || c2 || c3 || c4;
	};
	template <typename T>
	struct IsInt
	{
		static const bool c1 = IsNonvolatileInt<T>::value;
		static const bool c2 = IsVolatileInt<T>::value;
		static const bool value = c1 || c2;
	};

	template <typename T>
	struct IsNonvolatileUint
	{
		static const bool c1 = std::is_same<T, unsigned short>::value;
		static const bool c2 = std::is_same<T, unsigned int>::value;
		static const bool c3 = std::is_same<T, unsigned long>::value;
		static const bool c4 = std::is_same<T, uint64>::value;
		static const bool value = c1 || c2 || c3 || c4;
	};
	template <typename T>
	struct IsVolatileUint
	{
		static const bool c1 = std::is_same<T, volatile unsigned short>::value;
		static const bool c2 = std::is_same<T, volatile unsigned int>::value;
		static const bool c3 = std::is_same<T, volatile unsigned long>::value;
		static const bool c4 = std::is_same<T, volatile uint64>::value;
		static const bool value = c1 || c2 || c3 || c4;
	};
	template <typename T>
	struct IsUint
	{
		static const bool c1 = IsNonvolatileUint<T>::value;
		static const bool c2 = IsVolatileUint<T>::value;
		static const bool value = c1 || c2;
	};

	template <typename T> struct IsCharPtr: public std::false_type{};
	template <> struct IsCharPtr<char*>: public std::true_type{};
	template <> struct IsCharPtr<const char*>: public std::true_type{};

	template <typename T> struct IsWcharPtr: public std::false_type{};
	template <> struct IsWcharPtr<wchar_t*>: public std::true_type{};
	template <> struct IsWcharPtr<const wchar_t*>: public std::true_type{};


	template <typename T>
	struct IsCharArray
	{
		static const bool c1 = std::is_array<T>::value;
		static const bool c2 = std::is_same<typename std::remove_const<typename std::remove_extent<T>::type>::type, char>::value;
		static const bool value = c1 && c2;
	};

	template <typename T>
	struct IsWcharArray
	{
		static const bool c1 = std::is_array<T>::value;
		static const bool c2 = std::is_same<typename std::remove_const<typename std::remove_extent<T>::type>::type, wchar_t>::value;
		static const bool value = c1 && c2;
	};

	template <typename T> struct IsStdVector: public std::false_type{};
	template <typename U> struct IsStdVector<std::vector<U> >: public std::true_type{};

	//////////////////////////////////////////////////////////////////////////
	// Turning type information into assessment of whether a parameter can be written out for tracing

	template <typename T>
	struct ParamType
	{
		static const int isBool = std::is_same<T, bool>::value? PARAM_BOOL: 0;
		static const int isEnum = std::is_enum<T>::value? PARAM_ENUM : 0;
		static const int isChar = (IsChar<T>::value)? PARAM_CHAR : 0;
		static const int isUchar = (IsUchar<T>::value)? PARAM_UCHAR : 0;
		static const int isWchar = (std::is_same<T, wchar_t>::value)? PARAM_WCHAR : 0;
		static const int isInt16 = (IsInt<T>::value && sizeof(T) == 2)? PARAM_INT16 : 0;
		static const int isInt32 = (IsInt<T>::value && sizeof(T) == 4)? PARAM_INT32 : 0;
		static const int isInt64 = (IsInt<T>::value && sizeof(T) == 8)? PARAM_INT64 : 0;
		static const int isUint16 = (IsUint<T>::value && sizeof(T) == 2)? PARAM_UINT16 : 0;
		static const int isUint32 = (IsUint<T>::value && sizeof(T) == 4)? PARAM_UINT32 : 0;
		static const int isUint64 = (IsUint<T>::value && sizeof(T) == 8)? PARAM_UINT64 : 0;
		static const int isFloat32 = (std::is_floating_point<T>::value && sizeof(T) == 4)? PARAM_FLOAT32 : 0;
		static const int isFloat64 = (std::is_floating_point<T>::value && sizeof(T) == 8)? PARAM_FLOAT64 : 0;
		static const int isCharPtr = IsCharPtr<T>::value? PARAM_CHAR_PTR : 0;
		static const int isCharArray = IsCharArray<T>::value? PARAM_CHAR_ARRAY : 0;
		static const int isWcharPtr = IsWcharPtr<T>::value? PARAM_WCHAR_PTR : 0;
		static const int isWcharArray = IsWcharArray<T>::value? PARAM_WCHAR_ARRAY : 0;
		static const int isStdString = std::is_same<T, std::string>::value? PARAM_STD_STRING : 0;
		static const int isStdWstring = std::is_same<T, std::wstring>::value? PARAM_STD_WSTRING : 0;
		static const int isStdVector = IsStdVector<T>::value? PARAM_STD_VECTOR : 0;
		static const int isPointer = std::is_pointer<T>::value? PARAM_POINTER : 0;
		static const int isArray = std::is_array<T>::value? PARAM_ARRAY : 0;
		static const int isSzStringWrapper = std::is_same<T, sz_string>::value? PARAM_SZ_STRING_WRAPPER : 0;
		static const int isSzWStringWrapper = std::is_same<T, sz_wstring>::value? PARAM_SZ_WSTRING_WRAPPER : 0;
		static const int isPointerWrapper = IsPtrWrapper<T>::value? PARAM_PTR_WRAPPER : 0;
		static const int isCountedArrayWrapper = IsCountedArrayWrapper<T>::value? PARAM_COUNTED_ARRAY_WRAPPER : 0;

		static const int isInt = isInt16 | isInt32 | isInt64;
		static const int isUint = isUint16 | isUint32 | isUint64;
		static const int isInteger = isInt | isUint;
		static const int isFloat = isFloat32 | isFloat64;

		// The types that prevent efficient output are excluded from being used as trace parameters.
		// They must be wrapped in helper types.

		static const int value = isBool | isEnum | isChar | isUchar | isWchar | 
			isInteger | isFloat | 
//			isCharPtr | isCharArray | 
//			isWcharPtr | isWcharArray | 
			isStdString | isStdWstring | 
			isSzStringWrapper | isSzWStringWrapper |
			isStdVector |
//			isPointer | 
			isPointerWrapper | 
//			isArray |
			isCountedArrayWrapper;

		static const int hasVarLen = 
			//			isCharPtr | isCharArray | 
			//			isWcharPtr | isWcharArray | 
			isStdString | isStdWstring | 
			isSzStringWrapper | isSzWStringWrapper |
			isStdVector |
			//			isPointer | 
			isPointerWrapper | 
			//			isArray |
			isCountedArrayWrapper;
	};

}}

#endif // HEADER_Gbp_Tra_TraceTypes_h
