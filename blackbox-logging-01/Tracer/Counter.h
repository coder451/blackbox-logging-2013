#ifndef HEADER_Gbp_Tra_Counter_h
#define HEADER_Gbp_Tra_Counter_h
#pragma once

// Returns a sequence in a thread-safe manner. Needs a Linux implementation.

namespace Gbp { namespace Tra {
#ifdef _WIN32
	typedef __int64 Sequence_t;
#else
#error platform
#endif

	class Counter
	{
	public:
		Counter();
		~Counter();

		// Synchronized
		Sequence_t increment();
		
		// No sync here
		Sequence_t value(){return a_;}

	private:
		volatile Sequence_t a_;
	};
}}
#endif // HEADER_Gbp_Tra_Counter_h