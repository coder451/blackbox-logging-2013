#ifndef HEADER_Gbp_Tra_FormatSpec_h
#define HEADER_Gbp_Tra_FormatSpec_h
#pragma once
#include <string>
#include <stdio.h>

namespace Gbp { namespace Tra {
	enum SaveType
	{
		SAVETYPE_NONE,
		SAVETYPE_DIRECT,
		SAVETYPE_SZ_ARRAY,
		SAVETYPE_ARRAY_COUNT,
		SAVETYPE_COUNTED_ARRAY
	};

	class FormatSpec
	{
		int saveType_;

		// These two items tell where the format specs are.
		// They point at the beginning of either the length modifier or the type,
		// whichever exists.
		const char* pFmt_;
		int lenFmt_;

		// These are to tell whether this format has to be rewritten based on 
		// actual parameter types.
		enum FmtChg {FC_NONE, FC_JUST_TYPE, FC_LONG_TYPE};
		FmtChg fmtChg_;

		int typeIndex_;
		int size_;
		const char* pFmt0_;
	public:
		FormatSpec();

		void reset();

		void saveType(int v){saveType_ = v;}
		void fmtStart(const char* v){pFmt_ = v;}
		void fmtStart0(const char* v){pFmt0_ = v;}
		void lenFmt(int v){lenFmt_ = v;}
		void lenFmtFromPtr(const char* p){lenFmt_ = (int)(p  - pFmt_);}

		int saveType() const {return saveType_;}

		const char* fmtStart() const {return pFmt_;}
		char fmtTypeChar() const {return pFmt_[lenFmt_ - 1];}
		int lenFmt() const {return lenFmt_;}
		const char* fmtEnd() const {return pFmt_ + lenFmt_;}

		bool doFixup() const {return fmtChg_ != FC_NONE;}
		void useDefaultFmt(){fmtChg_ = FC_JUST_TYPE;}
		void useLongFmt(){fmtChg_ = FC_LONG_TYPE;}
		void fixType(std::string& fmt);

		void typeIndex(int v){typeIndex_ = v;}
		void size(int v){size_ = v;}
		ptrdiff_t offset() const {return pFmt_ - pFmt0_;}
		bool save(FILE* f);
	};
}}

#endif // HEADER_Gbp_Tra_FormatSpec_h
