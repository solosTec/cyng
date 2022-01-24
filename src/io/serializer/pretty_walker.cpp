/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#include <cyng/io/serializer/pretty_walker.h>
#include <cyng/io/serialize.h>
#include <cyng/io/ostream.h>
#include <cyng/obj/container_cast.hpp>

namespace cyng {
	namespace io {

		pretty_walker::pretty_walker(std::ostream& os)
			: os_(os)
			, nl_(true)
		{}

		void pretty_walker::visit(object const& obj, type_code, std::size_t depth, walker_state state) {
			if (nl_)	os_ << indentation(depth);
			serialize_typed(os_, obj);
			if (state != walker_state::LAST) {
				os_ << std::endl;
				nl_ = true;
			}
			else {
				nl_ = false;
			}
		}
		void pretty_walker::open(type_code tag, std::size_t depth, std::size_t size) {
			switch (tag) {
			case TC_TUPLE:
				if (nl_)	os_ << indentation(depth);

				os_ << "{";
				nl_ = false;
				if (size > 1) {
					os_ << ' ';
				}
				break;
			case TC_VECTOR:
				if (nl_)	os_ << indentation(depth);

				os_ << "[";
				if (size > 1) {
					os_ << std::endl;
					nl_ = true;
				}
				else {
					nl_ = false;
				}
				break;
			case TC_DEQUE:
				if (nl_)	os_ << indentation(depth);

				os_ << "<";
				if (size > 1) {
					os_ << std::endl;
					nl_ = true;
				}
				else {
					nl_ = false;
				}
				break;
			case TC_PARAM_MAP:
				if (nl_)	os_ << indentation(depth);

				os_ << "%(";
				if (size > 1) {
					os_ << std::endl;
					nl_ = true;
				}
				else {
					nl_ = false;
				}
				break;
			case TC_ATTR_MAP:
				if (nl_)	os_ << indentation(depth);

				os_ << "#(";
				if (size > 1) {
					os_ << std::endl;
					nl_ = true;
				}
				else {
					nl_ = false;
				}
				break;
			case TC_PROP_MAP:
				if (nl_)	os_ << indentation(depth);

				os_ << "$(";
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
		void pretty_walker::close(type_code tag, std::size_t depth, walker_state state, type_code parent_type) {
			switch (tag) {
			case TC_TUPLE:
				if (nl_) {
					os_ << indentation(depth);
				}
				os_ << "}";
				if (state != walker_state::LAST) {
					os_ << ", " << std::endl;
					nl_ = true;
				}
				else {
					nl_ = false;
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
					nl_ = false;
				}
				break;
			case TC_DEQUE:
				if (nl_) {
					os_ << indentation(depth);
				}
				os_ << ">";
				if (state != walker_state::LAST) {
					os_ << ", " << std::endl;
					nl_ = true;
				}
				else {
					nl_ = false;
				}
				break;
			case TC_PARAM_MAP:
			case TC_ATTR_MAP:
				os_ << indentation(depth) << ")";
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

		void pretty_walker::pair(std::size_t n, std::size_t depth) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< '#'
				<< n
				<< ' '
				;
			nl_ = false;
		}
		void pretty_walker::pair(std::string const& name, std::size_t depth) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< name
				<< ':'
				;
			nl_ = false;
		}
		void pretty_walker::pair(obis const& code, std::size_t depth) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< '$'
				<< code
				<< ':'
				;
			nl_ = false;
		}

		void pretty_walker::simple(attr_t const& attr, std::size_t depth, walker_state) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< '#'
				<< attr.first
				<< ','
				<< attr.second
				<< std::endl
				;
			nl_ = true;

		}
		void pretty_walker::simple(param_t const& param, std::size_t depth, walker_state) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< param.first
				<< ':'
				<< param.second
				<< std::endl
				;
			nl_ = true;

		}
		void pretty_walker::simple(prop_t const& prop, std::size_t depth, walker_state) {
			if (nl_)	os_ << indentation(depth);
			else os_ << ' ';

			os_
				<< '$'
				<< prop.first
				<< ':'
				<< prop.second
				<< std::endl
				;
			nl_ = true;
		}

		std::string pretty_walker::indentation(std::size_t depth) {
			std::string s;
			s.reserve(depth * 2);
			for (auto idx = 0; idx < depth * 2; ++idx) {
				s.push_back((idx % 2 == 0) ? '.' : ' ');				
			}
			return s;
		}
	}
}
