#ifndef HEADER_Gbp_Tra_TraceSpec_h
#define HEADER_Gbp_Tra_TraceSpec_h

// Define the segment the TraceSpec objects will go in
#pragma section(".trace", read, write)

namespace Gbp { namespace Tra {
	class TraceDefn;

	struct TraceSpec
	{
		// File name. Initialized before execution
		const char* file;

		// Line number. Initialized before execution
		int line;

		// Function. Initialized before runtime
		const char* function;

		// Frequency spec is used to determine the proportion
		// of space to be used by this. Initialized before execution.
		int freq;

		// The format specifier, similar to printf. initialized before execution.
		const char* fmt;

		// Initialized before execution to magic number to help find
		// specs in the .trace segment. Set to the size of the buffer allocated for this
		// spec.
		int n;

		// The count of parameters
		int paramCount;

		// This carries all the other data
		TraceDefn* pTraceDefn;
	};

	// used to find the TraceSpecs in the PE.
	const int TRACESPEC_MAGIC = 0xeeeeffff;

	// To find the start of a TraceSpec object, given tyhe address of its magic bytes.
	TraceSpec* TraceSpecStart(const char* pn);
	bool EqualBytes(unsigned char* a, unsigned char* b, size_t n);
}}

#endif // HEADER_Gbp_Tra_TraceSpec_h
