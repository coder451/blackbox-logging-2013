#include "./TraceItem.h"
#include "./ItemDefn.h"
namespace Gbp { namespace Tra {
	TraceItem::TraceItem(ItemDefn* pItemDefn, Sequence_t seq, Sequence_t thrId, Time_t t):
		pItemDefn_(pItemDefn),
		seq_(seq),
		thrId_(thrId),
		t_(t),
		value_("")
	{
	}

	TraceItem::~TraceItem()
	{
	}

	void TraceItem::addString(const std::string& s)
	{
		value_ += s;
	}

	bool TraceItem::save(FILE* f) const
	{
		int r = fprintf(f, "%I64u\t0x%04x\tt=%0.3f\t[%s(%d), func=%s, fmt=%s]\t%s\n", 
			seq_, (int)thrId_, double(t_)/1000., 
			pItemDefn_->file().c_str(), pItemDefn_->line(),
			pItemDefn_->function().c_str(),
			pItemDefn_->format().c_str(),
			value_.c_str());
		return r > 0;
	}

}}
