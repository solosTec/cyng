/*
* The MIT License (MIT)
*
* Copyright (c) 2020 Sylko Olzscher
*
*/
#ifndef CYNG_IO_CPP_DUMP_HPP
#define CYNG_IO_CPP_DUMP_HPP

#include "hex_dump.hpp"

namespace cyng 
{
	namespace io 
	{

		/**	@brief a cpp dumper
		 *	Implements a cpp dump utility to convert a range of binary data
		 *	to C++ code
		 *
		 *	Example:
		 *	@code
		 *		const char* data;
		 *		size_t size;
		 *		cyng::io::cpp_dump cd;
		 *		cd( std::cout, data, data + size, "var" );
		 *	@endcode
		 * 
		 */
		class cpp_dump
		{
		public:
			cpp_dump(std::size_t w = 8)
				: count_(0)
				, width_(2 * w)
			{}

			template < typename I >
			inline std::ptrdiff_t operator()(std::ostream& stream, I begin, I last, std::string name)
			{
				stream 
					<< "auto " 
					<< name 
					<< " = cyng::make_buffer ({" 
					<< std::endl
					<< std::hex
					;

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
							<< "/*"
							<< std::setw(4)
							<< std::setfill('0')
							<< std::hex
							<< count_
							<< "*/ "
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

					}

					if (count_ != 0) {
						stream << ",";
					}

					//	print hex value
					stream
						<< " 0x"
						<< std::setfill('0')
						<< std::setw(2)
						<< promote_to_printable_integer_type(*pos)
						;

					//	detect end of line
					if ((count_ > 0) && ((count_ % width_) == (width_ - 1)))
					{
						//	on
						gap = true;

						stream
							<< std::endl
							;
					}
				}

				stream << "});" << std::endl;
				return count_;
			}

		private:
			std::ptrdiff_t	count_;
			const std::size_t	width_;	//!<	total width
		};
		
		
		/**
		 * Dump ranges
		 */
 		//template<typename CharT, typename Traits, typename Iter>
 		//inline std::basic_ostream<CharT, Traits >& operator<<(std::basic_ostream<CharT, Traits>& os, const boost::range_iterator<Iter>& range)
 		//{
 		//	hex_dump hd;
 		//	hd(os, range.begin(), range.end());
 		//	return os;
 		//}
		
	}	
}	//	cyng

#endif // CYNG_IO_HEX_DUMP_HPP

