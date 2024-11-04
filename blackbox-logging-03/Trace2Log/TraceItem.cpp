#include "./TraceItem.h"

namespace Gbp { namespace Tra {
	TraceItem::TraceItem(Sequence_t seq, Sequence_t thrId): 
		seq_(seq),
		thrId_(thrId),
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
		int r = fprintf(f, "%I64u\t0x%04x\t%s\n", seq_, (int)thrId_, value_.c_str());
		return r > 0;
	}

}}
