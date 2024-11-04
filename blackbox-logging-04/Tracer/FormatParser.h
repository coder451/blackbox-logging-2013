#ifndef HEADER_Gbp_Tra_FormatParser_h
#define HEADER_Gbp_Tra_FormatParser_h
#pragma once
#include "./FormatSpec.h"
#include <vector>
#include <stdio.h>

namespace Gbp { namespace Tra {

	class FormatParser
	{
	public:
		FormatParser(size_t paramCount_);
		~FormatParser();
		int parse(const char* p);
		size_t size();
		bool checkFormatSpec(int i, int typeIndex, size_t size);
		bool save(FILE* f);
	private:
		enum ParseAction {PARSEACTION_COUNT, PARSEACTION_SAVE};
		const char* pFmt_;
		size_t maxFormatCount_;
		typedef std::vector<FormatSpec> FormatSpecs;
		FormatSpecs formatSpecs_;
		size_t formatSpecCount_;
		int errIndex_;
		int parse_detail(const char* p, ParseAction);
		bool saveFormatSpec(const FormatSpec& formatSpec);
		bool countFormatSpec(const FormatSpec& formatSpec);
	};

	//////////////////////////////////////////////////////////////////////////

	enum FmtErr 
	{
		FMTERR_NONE,
		FMTERR_TOO_MANY_FORMAT_SPECS,
		FMTERR_FINISHED_IN_FLAGS,
		FMTERR_FINISHED_IN_WIDTH,
		FMTERR_FINISHED_IN_PRECISION,
		FMTERR_UNRECOGNIZED_LENGTH_MODIFIER_AFTER_I3,
		FMTERR_UNRECOGNIZED_LENGTH_MODIFIER_AFTER_I6,
		FMTERR_FINISHED_IN_LENGTH_MODIFIER,
		FMTERR_UNEXPECTED_STATE_AFTER_LENGTH_MODIFIER,
		FMTERR_UNRECOGNIZED_TYPE_CHARACTER,
		FMTERR_COUNT
	};
	extern const char* FmtErrStrings[FMTERR_COUNT];
}}
#endif // HEADER_Gbp_Tra_FormatParser_h
