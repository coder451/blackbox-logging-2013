#include "./FormatParser.h"

namespace Gbp { namespace Tra {
	const char* FmtErrStrings[FMTERR_COUNT] =
	{
		"",
		"The format string requires more parameters than are supplied. A and B count for two.",
		"The format finished in the flags section of a format specifier.",
		"The format finished in the width section of a format specifier.",
		"The format finished in the precision section of a format specifier.",
		"Unrecognized length modifier after seeing I3",
		"Unrecognized length modifier after seeing I6",
		"Format finished in length modifier.",
		"Programming error- reached an unexpected state after the length modifier.",
		"The type character was not recognized."
	};

	FormatParser::FormatParser(size_t maxFormatCount):
		pFmt_(0),
		maxFormatCount_(maxFormatCount),
		formatSpecs_(),
		formatSpecCount_(0),
		errIndex_(0)
	{
	}

	FormatParser::~FormatParser()
	{
	}

	int FormatParser::parse(const char* p)
	{
		pFmt_ = p;
		int r = parse_detail(p, PARSEACTION_COUNT);
		if(r != FMTERR_NONE) return r;
		formatSpecs_.resize(formatSpecCount_);
		r = parse_detail(p, PARSEACTION_SAVE);
		return r;
	}

	int FormatParser::parse_detail(const char* p, ParseAction action)
	{
		FormatSpec fmtSpec;
		fmtSpec.fmtStart0(p);
		formatSpecCount_ = 0;

		enum State
		{
			S_NORMAL_CHAR,
			S_IN_FORMAT_SPEC,
			S_HAVE_H,
			S_HAVE_LLOWER,
			S_HAVE_I,
			S_HAVE_I3,
			S_HAVE_I6
		};

		State state = S_NORMAL_CHAR;
		for(; *p != 0; )
		{
			switch(state)
			{
			case S_NORMAL_CHAR:
				switch(*p)
				{
				case '%': state = S_IN_FORMAT_SPEC; ++p; continue;
				default: ++p; continue;
				}
			case S_IN_FORMAT_SPEC:
				switch(*p)
				{
				case '%': state = S_NORMAL_CHAR; ++p; continue;
				default:
					// Flags
					for(bool done = false; !done && *p != 0;)
					{
						switch(*p)
						{
						case '+': case '-': case '0': case ' ': case '#': ++p; continue;
						default: done = true; break;
						}
					}
					if(*p == 0) 
					{
						errIndex_ = (int)formatSpecCount_;
						return FMTERR_FINISHED_IN_FLAGS;
					}

					// Width
					for(bool done = false; !done && *p != 0;)
					{
						switch(*p)
						{
						case '1': case '2': case '3': case '4': case '5':
						case '6': case '7': case '8': case '9':
							++p;
							continue;
						default:
							done = true;
							break;
						}
					}
					if(*p == 0) 
					{
						errIndex_ = (int)formatSpecCount_;
						return FMTERR_FINISHED_IN_WIDTH;
					}

					// Precision
					if(*p == '.')
					{
						++p;
						if(*p == 0) 
						{
							errIndex_ = (int)formatSpecCount_;
							return FMTERR_FINISHED_IN_PRECISION;
						}
						for(bool done = false;!done && *p != 0;)
						{
							switch(*p)
							{
							case '1': case '2': case '3': case '4': case '5':
							case '6': case '7': case '8': case '9': case '0':
								++p;
								continue;
							default:
								done = true;
								break;
							}
						}
						if(*p == 0) 
						{
							errIndex_ = (int)formatSpecCount_;
							return FMTERR_FINISHED_IN_PRECISION;
						}
					}

					// Size
					for(bool done = false;!done && *p != 0;)
					{
						switch(state)
						{
						case S_IN_FORMAT_SPEC:
							switch(*p)
							{
							case 'h': state = S_HAVE_H; fmtSpec.fmtStart(p); ++p; break;
							case 'l': state = S_HAVE_LLOWER; fmtSpec.fmtStart(p); ++p; break;
							case 'L': fmtSpec.fmtStart(p); ++p; break;
							case 'z': fmtSpec.fmtStart(p); ++p; done = true; break;
							case 'j': fmtSpec.fmtStart(p); ++p; done = true; break;
							case 'I': state = S_HAVE_I; fmtSpec.fmtStart(p); ++p; break;
							case 'w': fmtSpec.fmtStart(p); ++p; break;
							default: done = true; break;
							}
							break;
						case S_HAVE_H:
							switch(*p)
							{
							case 'h': ++p; break;
							default: break;
							}
							done = true;
							state = S_IN_FORMAT_SPEC;
							break;
						case S_HAVE_LLOWER:
							switch(*p)
							{
							case 'l': ++p; break;
							default: break;
							}
							done = true;
							state = S_IN_FORMAT_SPEC;
							break;
						case S_HAVE_I:
							switch(*p)
							{
							case '3': ++p; state = S_HAVE_I3; break;
							case '6': ++p; state = S_HAVE_I6; break;
							default: done = true; state = S_IN_FORMAT_SPEC; break;
							}
							break;
						case S_HAVE_I3:
							switch(*p)
							{
							case '2': ++p; done = true; state = S_IN_FORMAT_SPEC; break;
							default: errIndex_ = (int)formatSpecCount_; return FMTERR_UNRECOGNIZED_LENGTH_MODIFIER_AFTER_I3; 
							}
							break;
						case S_HAVE_I6:
							switch(*p)
							{
							case '4': ++p; done = true; state = S_IN_FORMAT_SPEC; break;
							default: errIndex_ = (int)formatSpecCount_; return FMTERR_UNRECOGNIZED_LENGTH_MODIFIER_AFTER_I6; 
							}
							break;
						default:
							// Cannot happen
							break;
						}
					}
					if(*p == 0) 
					{
						errIndex_ = (int)formatSpecCount_;
						return FMTERR_FINISHED_IN_LENGTH_MODIFIER;
					}
					if(state != S_IN_FORMAT_SPEC)
					{
						errIndex_ = (int)formatSpecCount_;
						return FMTERR_UNEXPECTED_STATE_AFTER_LENGTH_MODIFIER;
					}

					// Finally, the single character that determines the type:
					switch(*p)
					{
					case 'c': case 'C':
						fmtSpec.saveType(SAVETYPE_DIRECT);
						if(fmtSpec.fmtStart() == 0) fmtSpec.fmtStart(p);
						++p;
						fmtSpec.lenFmtFromPtr(p);
						break;
					case 'd': case 'i': case 'o': case 'u': case 'x': case 'X': case 'm':
						fmtSpec.saveType(SAVETYPE_DIRECT);
						if(fmtSpec.fmtStart() == 0) fmtSpec.fmtStart(p);
						++p;
						fmtSpec.lenFmtFromPtr(p);
						break;
					case 'f': case 'e': case 'E': case 'g': case 'G': 
						// In c99, this case would also include %a and %A to print a double in hex.
						// AcceleNet does not support that.
						fmtSpec.saveType(SAVETYPE_DIRECT);
						if(fmtSpec.fmtStart() == 0) fmtSpec.fmtStart(p);
						++p;
						fmtSpec.lenFmtFromPtr(p);
						break;
					case 's':
						// caters for %s and %ws.
						fmtSpec.saveType(SAVETYPE_SZ_ARRAY);
						if(fmtSpec.fmtStart() == 0) fmtSpec.fmtStart(p);
						++p;
						fmtSpec.lenFmtFromPtr(p);
						break;
					case 'S':
						// In AcceleNet, %S always means wchar_t string.
						fmtSpec.saveType(SAVETYPE_SZ_ARRAY);
						if(fmtSpec.fmtStart() == 0) fmtSpec.fmtStart(p);
						++p;
						fmtSpec.lenFmtFromPtr(p);
						break;
					case 't':
						// This means a string in the current width.
						fmtSpec.saveType(SAVETYPE_SZ_ARRAY);
						if(fmtSpec.fmtStart() == 0) fmtSpec.fmtStart(p);
						++p;
						fmtSpec.lenFmtFromPtr(p);
						break;
					case 'A': case 'B':
						// Outputs a block of data by writing a length and the data to the output.
						// b and b write each character in hex. A and a replace printable characters.
						// These are always an array of char. They absorb two parameters from the 
						// passed-in parameter list.
						fmtSpec.saveType(SAVETYPE_ARRAY_COUNT);
						if(fmtSpec.fmtStart() == 0) fmtSpec.fmtStart(p);
						++p;
						fmtSpec.lenFmtFromPtr(p);
						break;
					case 'n': case 'p':
						fmtSpec.saveType(SAVETYPE_DIRECT);
						if(fmtSpec.fmtStart() == 0) fmtSpec.fmtStart(p);
						++p;
						fmtSpec.lenFmtFromPtr(p);
						break;
					default:
						errIndex_ = (int)formatSpecCount_;
						return FMTERR_UNRECOGNIZED_TYPE_CHARACTER;
					} // End of the switch on format spec type

					bool result = false;
					switch(action)
					{
					case PARSEACTION_COUNT:
						result = countFormatSpec(fmtSpec);
						break;
					case PARSEACTION_SAVE:
						result = saveFormatSpec(fmtSpec);
						break;
					default:
						break;
					}
					if(!result) return FMTERR_TOO_MANY_FORMAT_SPECS;

					fmtSpec.reset();

					// And we are back into normal text.
					state = S_NORMAL_CHAR;
					break;
				} // End of of main char case statement in IN_FMT state
			} // End of main "state" case statement
		} // End of main character processing loop
		return FMTERR_NONE;
	} // End of parser function

