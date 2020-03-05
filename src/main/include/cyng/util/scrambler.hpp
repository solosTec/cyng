/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Sylko Olzscher
 *
 */
#ifndef CYNG_UTIL_SCRAMBLER_HPP
#define CYNG_UTIL_SCRAMBLER_HPP

#include <cyng/util/rotating_counter.hpp>
#include <cstring>

namespace cyng 
{
	namespace crypto 
	{
		/**
		 * The purpose of this template class is to provide to simplify XORing
		 * over a range of data with a fixed mask. The values of the mask are stored
		 * in the class member key_.
		 * 
		 * T is typically of type char or unsigned char. AT least the XOR operator ^ 
		 * has to be defined for this data type.
		 */
		template <typename T, std::size_t N>
		class scrambler
		{
		public:
			typedef circular_counter< std::size_t, 0, N - 1 >	index_type;
			typedef std::array< T, N >	key_type;

		public:
			scrambler()
				: key_()
				, counter_()
			{}

			scrambler(key_type const& k)
				: key_(k)
				, counter_()
			{}

			scrambler(key_type&& k)
				: key_(std::move(k))
				, counter_()
			{}

			/**
			 *	Set new scramble key. Includes a counter reset.
			 *
			 *	@param key new scramble key
			 */
			scrambler& operator=(key_type const& key)
			{
				key_ = key;
				counter_.reset();
				return *this;
			}

			/**
			 *	Scrambles a single value.
			 *
			 *	@param value value to de/scramble
			 *	example:
			 *	@code

			 scrambler<char, 16> s;
			 char c = 'a';
			 //	scramble value c with XOR
			 c = s[ c ];

			 *	@endcode
			 */
			T operator[](T value)
			{
				const typename index_type::counter_type idx = counter_();
				BOOST_ASSERT_MSG(idx < N, "index out of range");

				const T x = key_[idx];
				return value ^ x;
			}

			/**
			 *	Scrambles the specified range.
			 *  The value type of the iterators have to match type T.
			 *
			 *	@param first first element of range (included)
			 *	@param last element of range (excluded)
			 */
			template < typename I >
			void operator()(I pos, I end)
			{
				BOOST_ASSERT_MSG(pos <= end, "invalid range");
				while (pos != end)
				{
					*pos = (*this)[*pos];
					++pos;
				}
			}

			/**
			 * @return the underlying key
			 */
			key_type const& key() const
			{
				return key_;
			}

			void reset(T v = T())
			{
				counter_.reset();
				key_.fill(v);
			}

		private:
			key_type	key_;
			index_type	counter_;

		};
	}
}

#endif

