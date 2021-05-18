/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/io/serializer/csv.hpp>
#include <cyng/io/serializer/plain.hpp>
#include <cyng/io/serialize.h>
#include <cyng/obj/tag.hpp>

#include <boost/io/ios_state.hpp>

namespace cyng {
	namespace io {

		std::size_t serializer <bool, CSV>::write(std::ostream& os, bool v)
		{
			return serializer<bool, PLAIN>::write(os, v);
		}

		std::size_t serializer <vector_t, CSV>::write(std::ostream& os, vector_t const& vec)
		{
			calc_size const cs(os);

			//	serialize each element from the tuple
			bool init = false;

			for (auto const& obj : vec) {
				if (!init) {
					init = true;
				}
				else {
					auto const tag = obj.rtti().tag();
					switch (tag)
					{
					case TC_TUPLE:
					case TC_VECTOR:
					case TC_DEQUE:
						break;
					default:
						os << ", ";
						break;
					}

				}
				serialize_csv(os, obj);
			}

			return cs;
		}

		std::size_t serializer <tuple_t, CSV>::write(std::ostream& os, tuple_t const& tpl)
		{
			calc_size const cs(os);

			//	serialize each element from the tuple
			bool init = false;

			for (auto const& obj : tpl) {
				if (!init) {
					init = true;
				}
				else {
					os << ", ";
				}
				serialize_csv(os, obj);
			}

			//
			//	next line
			//
			os << std::endl;
			return cs;
		}

	}
}
