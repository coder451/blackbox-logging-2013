#include "./ItemDefn.h"
#include "./FmtSpec.h"
#include "./TraceItem.h"
#include <Tracer/Trace.h>
#pragma warning(disable:4996)
namespace Gbp { namespace Tra {

	ItemDefn::ItemDefn(		
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
		size_t specCount):
	bufferOffset_(bufferOffset),
		bufferSize_(bufferSize),
		lastItemPos_(lastItemPos),
		oldestPos_(oldestPos),
		newestPos_(newestPos),
		totalCount_(totalCount),
		length_(length),
		hasArrays_(hasArrays),
		file_(file),
		line_(line),
		function_(function),
		format_(format),
		specCount_(specCount),
		fmtSpecs_()
	{
		fmtSpecs_.reserve(specCount);
	}

	ItemDefn::~ItemDefn()
	{
	}

	void ItemDefn::addFmtSpec(const FmtSpecPtr& spFmtSpec)
	{
		fmtSpecs_.push_back(spFmtSpec);
	}

	void ItemDefn::load(const std::vector<Slot_t>& wholeBuffer)
	{
		const Slot_t* buffer = &wholeBuffer[bufferOffset_];

		// When there are arrays, this can change for each item. Otherwise it is constant.
		size_t itemLength = length_;

		// Scan the circular buffer, starting at the most recent.
		for(size_t pos = oldestPos_; pos != newestPos_;)
		{
			// The start of the current item.
			size_t posCur = pos;

			if(hasArrays_) 
			{
				itemLength = (size_t)buffer[pos];
				pos = (pos < bufferSize_ - 1)? ++pos : 0;
			}

			Sequence_t seq = buffer[pos];
			pos = (pos < bufferSize_ - 1)? ++pos : 0;

			Sequence_t thrId = buffer[pos];
			pos = (pos < bufferSize_ - 1)? ++pos : 0;

			TraceItemPtr spItem(new TraceItem(seq, thrId));
			traceItems_.push_back(spItem);

			if(specCount_ == 0)
			{
				spItem->addString(format_);
				if(posCur == newestPos_) break;
				continue;
			}

			size_t cur = 0;
			for(size_t i = 0; i < specCount_; ++i)
			{
				FmtSpec& spec = *fmtSpecs_[i];
				std::string fmtSegment = format_.substr(cur, spec.pos() - 1 - cur);
				cur = spec.pos() + spec.fmt().size();
				spItem->addString(fmtSegment);

				switch(spec.typeIndex())
				{
				case PARAM_STD_STRING:
					{
						// This is the size of the item.
						size_t n = (size_t)buffer[pos];
						assert(n <= 7);
						pos = (pos < bufferSize_ - 1)? ++pos : 0;

						if(pos + SlotCount(n) <= bufferSize_)
						{
							std::string s((const char*)&buffer[pos], n);
							spItem->addString(s);
							pos += SlotCount(n);
							if(pos == bufferSize_) pos = 0;
						}
						else
						{
							// The content goes all the way to the end of the buffer, the number of characters in it is:
							size_t m = (bufferSize_ - pos) * sizeof(Slot_t);
							// Make the partial string
							std::string s1((const char*)&buffer[pos], m);
							// Reduce the number of characters still to copy by the number so far written.
							n -= m;
							// Make the second part of the string from the start of the buffer.
							std::string s2((const char*)&buffer[0], n);
							// Set pos to be just after the data read.
							pos = SlotCount(n);

							// Add the partial strings.
							spItem->addString(s1);
							spItem->addString(s2);
						}
					}
					break;
				default:
					{
						std::vector<char> v(32);
						std::string fmt("%");
						fmt += spec.fmt();
						Slot_t value = buffer[pos];
						pos = (pos < bufferSize_ - 1)? ++pos : 0;
						int m = 0;
						switch(spec.typeSize())
						{
						case 4:
							{
								int v4 = (int)value;
								m = sprintf(&v[0], fmt.c_str(), v4);
							}
							break;
						case 8:
							m = sprintf(&v[0], fmt.c_str(), value);
							break;
						}
						std::string s(&v[0], m);
						spItem->addString(s);
					}
					break;
				}
			}
		}
	}

	void ItemDefn::donateTraceItems(TraceItems& v) const
	{
		v.insert(v.end(), traceItems_.begin(), traceItems_.end());
	}
}}
