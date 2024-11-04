#include "./FormatSpec.h"

namespace Gbp { namespace Tra {

	FormatSpec::FormatSpec(): 
	saveType_(SAVETYPE_NONE),
	pFmt_(0),
	lenFmt_(0),
	fmtChg_(FC_NONE),
	typeIndex_(0),
	size_(0),
	pFmt0_(0)
	{
	}

	void FormatSpec::reset()
	{
		saveType_ = SAVETYPE_NONE;
		pFmt_ = 0;
		lenFmt_ = 0;
	}

	void FormatSpec::fixType(std::string& fmt)
	{
		switch(fmtChg_)
		{
		case FC_NONE:
			break;
		case FC_JUST_TYPE:
			fmt.append(1, fmtTypeChar());
			break;
		case FC_LONG_TYPE:
			fmt.append(1, 'l');
			fmt.append(1, fmtTypeChar());
			break;
		default:
			throw 1;
		}
	}

	bool FormatSpec::save(FILE* f)
	{
		std::string s;
		s.append(pFmt_, lenFmt_);
		int r = fprintf(f, "FormatSpec pos=%d string \"%s\" typeindex=0x%x typeSize=%u\n", pFmt_ - pFmt0_, s.c_str(), typeIndex_, size_);
		if(r < 0) return false;
		return true;
	}
}}