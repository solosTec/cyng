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

		void json_walker::visit(object const& obj, type_code, std::size_t depth, walker_state state) {
			if (nl_)	os_ << indentation(depth);
			serialize_json(os_, obj);
			if (state != walker_state::LAST) {
				os_ << ", ";
				os_ << std::endl;
				nl_ = true;
			}
			else {
				nl_ = false;
			}
		}
		void json_walker::open(type_code tag, std::size_t depth, std::size_t size) {
			switch (tag) {
			case TC_TUPLE:
			case TC_DEQUE:
			case TC_PARAM_MAP:
			case TC_ATTR_MAP:
				if (nl_)	os_ << indentation(depth);

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
				if (nl_)	os_ << indentation(depth);

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
		void json_walker::close(type_code tag, std::size_t depth, walker_state state, type_code parent_type) {
			switch (tag) {
			case TC_TUPLE:
			case TC_DEQUE:
			case TC_PARAM_MAP:
			case TC_ATTR_MAP:
				if (nl_) {
					os_ << indentation(depth);
				}
				os_ << "}";
				if (state != walker_state::LAST) {
					os_ << ", " << std::endl;
					nl_ = true;
				}
				else {
					switch (parent_type) {
					case TC_TUPLE:
					case TC_VECTOR:
					case TC_DEQUE:
						nl_ = false;
						break;
					default:
						os_ << std::endl;
						nl_ = true;
						break;
					}
				}
				break;
			case TC_VECTOR:
				if (nl_) {
					os_ << indentation(depth);
				}
				os_ << "]";
				if (state != walker_state::LAST) {
					os_ << ", " << std::endl;
					nl_ = true;
				}
				else {
					switch (parent_type) {
					case TC_TUPLE:
					case TC_VECTOR:
					case TC_DEQUE:
						nl_ = false;
						break;
					default:
						os_ << std::endl;
						nl_ = true;
						break;
					}
				}
				break;
			default:
				break;
			}
		}

		void json_walker::pair(std::size_t n, std::size_t depth) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< "\""
				<< n
				<< "\": "
				;
			nl_ = false;
		}
		void json_walker::pair(std::string const& name, std::size_t depth) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< "\""
				<< name
				<< "\": "
				;
			nl_ = false;
		}
		void json_walker::pair(obis const& code, std::size_t depth) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< "\""
				<< code
				<< "\": "
				;
			nl_ = false;
		}
		void json_walker::simple(attr_t const& attr, std::size_t depth, walker_state state) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< "\""
				<< attr.first
				<< "\": "
				;
			serialize_json(os_, attr.second);

			if (state != walker_state::LAST) {
				os_ << ", ";
				nl_ = true;
			}

			os_ << std::endl;
			nl_ = true;
		}
		void json_walker::simple(param_t const& param, std::size_t depth, walker_state state) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< "\""
				<< param.first
				<< "\": "
				;

			serialize_json(os_, param.second);

			if (state != walker_state::LAST) {
					os_ << ", ";
					nl_ = true;
				}

			os_ << std::endl;
			nl_ = true;
		}
		void json_walker::simple(prop_t const& prop, std::size_t depth, walker_state state) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< "\""
				<< prop.first
				<< "\": "
				;

			serialize_json(os_, prop.second);

			if (state != walker_state::LAST) {
				os_ << ", ";
				nl_ = true;
			}

			os_ << std::endl;
			nl_ = true;
		}

		std::string json_walker::indentation(std::size_t depth) {
			return std::string(depth * 2, ' ');
		}

	}
}
