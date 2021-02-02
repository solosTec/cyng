/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_IO_HEX_DUMP_HPP
#define DOCC_IO_HEX_DUMP_HPP

#include <ostream>
#include <sstream>
#include <iomanip>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <boost/range.hpp>

namespace docscript {
	namespace io {

		/**	@brief a hex dumper
		 *	Implements a hex dump utility to convert a range to
		 *	an output stream.
		 *
		 *	Example:
		 *	@code
		 *		const char* data;
		 *		size_t size;
		 *		docscript::io::hex_dump hd;
		 *		hd( std::cout, data, data + size );
		 *	@endcode
		 *
		 * ToDo: improve this code if std::range< T > is available.
		 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3350.html
		 */
		template <std::size_t N = 8>
		class hex_dump
		{
			using width_t = std::integral_constant<std::size_t, 2 * N>;

		public:
			hex_dump()
				: ascii_values_()
			{}

			template < typename CharT, typename Traits, typename I >
			std::size_t operator()(std::basic_ostream<CharT, Traits>& stream, I begin, I last) {

				using value_type = typename I::value_type;

				bool gap = false;
				std::size_t	count = 0;

				ascii_values_.str("");

				for (I pos = begin; pos != last; ++pos, ++count)
				{
					//	detect start of a new line
					if ((count % width_t::value) == 0)
					{
						//	off
						gap = false;

						//	dump prefix
						stream
							<< "["
							<< std::setw(4)
							<< std::setfill('0')
							<< std::hex
							<< count
							<< "] "		//	+2 characters
							<< std::flush	//	7 characters in total
							;

						//	margin
						ascii_values_
							<< ' '
							<< ' '
							;
					}

					if (((count % width_t::value) != 0) && ((count % (width_t::value / 2)) == 0))
					{
						//	on
						gap = true;

						//	gap
						stream
							<< ' '	//	space
							;

						ascii_values_
							<< ' '
							;
					}

					//	print hex value
					stream
						<< ' '		//	space
						<< std::setfill('0')
						<< std::setw(2)
						<< std::hex
						<< +(*pos & std::numeric_limits<value_type>::max())
						<< std::dec	//	reset
						;

					ascii_values_
						<< (((*pos >= 0x20) && (*pos <= 0x7e)) ? *pos : '.')
						;

					//	detect end of line
					if ((count > 0) && ((count % width_t::value) == (width_t::value - 1)))
					{
						//	on
						gap = true;

						stream
							<< ascii_values_.str()
							<< std::endl
							;
						ascii_values_.str("");
					}
				}

				//	print remaining ascii values
				auto const r = ascii_values_.str();

				//	calculate padding spaces
				auto tmp = (width_t::value - (count % width_t::value)) * 3;
				tmp -= 2;
				while (tmp-- > 0) {
					stream << ' ';
				}

				if (!gap) {
					stream << ' ';

				}

				stream
					<< "  "
					<< r
					<< std::endl
					;

				return count;
			}

			private:
				std::stringstream	ascii_values_;		
		};

	}
}
#endif
