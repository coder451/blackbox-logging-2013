#ifndef HEADER_Gbp_Tr_TraceDefn_h
#define HEADER_Gbp_Tr_TraceDefn_h
#pragma once
#include "./Mutex.h"
#include "./FormatParser.h"
#include "./Counter.h"
#include "./Traceable.h"
#include "./TraceHelpers.h"
#include "./SaveValue.h"
#include "./SaveValueWrap.h"
#include <stdio.h>

namespace Gbp { namespace Tra {
	struct TraceSpec;

	class TraceDefn
	{
		// The TraceSpec this came from.
		TraceSpec* pTraceSpec_;

		// The size of the circular buffer for this item
		size_t size_;

		// Offset of this buffer in the total
		size_t offset_;

		// The start of the circular buffer.
		Slot_t* pBuffer0_;

		// Position in the buffer. This is the next position that would be written.
		size_t pos_;

		// The start of the last item written
		size_t prevPos_;

		// The start of the oldest item, not reduced to the buffer size.
		// Also a similar thing for the next position to be written.
		__int64 oldestIndex_;
		__int64 newestIndex_;

		// Details about the format string
		FormatParser formatParser_;

		// Number of times called
		Sequence_t count_;

		// Are there any variable length parameters such as arrays (including strings)?
		bool hasArrays_;

		// The length of the item if it has no arrays, or the minimum length if it does.
		// Arrays are written with a length.
		size_t minLength_;

		// For thread synchronization
		Mutex mutex_;

		static const size_t MAX_PARAM_COUNT = 20;

	public:
		static Sequence_t SequenceNumber();
		TraceDefn(TraceSpec* pTraceSpec, size_t offset, char*& pBuffer);
		~TraceDefn();

		void checkFormatSpec(size_t length, int hasArrays, size_t paramCount, const int* paramTypes, const size_t* paramSizes);
		bool save(FILE* f);

		//////////////////////////////////////////////////////////////////////////
		bool wrappedInSequence(size_t& pos, Slot_t*& pWrite)
		{
			if(pos + SlotCount(sizeof(Sequence_t)) > size_)
			{
				// wrap
				pWrite = pBuffer0_;
				SaveValue(pWrite, SequenceNumber());
				pos = SlotCount(sizeof(Sequence_t));
				return true;
			}
			// Will not wrap here - just write it out.
			SaveValue(pWrite, SequenceNumber());
			pos += SlotCount(sizeof(Sequence_t));
			return false;
		}

		bool wrappedInLength(size_t length,  size_t& pos, Slot_t*& pWrite)
		{
			if(pos + SlotCount(sizeof(size_t)) > size_)
			{
				// wrap
				pWrite = pBuffer0_;
				SaveValue(pWrite, length);
				pos = SlotCount(sizeof(length));
				return true;
			}
			// Will not wrap here - just write it out.
			SaveValue(pWrite, length);
			pos += SlotCount(sizeof(size_t));
			return false;
		}


