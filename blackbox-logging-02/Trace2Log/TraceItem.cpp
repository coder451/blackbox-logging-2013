#include "./TraceItem.h"

namespace Gbp { namespace Tra {
	TraceItem::TraceItem(Sequence_t seq): 
		seq_(seq),
		value_()
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
		int r = fprintf(f, "%I64u: %s\n", seq_, value_.c_str());
		return r > 0;
	}

}}
