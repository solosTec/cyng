/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/io/serializer/json_walker.h>
#include <cyng/io/serialize.h>
#include <cyng/io/ostream.h>
#include <cyng/obj/container_cast.hpp>

namespace cyng {
	namespace io {

		json_walker::json_walker(std::ostream& os)
			: os_(os)
			, nl_(true)
		{}

		void json_walker::visit(object const& obj, type_code, std::size_t depth, walker_state state, bool is_vector) {
			if (is_vector)	os_ << std::string(depth * 2, ' ');
			serialize_json(os_, obj);
			if (state != walker_state::LAST) {
				os_ << ", ";
			}
			os_ << std::endl;
			nl_ = true;
		}
		void json_walker::open(type_code tag, std::size_t depth, std::size_t size) {
			switch (tag) {
			case TC_TUPLE:
			case TC_DEQUE:
			case TC_PARAM_MAP:
			case TC_ATTR_MAP:
				if (nl_)	os_ << std::string(depth * 2, ' ');

				os_ << "{";
				if (size > 1) {
					os_ << std::endl;
					nl_ = true;
				}
				else {
					nl_ = false;
				}
				break;
			case TC_VECTOR:
				if (nl_)	os_ << std::string(depth * 2, ' ');

				os_ << "[ ";
				if (size > 1) {
					os_ << std::endl;
					nl_ = true;
				}
				else {
					nl_ = false;
				}
				break;
			default:
				break;
			}
		}
		void json_walker::close(type_code tag, std::size_t depth, walker_state state) {
			switch (tag) {
			case TC_TUPLE:
			case TC_DEQUE:
			case TC_PARAM_MAP:
			case TC_ATTR_MAP:
				os_ << std::string(depth * 2, ' ') << "}";
				if (state != walker_state::LAST) {
					os_ << ", ";
				}
				os_ << std::endl;
				nl_ = true;
				break;
			case TC_VECTOR:
				os_ << std::string(depth * 2, ' ') << "]";
				if (state != walker_state::LAST) {
					os_ << ", ";
				}
				os_ << std::endl;
				nl_ = true;
				break;
			default:
				break;
			}
		}

		void json_walker::pair(std::size_t n, std::size_t depth) {
			if (nl_)	os_ << std::string(depth * 2, ' ');
			else os_ << ' ';

			os_
				<< "\""
				<< n
				<< "\": "
				;
			nl_ = false;
		}
		void json_walker::pair(std::string name, std::size_t depth) {
			if (nl_)	os_ << std::string(depth * 2, ' ');
			else os_ << ' ';

			os_
				<< "\""
				<< name
				<< "\": "
				;
			nl_ = false;
		}

	}
}