		template <typename T>
		bool wrappedInParam(const T& t, size_t& pos, Slot_t*& pWrite)
		{
			if(ParamType<T>::hasVarLen == 0)
			{
				if(pos + SlotCount(sizeof(T)) > size_)
				{
					// wrap
					pWrite = pBuffer0_;
					SaveValue(pWrite, t);
					pos = SlotCount(sizeof(T));
					return true;
				}
				SaveValue(pWrite, t);
				pos += SlotCount(sizeof(T));
				return false;
			}

			size_t n1 = SlotCount(sizeof(size_t));
			size_t n2 = SlotCount(ValueSize(t));
			if(pos + n1 + n2 <= size_)
			{
				// no wrap
				SaveValue(pWrite, t);
				pos += n1 + n2;
				return false;
			}

			if(pos + n1 <= size_)
			{
				// wrap in variable part, perhaps at very beginning (n == 0)
				SaveValue(pWrite, ValueSize(t));
				pos += n1;
				size_t n = size_ - pos;
				SaveValueBegin(pWrite, t, n);
				pWrite = pBuffer0_;
				SaveValueEnd(pWrite, t, n);
				pos = SlotCount(ValueSize(t)) - n;
				return true;
			}

			// wrap at size part
			pWrite = pBuffer0_;
			SaveValue(pWrite, t);
			pos = n1 + n2;
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		// Incrementing pos. This increments the position past the item being written.
		// In doing so it will usually update the oldest position at which it is known that
		// an item has been written.
		// We don't bother wrapping. "newestIndex_" is kept up to date by adding the length of each item.
		// It starts negative. Whenever it passes "oldestIndex_", oldestIndex_ is moved forward until it refers to
		// the next item, by using the length that is written into the first slot of each item that has arrays.
		// For items without arrays, only minLength is used.
		void incrementPosition(size_t d)
		{
			newestIndex_ += d;
			pos_ += d;
			if(hasArrays_)
			{
				while(newestIndex_ > oldestIndex_)
				{
					size_t pos = (size_t)(oldestIndex_ % size_);
					oldestIndex_ += *(pBuffer0_ + pos);
				}
				return;
			}

			if(newestIndex_ == oldestIndex_) oldestIndex_ += minLength_;
		}

		//////////////////////////////////////////////////////////////////////////
		void trace()
		{
			MutexMgr m(mutex_);
			if(count_ == 0) checkFormatSpec(1, 0, 0, 0, 0);
			++count_;
			prevPos_ = pos_;
			incrementPosition(1);

			if(pos_ <= size_)
			{
				Slot_t* pWrite = pBuffer0_ + prevPos_;
				SaveValue(pWrite, SequenceNumber());
				return;
			}
			
			pos_ = 1;
			SaveValue(pBuffer0_, SequenceNumber());
		}


		//////////////////////////////////////////////////////////////////////////
		template<typename T0> 
		void trace(const T0& t0)
		{
			(void) sizeof(Gbp::Tra::AreTraceable<T0>);
			static const int HasArrays = ParamType<T0>::hasVarLen;
			static const size_t ParamCount = 1;
			static const size_t MinLength = 1 + ParamCount;
			static const int ParamTypes[ParamCount] = {ParamType<T0>::value, };
			static const size_t ParamSizes[ParamCount] = {sizeof(T0), };

			size_t length = MinLength;
			if(HasArrays != 0)
			{
				if(ParamType<T0>::hasVarLen != 0) length += LengthOf(t0);
				length += 1;
			}

			MutexMgr m(mutex_);	
			if(count_ == 0) 
			{
				checkFormatSpec(length, HasArrays, ParamCount, &ParamTypes[0], &ParamSizes[0]);
			}
			++count_;
			Slot_t* pWrite = pBuffer0_ + pos_;
			size_t pos = pos_;
			prevPos_ = pos;
			incrementPosition(length);

			if(pos_ <= size_)
			{
				if(HasArrays != 0) SaveValue(pWrite, length);
TRACEDEFN_S:	SaveValue(pWrite, SequenceNumber());
TRACEDEFN_0:	SaveValue(pWrite, t0);
				return;
			}

			pos_ -= size_;
			if(HasArrays != 0) if(wrappedInLength(length, pos, pWrite)) goto TRACEDEFN_S;
			if(wrappedInSequence(pos, pWrite)) goto TRACEDEFN_0;
			wrappedInParam(t0, pos, pWrite);
		}

		//////////////////////////////////////////////////////////////////////////
		template<typename T0, typename T1> 
		void trace(const T0& t0, const T1& t1)
		{
			(void) sizeof(Gbp::Tra::AreTraceable<T0, T1>);
			static const int HasArrays = ParamType<T0>::hasVarLen | ParamType<T1>::hasVarLen;
			static const size_t ParamCount = 2;
			static const size_t MinLength = 1 + ParamCount;
			static const int ParamTypes[ParamCount] = {ParamType<T0>::value, ParamType<T1>::value, };
			static const int ParamSizes[ParamCount] = {sizeof(T0), sizeof(T1), };

			size_t length = MinLength;
			if(HasArrays != 0)
			{
				if(ParamType<T0>::hasVarLen != 0) length += LengthOf(t0);
				if(ParamType<T1>::hasVarLen != 0) length += LengthOf(t1);
				++length;
			}

			MutexMgr m(mutex_);	
			if(count_ == 0) checkFormatSpec(length, HasArrays, ParamCount, &ParamTypes[0], &ParamSizes[0]);
			++count_;
			Slot_t* pWrite = pBuffer0_ + pos_;
			size_t pos = pos_;
			prevPos_ = pos;
			incrementPosition(length);

			if(pos_ <= size_)
			{
				if(HasArrays != 0) SaveValue(pWrite, length);
TRACEDEFN_S:	SaveValue(pWrite, SequenceNumber());
TRACEDEFN_0:	SaveValue(pWrite, t0);
TRACEDEFN_1:	SaveValue(pWrite, t1);
				return;
			}

			pos_ -= size_;
			if(HasArrays != 0) if(wrappedInLength(length, pos, pWrite)) goto TRACEDEFN_S;
			if(wrappedInSequence(pos, pWrite)) goto TRACEDEFN_0;
			if(wrappedInParam(t0, pos, pWrite)) goto TRACEDEFN_1;
			wrappedInParam(t1, pos, pWrite);
		}

		//////////////////////////////////////////////////////////////////////////
		template<typename T0, typename T1, typename T2> 
		void trace(const T0& t0, const T1& t1, const T2& t2)
		{
			(void) sizeof(Gbp::Tra::AreTraceable<T0, T1, T2>);
			static const int HasArrays = ParamType<T0>::hasVarLen | ParamType<T1>::hasVarLen | ParamType<T2>::hasVarLen;
			static const size_t ParamCount = 3;
			static const size_t MinLength = 1 + ParamCount;
			static const int ParamTypes[ParamCount] = {ParamType<T0>::value, ParamType<T1>::value, ParamType<T2>::value, };
			static const size_t ParamSizes[ParamCount] = {sizeof(T0), sizeof(T1), sizeof(T2), };

			size_t length = MinLength;
			if(HasArrays != 0)
			{
				if(ParamType<T0>::hasVarLen != 0) length += LengthOf(t0);
				if(ParamType<T1>::hasVarLen != 0) length += LengthOf(t1);
				if(ParamType<T1>::hasVarLen != 0) length += LengthOf(t2);
				length += 1;
			}

			MutexMgr m(mutex_);	
			if(count_ == 0) checkFormatSpec(length, HasArrays, ParamCount, &ParamTypes[0], &ParamSizes[0]);
			++count_;
			Slot_t* pWrite = pBuffer0_ + pos_;
			size_t pos = pos_;
			prevPos_ = pos;
			incrementPosition(length);

			if(pos_ <= size_)
			{
				if(HasArrays != 0) SaveValue(pWrite, length);
TRACEDEFN_S:	SaveValue(pWrite, SequenceNumber());
TRACEDEFN_0:	SaveValue(pWrite, t0);
TRACEDEFN_1:	SaveValue(pWrite, t1);
TRACEDEFN_2:	SaveValue(pWrite, t2);
				return;
			}

			pos_ -= size_;
			if(HasArrays != 0) if(wrappedInLength(length, pos, pWrite)) goto TRACEDEFN_S;
			if(wrappedInSequence(pos, pWrite)) goto TRACEDEFN_0;
			if(wrappedInParam(t0, pos, pWrite)) goto TRACEDEFN_1;
			if(wrappedInParam(t1, pos, pWrite)) goto TRACEDEFN_2;
			wrappedInParam(t2, pos, pWrite);
		}
	};
}}
#endif  // HEADER_Gbp_Tr_TraceDefn_h
