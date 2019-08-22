/*
* The MIT License (MIT)
*
* Copyright (c) 2017 Sylko Olzscher
*
*/
#ifndef CYNG_IO_HEX_DUMP_HPP
#define CYNG_IO_HEX_DUMP_HPP

#include <iterator>
#include <iomanip>
#include <boost/assert.hpp>
#include <boost/range.hpp>

namespace cyng 
{
	namespace io 
	{
		template <typename T>
		auto promote_to_printable_integer_type(T i) -> decltype(+i)
		{
			typedef typename std::make_unsigned<T>::type U;
			return +i & std::numeric_limits<U>::max();
		}

		/**	@brief a hex dumper
		 *	Implements a hex dump utility to convert a range to
		 *	an output stream.
		 *
		 *	Example:
		 *	@code
		 *		const char* data;
		 *		size_t size;
		 *		cyng::io::hex_dump hd;
		 *		hd( std::cout, data, data + size );
		 *	@endcode
		 * 
		 * ToDo: improve this code if std::range< T > is available.
		 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3350.html
		 */
		class hex_dump
		{
		public:
			hex_dump(std::size_t w = 8)
				: count_(0)
				, width_(2 * w)
			{}

			template < typename I >
			inline std::ptrdiff_t operator()(std::ostream& stream, I begin, I last)
			{
				//BOOST_ASSERT(begin <= last);
				std::stringstream	ascii_values;
				bool gap = false;
				count_ = 0;
				for (I pos = begin; pos != last; ++pos, ++count_)
				{
					//	detect start of a new line
					if ((count_ % width_) == 0)
					{
						//	off
						gap = false;

						//	dump prefix
						stream
							<< "["
							<< std::setw(4)
							<< std::setfill('0')
							<< std::hex
							<< count_
							<< "] "		//	+2 characters
							<< std::flush	//	7 characters in total
							;

						//	margin
						ascii_values
							<< ' '
							<< ' '
							;
					}

					if (((count_ % width_) != 0) && ((count_ % (width_ / 2)) == 0))
					{
						//	on
						gap = true;

						//	gap
						stream
							<< ' '	//	space
							;

						ascii_values
							<< ' '
							;
					}

					//	print hex value
					stream
						<< ' '		//	space
						<< std::setfill('0')
						<< std::setw(2)
						<< std::hex
						<< promote_to_printable_integer_type(*pos)
						<< std::dec	//	reset
						;

					ascii_values
						<< (((*pos >= 0x20) && (*pos <= 0x7e)) ? *pos : '.')
						;

					//	detect end of line
					if ((count_ > 0) && ((count_ % width_) == (width_ - 1)))
					{
						//	on
						gap = true;

						stream
							<< ascii_values.str()
							<< std::endl
							;
						ascii_values.str("");
					}
				}

				//	print remaining ascii values
				const std::string r = ascii_values.str();
				//	calculate padding spaces
				auto tmp = (width_ - (count_ % width_)) * 3;
				tmp -= 2;
				while (tmp-- > 0)	
				{
					stream << ' ';
				}
				if (!gap)	
				{
					stream << ' ';

				}

				stream
					<< "  "
					<< r
					<< std::endl
					;

				//	doesn't work for file streams
				//BOOST_ASSERT(std::distance(begin, last) == count_);
				return count_;
			}

		private:
			std::ptrdiff_t	count_;
			const std::size_t	width_;	//!<	total width
		};
		
		
		/**
		 * Dump ranges
		 */
 		template<typename CharT, typename Traits, typename Iter>
 		inline std::basic_ostream<CharT, Traits >& operator<<(std::basic_ostream<CharT, Traits>& os, const boost::range_iterator<Iter>& range)
 		{
 			hex_dump hd;
 			hd(os, range.begin(), range.end());
 			return os;
 		}
		
	}	
}	//	cyng

#endif // CYNG_IO_HEX_DUMP_HPP

