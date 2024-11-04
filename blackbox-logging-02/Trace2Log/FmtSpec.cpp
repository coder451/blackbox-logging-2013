#include "./FmtSpec.h"
namespace Gbp { namespace Tra {

	FmtSpec::FmtSpec(		
		 size_t formatSpecPos,
		 const char* formatSpecString,
		 int typeIndex,
		 size_t typeSize):
	formatSpecPos_(formatSpecPos),
	formatSpecString_(formatSpecString),
	typeIndex_(typeIndex),
	typeSize_(typeSize)
	{
	}

	FmtSpec::~FmtSpec()
	{
	}
}}
