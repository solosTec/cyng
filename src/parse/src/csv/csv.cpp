/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/csv.h>
#include <cyng/obj/factory.hpp>
#include <cyng/obj/container_cast.hpp>
#include <fstream>

namespace cyng {

	namespace csv {

		parser_named::parser_named(cb_f cb, char sep)
			: cb_(cb)
			, header_()
			, counter_(0)
			, parser_(sep, [this](line_t&& line) {
			if (counter_ == 0) {
				//
				//	set csv header
				//
				header_ = std::move(line);
			}
			else {
				//std::cout << line.size() << "/" << header_.size() << std::endl;
				std::map<std::string, std::string> r;
				for (auto pos_line = line.begin(), pos_header = header_.begin(); pos_line != line.end() && pos_header != header_.end(); ++pos_line, ++pos_header) {
					r.emplace(*pos_header, *pos_line);
				}
				cb_(r, counter_);
			}
			//
			//	update line counter
			//
			++counter_;
			})
		{}


	}
}

