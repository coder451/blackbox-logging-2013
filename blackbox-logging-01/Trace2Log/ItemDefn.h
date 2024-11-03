#ifndef HEADER_Gbp_Tra_ItemDefn_h
#define HEADER_Gbp_Tra_ItemDefn_h
#pragma once
#include <Tracer/TraceTypes.h>
#include <string>
#include <vector>
#include <memory>
#include <stdio.h>
namespace Gbp { namespace Tra {
	class FmtSpec;
	typedef std::shared_ptr<FmtSpec> FmtSpecPtr;
	class TraceItem;
	typedef std::shared_ptr<TraceItem> TraceItemPtr;
	typedef std::vector<TraceItemPtr> TraceItems;

	class ItemDefn
	{
	public:
		ItemDefn(		
			size_t bufferOffset,
			size_t bufferSize,
			size_t lastItemPos,
			size_t oldestPos,
			size_t newestPos,
			size_t totalCount,
			size_t length,
			int hasArrays,
			const char *file,
			int line,
			const char* function,
			const char* format,
			size_t specCount);
		~ItemDefn();
		void addFmtSpec(const FmtSpecPtr& spFmtSpec);
		void load(const std::vector<Slot_t>& wholeBuffer);
		void donateTraceItems(TraceItems&) const;
	private:
		size_t bufferOffset_;
		size_t bufferSize_;
		size_t lastItemPos_;
		size_t oldestPos_;
		size_t newestPos_;
		size_t totalCount_;
		size_t length_;
		int hasArrays_;
		std::string file_;
		int line_;
		std::string function_;
		std::string format_;
		size_t specCount_;
		
		typedef std::vector<FmtSpecPtr> FmtSpecs;
		FmtSpecs fmtSpecs_;
		TraceItems traceItems_;
	};

	typedef std::shared_ptr<ItemDefn> ItemDefnPtr;
}}
#endif // HEADER_Gbp_Tra_ItemDefn_h
