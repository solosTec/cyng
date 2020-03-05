/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Sylko Olzscher
 *
 */
#ifndef CYNG_UTIL_ROTATING_COUNTER_HPP
#define CYNG_UTIL_ROTATING_COUNTER_HPP

#include <boost/assert.hpp>

namespace cyng 
{
	
	/**
	 *	iota() as iterator
	 */
	template < typename T, T FIRST, T LAST >
	class circular_counter
	{
	public:
		typedef	T counter_type;

	public:
		circular_counter()
			: index_(FIRST)
		{
			static_assert(FIRST != LAST, "invalid range");
		}

		circular_counter(circular_counter const& other)
			: index_(other.index_)
		{}

		/**
		 *	@return current element
		 */
		T operator*() const	
		{
			return index_;
		}

		/**
		 *	Reset internal position to initial state.
		 */
		void reset()	
		{
			index_ = FIRST;
		}

		/**
		 *	@return next element of range
		 *	not thread safe(!)
		 */
		T operator()()	
		{
			if (index_ == LAST)	
			{
				T tmp = index_;
				index_ = FIRST;
				return tmp;
			}
			BOOST_ASSERT_MSG(index_ < LAST, "counter out of range");
			return index_++;
		}

	private:
		T index_;
	};

	/**
	 * Generate a sequence of linear rising numbers (start, end]. After
	 * reaching one element before end it starts from the start 
	 * value again.
	 * 
	 * Works best with POD types.
	 */
	template < typename T>
	class ring_counter
	{
	public:
		ring_counter(T start, T end)
		: start_(start) 
		, end_(end)
		, pos_(start)
		{
			BOOST_ASSERT_MSG(start_ < end_, "invalid range");
		}
		
		/**
		 * Possible overflow!
		 * 
		 *	@return next element of range
		 */
		T operator()()	
		{
			const T tmp = (pos_ % end_);
			
			if (tmp < pos_)
			{
				//
				//	go back to start
				//
				pos_ = start_;
			}
			pos_++;
			
			return tmp;
		}
		
		/**
		 *	@return current element
		 */
		T operator*() const	
		{
			return (pos_ == end_)
			? start_ 
			: pos_
			;
		}
		
		
	private:
		T const start_;
		T const end_;
		T pos_;
	};
	
}	//	cyng

#endif
