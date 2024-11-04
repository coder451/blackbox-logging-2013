#ifndef HEADER_Gbp_Tra_TraceSpecs_h
#define HEADER_Gbp_Tra_TraceSpecs_h
#pragma once
namespace Gbp { namespace Tra {
	struct TraceSpec;

	class TraceSpecs
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		// Iterator through the TraceSpecs.
		class iterator
		{
		public:
			// Used as "end"
			iterator(): pOwner_(0), p_(0){}
			iterator(TraceSpecs* pOwner);
			~iterator(){}
			iterator(const iterator&);
			iterator& operator=(const iterator&);
			TraceSpec* operator->();
			TraceSpec& operator*();
			bool operator==(const iterator&a);
			bool operator!=(const iterator&a);
			iterator& operator++();
			iterator operator++(int);
		private:
			// The container
			TraceSpecs* pOwner_;
			// Current pointer
			char* p_;
		};
		//////////////////////////////////////////////////////////////////////////
		friend class iterator;

		TraceSpecs();
		~TraceSpecs();
		
		// Check if anything went wrong traversing the PE.
		bool isValid() const {return isValid_;}

		// Get a TraceSpec iterator pointing at the first one.
		void next(char*& p);
		iterator begin();
		iterator end();

	private:
		// Did something go wrong?
		bool isValid_;

		// Start of the section
		char* p0_;

		// Size of the section
		size_t size_;
	};
}}
#endif // HEADER_Gbp_Tra_TraceSpecs_h