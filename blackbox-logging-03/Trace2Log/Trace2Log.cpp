// Turn a tracefile into a log file.
#include <Tracer/Trace.h>
#include "./ItemDefn.h"
#include "./FmtSpec.h"
#include "./TraceItem.h"
#include <string>
#include <stdio.h>
#include <algorithm>

using Gbp::Tra::Tracer;
using Gbp::Tra::TracerEx;
using Gbp::Tra::ItemDefn;
using Gbp::Tra::ItemDefnPtr;
using Gbp::Tra::FmtSpec;
using Gbp::Tra::FmtSpecPtr;
using Gbp::Tra::Slot_t;
using Gbp::Tra::TraceItems;

#pragma warning(disable: 4996)

bool FilterTrace(FILE*, FILE*, FILE*);

int main(int argc, char* argv[])
{
	std::string fname(argv[1]);
	std::string ftname = fname +".ttra";
	FILE* ft = ::fopen(ftname.c_str(), "r");
	if(ft == 0)
	{
		printf("Could not open text input\n");
		return 1;
	}

	std::string fbname = fname +".btra";
	FILE* fb = ::fopen(fbname.c_str(), "rb");
	if(fb == 0)
	{
		printf("Could not open binary input\n");
		return 1;
	}

	FILE* fout = stdout;
	if(argc > 2)
	{
		fout = ::fopen(argv[2], "w");
		if(fout == 0)
		{
			printf("Could not open output.\n");
			return 1;
		}
	}

	FilterTrace(ft, fb, fout);

	return 0;
}

bool FilterTrace(FILE* ft, FILE* fb, FILE* fout)
{
	typedef std::vector<ItemDefnPtr> ItemDefns;
	ItemDefns itemDefns;

	size_t dataSize = 0;
	size_t defnCount = 0;
	int n1 = fscanf(ft, "Tracer BufferSize=%10zu DefnCount=%10zu\n", &dataSize, &defnCount);
	if(n1 != 2)
	{
		printf("Scan error in Tracer, n=%d\n", n1);
		return false;
	}
	itemDefns.reserve(defnCount);

	for(size_t i = 0; i < defnCount; ++i)
	{
		size_t bufferOffset = 0;
		size_t bufferSize = 0;
		size_t lastItemPos = 0;
		int oldestPos = 0;
		int newestPos = 0;
		size_t totalCount = 0;
		size_t length = 0;
		int hasArrays = 0;
		char file[500] = {};
		int line = 0;
		char function[500] = {};
		char format[500] = {};

		int n2 = fscanf(ft, "TraceDefn offset=%10zu sz=%10zu last=%10zu oldestIndex=%10d newestIndex=%10d "
			"count=%32zu length=%10zu hasArrays=%2d file \"%[^\"]\" line=%10d function \"%[^\"]\" format=%[^\n]\n", 
			&bufferOffset, &bufferSize, &lastItemPos, &oldestPos, &newestPos,
			&totalCount, &length, &hasArrays, &file[0], &line, &function[0], &format[0]);

		if(n2 != 12) 
		{
			printf("Scan error in TraceDefn, n=%d\n", n2);
			return false;
		}

		size_t specCount = 0;

		int n3 = fscanf(ft, "FormatParser SpecCount=%10zu\n", &specCount);

		if(n3 != 1) 
		{
			printf("Scan error in FormatParser, n=%d\n", n3);
			return false;
		}

		ItemDefnPtr spItemDefn(new ItemDefn(bufferOffset, bufferSize, lastItemPos, 
			oldestPos, newestPos,
			totalCount, length, hasArrays, &file[0], line, 
			&function[0], &format[0], specCount));

		for(size_t j = 0; j < specCount; ++j)
		{
			size_t formatSpecPos = 0;
			char formatSpecString[10] = {};
			int typeIndex = 0;
			size_t typeSize = 0;

			int n4 = fscanf(ft, "FormatSpec pos=%10zu string \"%[^\"]\" typeindex=%13x typeSize=%10zu\n", 
				&formatSpecPos, &formatSpecString[0], &typeIndex, &typeSize);

			if(n4 != 4) 
			{
				printf("Scan error in FormatSpec, n=%d\n", n4);
				return false;
			}

			FmtSpecPtr spFmtSpec(new FmtSpec(formatSpecPos, &formatSpecString[0], typeIndex, typeSize));
			spItemDefn->addFmtSpec(spFmtSpec);
		}

		itemDefns.push_back(spItemDefn);
	}
	fclose(ft);

	std::vector<Slot_t> buffer(dataSize);
	size_t n5 = fread(&buffer[0], sizeof(Slot_t), dataSize, fb);
	if(n5 < dataSize)
	{
		if(ferror(fb)) 
		{
			printf("Error reading binary file, read %zu\n", n5);
			return false;
		}
		else
		{
			printf("Binary file truncated, dataSize=%zu\n", n5);
			dataSize = n5;
		}
	}
	fclose(fb);

	TraceItems traceItems;
	for(size_t i = 0; i < defnCount; ++i)
	{
		itemDefns[i]->load(buffer);
		itemDefns[i]->donateTraceItems(traceItems);
	}

	std::sort(traceItems.begin(), traceItems.end(), Gbp::Tra::TraceItemPtrLess());

	for(TraceItems::iterator i = traceItems.begin(); i != traceItems.end(); ++i)
	{
		if(!(*i)->save(fout)) 
		{
			printf("Error writing output file.\n");
			return false;
		}
	}

	return true;
}

