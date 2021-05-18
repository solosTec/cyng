/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/io/serializer/plain.hpp>
#include <cyng/io/ostream.h>

#include <boost/io/ios_state.hpp>

#include <iomanip>
#include <algorithm>

namespace cyng {
	namespace io {

		std::size_t serializer <bool, PLAIN>::write(std::ostream& os, bool v)
		{
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);
			os << std::boolalpha << v;
			return cs;
		}

		std::size_t serializer <boost::uuids::uuid, PLAIN>::write(std::ostream& os, boost::uuids::uuid const& v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);

			os
				<< std::hex
				<< std::setfill('0')
				;

			//
			// 	format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
			//	36 characters

			std::size_t idx{ 0 };
			for(auto const c: v) {
				os
					<< std::setw(2)
					<< (+c & 0xFF)	//	promote to integer
					;

				++idx;
				if (idx == 4 || idx == 6 || idx == 8 || idx == 10) {
					os << '-';
				}
			}
			return cs;
		}

		std::size_t serializer <std::int8_t, PLAIN>::write(std::ostream& os, std::int8_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);

			os
				<< std::dec
				<< std::setfill('0')
				<< +v
				;
			return cs;
		}

		std::size_t serializer <std::int16_t, PLAIN>::write(std::ostream& os, std::int16_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);

			os
				<< std::dec
				<< v
				;
			return cs;
		}

		std::size_t serializer <std::int32_t, PLAIN>::write(std::ostream& os, std::int32_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);

			os
				<< std::dec
				<< v
				;
			return cs;
		}

		std::size_t serializer <std::int64_t, PLAIN>::write(std::ostream& os, std::int64_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);

			os
				<< std::dec
				<< v
				;
			return cs;
		}

		std::size_t serializer <std::uint8_t, PLAIN>::write(std::ostream& os, std::uint8_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);

			os
				<< std::hex
				<< std::setfill('0')
				<< +v
				;
			return cs;
		}
		std::size_t serializer <std::uint16_t, PLAIN>::write(std::ostream& os, std::uint16_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);

			os
				<< std::hex
				<< std::setfill('0')
				<< std::setw(4)
				<< v
				;
			return cs;
		}
		std::size_t serializer <std::uint32_t, PLAIN>::write(std::ostream& os, std::uint32_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);

			os
				<< std::hex
				<< std::setfill('0')
				<< std::setw(8)
				<< v
				;
			return cs;
		}

		std::size_t serializer <std::uint64_t, PLAIN>::write(std::ostream& os, std::uint64_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);

			os
				<< std::hex
				<< std::setfill('0')
				<< std::setw(16)
				<< v
				;
			return cs;
		}

		std::size_t serializer <double, PLAIN>::write(std::ostream& os, double v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			calc_size const cs(os);

			os
				<< std::setprecision(2)
				<< std::fixed
				<< v
				;
			return cs;
		}
		
		//std::size_t serializer <obis_path_t, PLAIN>::write(std::ostream& os, obis_path_t const& v) 
		//{
		//	calc_size const cs(os);
		//	bool init = false;
		//	for (auto const& o : v) {
		//		if (!init) {
		//			init = true;
		//		}
		//		else {
		//			os << ":";
		//		}
		//		os << o;
		//	}
		//	return cs;
		//}

	}
}
