#ifndef HEADER_Gbp_TraceMacros_h
#define HEADER_Gbp_TraceMacros_h

// Helper needed for MSVC
#define EXPAND(X) X

// Concatenate a variable name with a line number
#define V_N(V, L) V ## L

// Get the number of params in a VA list, OK up to 20 in the list.
#define ARG_COUNT(...) EXPAND(ARG_COUNT_IMPL(__VA_ARGS__, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define ARG_COUNT_IMPL(B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15, B16, B17, B18, B19, N, ...) N

// Creates the TraceSpec instance in a section
#define MAKE_TRACESPEC_IMPL(FREQ, FMT, LINE, ...) \
	__declspec(allocate(".trace")) \
	static Gbp::Tra::TraceSpec V_N(traceSpec_, LINE) = { \
	__FILE__, LINE, __FUNCTION__, \
	FREQ, \
	FMT, \
	Gbp::Tra::TRACESPEC_MAGIC, \
	ARG_COUNT(__VA_ARGS__), \
	0}

// We need this indirection on Windows.
#define MAKE_TRACESPEC(FREQ, FMT, LINE, ...) EXPAND(MAKE_TRACESPEC_IMPL(FREQ, FMT, LINE, __VA_ARGS__))

// Make the function call to write the trace information
#define CALL_TRACE_FUNCTION(LINE, ...) \
	V_N(traceSpec_, LINE).pTraceDefn->trace(__VA_ARGS__)

// Combine the TraceSpec creation with the trace function call.
// This can be treated as a statement. The static is protected by {} to avoid
// warnings when used in switches. 
#define TRACEF(FREQ, FMT, ...) \
	{MAKE_TRACESPEC(FREQ, FMT, __LINE__, __VA_ARGS__); \
	CALL_TRACE_FUNCTION(__LINE__, __VA_ARGS__);}\
	(void)0

#endif // HEADER_Gbp_TraceMacros_h