	bool FormatParser::saveFormatSpec(const FormatSpec& fmtSpec)
	{
		++formatSpecCount_;
		if(formatSpecCount_ > maxFormatCount_)
		{
			errIndex_ = (int)formatSpecCount_;
			return false;
		}
		formatSpecs_[formatSpecCount_ - 1] = fmtSpec;
		if(fmtSpec.saveType() == SAVETYPE_ARRAY_COUNT)
		{
			// When a format spec requires two parameters, add a dummy format spec for the second parameter.
			++formatSpecCount_;
			if(formatSpecCount_ > maxFormatCount_)
			{
				errIndex_ = (int)formatSpecCount_;
				return false;
			}
			FormatSpec& extraFmtSpec = formatSpecs_[formatSpecCount_ - 1];
			extraFmtSpec.saveType(SAVETYPE_COUNTED_ARRAY);
			extraFmtSpec.fmtStart0(pFmt_);
		}
		return true;
	}

	bool FormatParser::countFormatSpec(const FormatSpec& fmtSpec)
	{
		++formatSpecCount_;
		if(formatSpecCount_ > maxFormatCount_)
		{
			errIndex_ = (int)formatSpecCount_;
			return false;
		}
		if(fmtSpec.saveType() == SAVETYPE_ARRAY_COUNT)
		{
			// When a format spec requires two parameters, add a dummy format spec for the second parameter.
			++formatSpecCount_;
			if(formatSpecCount_ > maxFormatCount_)
			{
				errIndex_ = (int)formatSpecCount_;
				return false;
			}
		}
		return true;
	}

	size_t FormatParser::size()
	{
		return formatSpecs_.size();
	}

	bool FormatParser::checkFormatSpec(int i, int typeIndex, size_t size)
	{
		if(i >= (int)formatSpecs_.size()) return false;
		FormatSpec& fs = formatSpecs_[i];
		fs.typeIndex(typeIndex);
		fs.size(size);
		return true;
	}

	bool FormatParser::save(FILE* f)
	{
		int r = fprintf(f, "FormatParser SpecCount=%u\n", formatSpecs_.size());
		if(r < 0) return false;
		for(FormatSpecs::iterator i = formatSpecs_.begin(); i != formatSpecs_.end(); ++i)
		{
			if(!i->save(f)) return false;
		}
		return true;
	}
}}
