#ifndef HEADER_Gbp_Tra_TraceItem_h
#define HEADER_Gbp_Tra_TraceItem_h
#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include <functional>

namespace Gbp { namespace Tra {

	typedef __int64 Sequence_t;
	typedef unsigned __int64 Time_t;
	class TraceItem;
	typedef boost::shared_ptr<TraceItem> TraceItemPtr;
	typedef const TraceItemPtr& TraceItemPtrCRef;
	class ItemDefn;
	
	class TraceItem
	{
	public:
		TraceItem(ItemDefn* pItemDefn, Sequence_t seq, Sequence_t thrId, Time_t t);
		~TraceItem();
		void addString(const std::string& s);
		bool operator<(const TraceItem& a) const {return seq_ < a.seq_;}
		const std::string& value() const {return value_;}
		Sequence_t seq() const {return seq_;}
		bool save(FILE* f) const;
	private:
		ItemDefn* pItemDefn_;
		Sequence_t seq_;
		Sequence_t thrId_;
		Time_t t_;
		std::string value_;
	};

	struct TraceItemPtrLess: public std::binary_function<TraceItemPtrCRef, TraceItemPtrCRef, bool>
	{
		bool operator()(TraceItemPtrCRef a, TraceItemPtrCRef b)
		{
			return *a < *b;
		}
	};
}}
#endif // HEADER_Gbp_Tra_TraceItem_h
