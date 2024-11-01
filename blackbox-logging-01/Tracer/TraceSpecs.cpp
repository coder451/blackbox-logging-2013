#include "./TraceSpecs.h"
#include "./TraceSpec.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// For PE format description structs
#include <WinNT.h>

// For swap
#include <algorithm>

namespace Gbp { namespace Tra {
	TraceSpecs::TraceSpecs(): isValid_(false), p0_(0), size_(0)
	{
		//////////////////////////////////////////////////////////////////////////
		// http://msdn.microsoft.com/en-us/magazine/cc301805.aspx

		// Get the instance handle, i.e., the base address of the module in memory.
		HMODULE h = GetModuleHandle(0);

		// Check the DOS header
		PIMAGE_DOS_HEADER pDosHeader = pDosHeader = (PIMAGE_DOS_HEADER)h;
		if(pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		{
			return;
		}

		PIMAGE_NT_HEADERS32 pHeader = (PIMAGE_NT_HEADERS32)((char *)h + pDosHeader->e_lfanew);
		int sectionCount = pHeader->FileHeader.NumberOfSections;

		// In the PE file, the raw data that comprises each section is guaranteed 
		// to start at a multiple of this value. The default value is 0x200 bytes, 
		// probably to ensure that sections always start at the beginning of a disk 
		// sector (which are also 0x200 bytes in length). Not used.
		// size_t fileAlignment = pHeader->OptionalHeader.FileAlignment;

		// When mapped into memory, each section is guaranteed to start at a 
		// virtual address that's a multiple of this value. We don't actually use it. 
		// size_t sectionAlignment = pHeader->OptionalHeader.SectionAlignment;

		PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)(pHeader + 1);

		// Look for the named section in the PE. // , '-', 's', 'e', 'c', 't', 'i', 'o', 'n'
		BYTE name[] = {'.', 't', 'r', 'a', 'c', 'e'};
		bool found = false;
		for(int i = 0; i < sectionCount; ++i, ++pSection)
		{
			// Section name is not null-terminated.
			if(!EqualBytes(&name[0], &pSection->Name[0], sizeof(name)/sizeof(BYTE)))
			{
				continue;
			}
			found = true;
			break;
		}

		if(!found) return;
		isValid_ = true;

		// pSection now points at the section we were looking for.

		size_ = pSection->Misc.VirtualSize;

		// Compute the start of the section.
		p0_ = (char *)h;
		p0_ += pSection->VirtualAddress;
	}

	TraceSpecs::~TraceSpecs()
	{
	}

	TraceSpecs::iterator 
	TraceSpecs::begin()
	{
		return iterator(this);
	}

	TraceSpecs::iterator 
	TraceSpecs::end()
	{
		return iterator();
	}

	void TraceSpecs::next(char*& p)
	{
		p += sizeof(int);
		for(; p < p0_ + size_; p += sizeof(int))
		{
			if(*((const int*)p) == TRACESPEC_MAGIC) 
			{
				// Now in a TraceSpec
				return;
			}
		}

		// If we get here, we found nothing. Set p_ to zero to indicate the end.
		// This compares equal with the iterator returned by end().
		p = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// Iterator

	TraceSpecs::iterator::iterator(TraceSpecs* pOwner): pOwner_(pOwner), p_(pOwner->p0_)
	{
		pOwner_->next(p_);
	}

	TraceSpecs::iterator::iterator(const TraceSpecs::iterator& a): pOwner_(a.pOwner_), p_(a.p_)
	{
	}

	TraceSpecs::iterator& TraceSpecs::iterator::operator=(const TraceSpecs::iterator& a)
	{
		iterator temp(a);
		std::swap(temp.pOwner_, pOwner_);
		std::swap(temp.p_, p_);
		return *this;
	}

	// Pre
	TraceSpecs::iterator& 
	TraceSpecs::iterator::operator++()
	{
		pOwner_->next(p_);
		return *this;
	}

	// Post
	TraceSpecs::iterator 
	TraceSpecs::iterator::operator++(int)
	{
		iterator temp(*this);
		pOwner_->next(p_);
		return temp;
	}

	TraceSpec* 
	TraceSpecs::iterator::operator->()
	{
		return Gbp::Tra::TraceSpecStart(p_);
	}

	TraceSpec& 
	TraceSpecs::iterator::operator*()
	{
		return *Gbp::Tra::TraceSpecStart(p_);
	}

	bool 
	TraceSpecs::iterator::operator==(const TraceSpecs::iterator& a)
	{
		if(a.p_ == 0 && p_ == 0) return true;
		if(a.p_ == p_) return true;
		return false;
	}

	bool 
	TraceSpecs::iterator::operator!=(const TraceSpecs::iterator& a)
	{
		return !operator==(a);
	}
}}
