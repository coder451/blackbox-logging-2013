#ifndef HEADER_Gbp_Tra_FmtSpec_h
#define HEADER_Gbp_Tra_FmtSpec_h
#pragma once
#include <string>
#include <memory>
namespace Gbp { namespace Tra {
	class FmtSpec
	{
	public:
		FmtSpec(		
			size_t formatSpecPos,
			const char* formatSpecString,
			int typeIndex,
			size_t typeSize);
		~FmtSpec();
		size_t pos() const {return formatSpecPos_;}
		int typeIndex() const {return typeIndex_;}
		std::string fmt() const {return formatSpecString_;}
		size_t typeSize() const {return typeSize_;}
	private:
		size_t formatSpecPos_;
		std::string formatSpecString_;
		int typeIndex_;
		size_t typeSize_;
	};

	typedef std::shared_ptr<FmtSpec> FmtSpecPtr;
}}

#endif // HEADER_Gbp_Tra_FmtSpec_h
