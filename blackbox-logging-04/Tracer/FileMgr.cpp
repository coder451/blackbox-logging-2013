#include "./FileMgr.h"

namespace Gbp { namespace Tra {

	FileMgr::FileMgr(const std::string& baseName, const char* suffix, const char* options): f_(0)
	{
		std::string ftname = baseName + suffix;
#pragma warning(push)
#pragma warning(disable: 4996)
		f_ = ::fopen(ftname.c_str(), options);
#pragma warning(pop)
		if(f_ == 0)
		{
			printf("Could not open %s file.\n", suffix);
		}
	}
	FileMgr::~FileMgr()
	{
		if(f_ != 0)
		{
			fclose(f_);
		}
	}

}}

